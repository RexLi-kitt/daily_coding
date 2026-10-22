"""Train the existing CNN on Kaggle CSVs and export submission.csv.

Put train.csv and test.csv in kaggle_data beside this script, then run:
    python kaggle_digit_recognizer.py
Reuse a model trained by this script:
    python kaggle_digit_recognizer.py --predict-only
"""

import argparse
import csv
import random
import time
from pathlib import Path

import numpy as np
import torch
from torch import nn
from torch.utils.data import DataLoader, TensorDataset, random_split


PROJECT_DIR = Path(__file__).resolve().parent


def make_model():
    # Same architecture as mnist_cnn.py; weights are trained from scratch.
    return nn.Sequential(
        nn.Conv2d(1, 6, kernel_size=5, padding=2), nn.ReLU(),
        nn.AvgPool2d(kernel_size=2, stride=2),
        nn.Conv2d(6, 16, kernel_size=5), nn.ReLU(),
        nn.AvgPool2d(kernel_size=2, stride=2),
        nn.Flatten(),
        nn.Linear(16 * 5 * 5, 120), nn.ReLU(),
        nn.Linear(120, 84), nn.ReLU(),
        nn.Linear(84, 10),
    )


def load_csv(path, labeled):
    """Return uint8 images [N, 1, 28, 28] and optional int64 labels."""
    expected = (["label"] if labeled else []) + [f"pixel{i}" for i in range(784)]
    with path.open(newline="", encoding="utf-8-sig") as file:
        if next(csv.reader(file), None) != expected:
            raise ValueError(f"Unexpected CSV columns in {path}")
        values = np.loadtxt(file, delimiter=",", dtype=np.float32, ndmin=2)
    if not len(values) or values.shape[1] != len(expected):
        raise ValueError(f"Empty data or incorrect column count in {path}")
    if not np.isfinite(values).all() or not np.equal(values, np.floor(values)).all():
        raise ValueError(f"CSV must contain finite integers: {path}")
    pixels = values[:, 1:] if labeled else values
    if pixels.min() < 0 or pixels.max() > 255:
        raise ValueError(f"Pixel values must be between 0 and 255: {path}")
    images = torch.from_numpy(pixels.astype(np.uint8)).reshape(-1, 1, 28, 28)
    labels = None
    if labeled:
        if values[:, 0].min() < 0 or values[:, 0].max() > 9:
            raise ValueError(f"Labels must be between 0 and 9: {path}")
        labels = torch.from_numpy(values[:, 0].astype(np.int64))
    return images, labels


@torch.inference_mode()
def evaluate(model, loader, device):
    model.eval()
    loss_sum, correct, total = 0.0, 0, 0
    for images, labels in loader:
        images = images.to(device).float().div_(255)
        labels = labels.to(device)
        logits = model(images)
        loss_sum += nn.functional.cross_entropy(logits, labels, reduction="sum").item()
        correct += (logits.argmax(1) == labels).sum().item()
        total += len(labels)
    return loss_sum / total, correct / total


def plot_history(rows, output_dir):
    import matplotlib
    matplotlib.use("Agg")
    from matplotlib import pyplot as plt

    for metric, ylabel in (("loss", "Cross-entropy loss"), ("acc", "Accuracy (%)")):
        fig, ax = plt.subplots(figsize=(6, 4))
        for split, label in (("train", "Training"), ("val", "Validation")):
            scale = 100 if metric == "acc" else 1
            ax.plot([r["epoch"] for r in rows],
                    [r[f"{split}_{metric}"] * scale for r in rows], label=label)
        ax.set(xlabel="Epoch", ylabel=ylabel, title="Digit Recognizer CNN")
        ax.legend()
        ax.grid(alpha=0.25)
        fig.tight_layout()
        fig.savefig(output_dir / f"{metric}_curve.png", dpi=180)
        plt.close(fig)


def train(args, device):
    images, labels = load_csv(args.train_csv, labeled=True)
    dataset = TensorDataset(images, labels)
    val_n = max(1, round(len(dataset) * args.val_ratio))
    if val_n >= len(dataset):
        raise ValueError("Training CSV needs enough rows for training and validation")
    train_data, val_data = random_split(
        dataset, [len(dataset) - val_n, val_n],
        generator=torch.Generator().manual_seed(args.seed))
    pin = device.type == "cuda"
    train_loader = DataLoader(
        train_data, batch_size=args.batch_size, shuffle=True, pin_memory=pin,
        generator=torch.Generator().manual_seed(args.seed))
    train_eval = DataLoader(train_data, batch_size=1024, pin_memory=pin)
    val_eval = DataLoader(val_data, batch_size=1024, pin_memory=pin)
    model = make_model().to(device)
    optimizer = torch.optim.SGD(model.parameters(), lr=args.lr)
    print(f"Training: {len(train_data)}; validation: {len(val_data)}", flush=True)

    fields = ["epoch", "batch_loss", "train_loss", "val_loss", "train_acc", "val_acc", "seconds"]
    rows = []
    best_acc = -1.0
    with (args.output_dir / "history.csv").open("w", newline="", encoding="utf-8") as file:
        writer = csv.DictWriter(file, fieldnames=fields)
        writer.writeheader()
        for epoch in range(1, args.epochs + 1):
            start = time.perf_counter()
            model.train()
            batch_loss = 0.0
            for batch_images, batch_labels in train_loader:
                batch_images = batch_images.to(device, non_blocking=pin).float().div_(255)
                batch_labels = batch_labels.to(device, non_blocking=pin)
                optimizer.zero_grad(set_to_none=True)
                loss = nn.functional.cross_entropy(model(batch_images), batch_labels)
                loss.backward()
                optimizer.step()
                batch_loss += loss.item() * len(batch_labels)
            train_loss, train_acc = evaluate(model, train_eval, device)
            val_loss, val_acc = evaluate(model, val_eval, device)
            row = dict(zip(fields, (
                epoch, batch_loss / len(train_data), train_loss, val_loss,
                train_acc, val_acc, time.perf_counter() - start)))
            rows.append(row)
            writer.writerow(row)
            file.flush()
            if val_acc > best_acc:
                best_acc = val_acc
                torch.save(model.state_dict(), args.output_dir / "kaggle_cnn.pt")
            print(f"Epoch {epoch:02d}/{args.epochs}: train loss={train_loss:.4f}, "
                  f"val loss={val_loss:.4f}, train acc={train_acc:.2%}, "
                  f"val acc={val_acc:.2%}", flush=True)
    plot_history(rows, args.output_dir)
    print(f"Best validation accuracy: {best_acc:.2%}", flush=True)


@torch.inference_mode()
def export_submission(args, device, test_images):
    model = make_model().to(device)
    model.load_state_dict(torch.load(
        args.output_dir / "kaggle_cnn.pt", map_location=device, weights_only=True))
    model.eval()
    # shuffle=False preserves the row order of Kaggle test.csv.
    loader = DataLoader(TensorDataset(test_images), batch_size=args.batch_size, shuffle=False)
    path = args.output_dir / "submission.csv"
    with path.open("w", newline="", encoding="utf-8") as file:
        writer = csv.writer(file)
        writer.writerow(["ImageId", "Label"])
        image_id = 1
        for (images,) in loader:
            predictions = model(images.to(device).float().div_(255)).argmax(1).cpu().tolist()
            for prediction in predictions:
                writer.writerow([image_id, prediction])
                image_id += 1
    print(f"Saved {len(test_images)} predictions to {path}", flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--train-csv", type=Path, default=PROJECT_DIR / "kaggle_data" / "train.csv")
    parser.add_argument("--test-csv", type=Path, default=PROJECT_DIR / "kaggle_data" / "test.csv")
    parser.add_argument("--output-dir", type=Path, default=PROJECT_DIR / "kaggle_output")
    parser.add_argument("--epochs", type=int, default=50)
    parser.add_argument("--batch-size", type=int, default=256)
    parser.add_argument("--lr", type=float, default=0.01)
    parser.add_argument("--val-ratio", type=float, default=0.2)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--device", choices=("auto", "cuda", "cpu"), default="auto")
    parser.add_argument("--predict-only", action="store_true", help="Load kaggle_cnn.pt and export predictions without training")
    args = parser.parse_args()
    if args.epochs < 1 or args.batch_size < 1 or not np.isfinite(args.lr) or args.lr <= 0:
        parser.error("epochs, batch-size and lr must be positive and finite")
    if not 0 < args.val_ratio < 1:
        parser.error("val-ratio must be between 0 and 1")
    required = [args.test_csv]
    required.append(args.output_dir / "kaggle_cnn.pt" if args.predict_only else args.train_csv)
    for path in required:
        if not path.is_file():
            parser.error(f"File not found: {path}")
    if args.device == "cuda" and not torch.cuda.is_available():
        parser.error("CUDA is unavailable; use --device cpu or --device auto")
    device = torch.device("cuda" if args.device == "auto" and torch.cuda.is_available()
                          else "cpu" if args.device == "auto" else args.device)
    random.seed(args.seed)
    np.random.seed(args.seed)
    torch.manual_seed(args.seed)
    if device.type == "cuda":
        torch.cuda.manual_seed_all(args.seed)
    print(f"Using {device}", flush=True)
    # Validate test input before spending time training.
    test_images, _ = load_csv(args.test_csv, labeled=False)
    args.output_dir.mkdir(parents=True, exist_ok=True)
    if not args.predict_only:
        train(args, device)
    export_submission(args, device, test_images)


if __name__ == "__main__":
    main()
