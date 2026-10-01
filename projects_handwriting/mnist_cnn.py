"""Train the D2L LeNet-style CNN on local MNIST and plot its predictions.

Examples (from this directory):
    python mnist_cnn.py train
    python mnist_cnn.py predict --test-index 42
    python mnist_cnn.py predict --image digit.png --invert
"""

import argparse
import csv
import random
import time
from pathlib import Path

import torch
from torch import nn
from torch.utils.data import DataLoader, TensorDataset

from mnist_mlp import load_split
from train_mnist_curves import evaluate, plot_history


DATA_DIR = Path(__file__).resolve().parent
MODEL_PATH = DATA_DIR / "mnist_cnn.pt"
FIGURE_DIR = DATA_DIR / "figures" / "CNN"


def make_model():
    # Same LeNet-style architecture as D2L/卷积神经网络.py, with 10 digit classes.
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


def plot_predictions(model, images, labels, indices, device, path):
    """Show selected test digits with their predicted and true labels."""
    from matplotlib import pyplot as plt

    chosen = images[indices]
    with torch.inference_mode():
        predictions = model(chosen.to(device).float().div_(255)).argmax(1).cpu()
    fig, axes = plt.subplots(4, 4, figsize=(8, 8))
    for ax, image, index, pred in zip(axes.flat, chosen, indices, predictions):
        truth = labels[index].item()
        ax.imshow(image.squeeze(0), cmap="gray", vmin=0, vmax=255)
        ax.set_title(f"Pred {pred.item()}  |  True {truth}",
                     color="#16803c" if pred.item() == truth else "#c62828", fontsize=10)
        ax.axis("off")
    fig.suptitle("MNIST CNN: test-set predictions", fontsize=14)
    fig.tight_layout()
    path.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(path, dpi=180)
    plt.close(fig)


def train(args, device):
    random.seed(args.seed)
    torch.manual_seed(args.seed)
    if device.type == "cuda":
        torch.cuda.manual_seed_all(args.seed)

    train_images, train_labels = load_split("train")
    test_images, test_labels = load_split("t10k")
    train_data = TensorDataset(train_images, train_labels)
    test_data = TensorDataset(test_images, test_labels)
    pin = device.type == "cuda"
    train_loader = DataLoader(train_data, batch_size=args.batch_size, shuffle=True,
                              pin_memory=pin, generator=torch.Generator().manual_seed(args.seed))
    train_eval = DataLoader(train_data, batch_size=1024, pin_memory=pin)
    test_eval = DataLoader(test_data, batch_size=1024, pin_memory=pin)

    model = make_model().to(device)
    optimizer = torch.optim.Adam(model.parameters(), lr=args.lr)
    args.output_dir.mkdir(parents=True, exist_ok=True)
    history_path = args.output_dir / "history.csv"
    fields = ["epoch", "batch_loss", "train_loss", "test_loss", "train_acc", "test_acc",
              "train_correct", "test_correct", "train_n", "test_n", "seconds"]
    rows = []
    with history_path.open("w", newline="", encoding="utf-8") as file:
        writer = csv.DictWriter(file, fieldnames=fields)
        writer.writeheader()
        for epoch in range(1, args.epochs + 1):
            start = time.perf_counter()
            model.train()
            batch_loss_sum = 0.0
            for images, labels in train_loader:
                images = images.to(device, non_blocking=pin).float().div_(255)
                labels = labels.to(device, non_blocking=pin)
                optimizer.zero_grad(set_to_none=True)
                loss = nn.functional.cross_entropy(model(images), labels)
                loss.backward()
                optimizer.step()
                batch_loss_sum += loss.item() * len(labels)

            train_loss, train_acc, train_correct, train_n = evaluate(model, train_eval, device)
            test_loss, test_acc, test_correct, test_n = evaluate(model, test_eval, device)
            row = dict(zip(fields, (epoch, batch_loss_sum / train_n,
                                    train_loss, test_loss, train_acc, test_acc,
                                    train_correct, test_correct, train_n, test_n,
                                    time.perf_counter() - start)))
            rows.append(row)
            writer.writerow(row)
            file.flush()
            print(f"Epoch {epoch:02d}/{args.epochs}: "
                  f"train loss={train_loss:.4f}, test loss={test_loss:.4f}, "
                  f"train acc={train_acc:.2%}, test acc={test_acc:.2%}", flush=True)

    args.model.parent.mkdir(parents=True, exist_ok=True)
    torch.save(model.cpu().state_dict(), args.model)
    plot_history(rows, args.output_dir)
    model.to(device).eval()
    indices = torch.randperm(len(test_images), generator=torch.Generator().manual_seed(args.seed))[:16]
    plot_predictions(model, test_images, test_labels, indices, device,
                     args.output_dir / "predictions.png")
    print(f"Saved model to {args.model}; curves and predictions to {args.output_dir}")


def predict(args, device):
    from matplotlib import pyplot as plt

    model = make_model().to(device)
    model.load_state_dict(torch.load(args.model, map_location=device, weights_only=True))
    model.eval()
    if args.test_index is not None:
        images, labels = load_split("t10k")
        if not 0 <= args.test_index < len(images):
            raise ValueError(f"Test index must be between 0 and {len(images) - 1}")
        image = images[args.test_index]
        truth = labels[args.test_index].item()
        default_name = f"prediction_test_{args.test_index}.png"
    else:
        from PIL import Image, ImageOps

        with Image.open(args.image) as source:
            gray = ImageOps.grayscale(source).resize((28, 28), Image.Resampling.LANCZOS)
            if args.invert:
                gray = ImageOps.invert(gray)
            image = torch.frombuffer(bytearray(gray.tobytes()), dtype=torch.uint8).reshape(1, 28, 28)
        truth = None
        default_name = f"prediction_{args.image.stem}.png"

    with torch.inference_mode():
        probabilities = model(image.unsqueeze(0).to(device).float().div_(255)).softmax(1)[0].cpu()
    digit = probabilities.argmax().item()
    print(f"Predicted digit: {digit} (confidence {probabilities[digit].item():.2%})")
    if truth is not None:
        print(f"True label: {truth}")

    save_path = args.save or FIGURE_DIR / default_name
    save_path.parent.mkdir(parents=True, exist_ok=True)
    fig, (image_ax, probability_ax) = plt.subplots(1, 2, figsize=(8, 3.5),
                                                  gridspec_kw={"width_ratios": [1, 1.6]})
    image_ax.imshow(image.squeeze(0), cmap="gray", vmin=0, vmax=255)
    image_ax.set_title(f"Prediction: {digit}" + (f"  |  True: {truth}" if truth is not None else ""))
    image_ax.axis("off")
    probability_ax.bar(range(10), probabilities.numpy(), color="#2563a6")
    probability_ax.set_xticks(range(10))
    probability_ax.set_xlabel("Digit")
    probability_ax.set_ylabel("Probability")
    probability_ax.set_ylim(0, 1)
    fig.tight_layout()
    fig.savefig(save_path, dpi=180)
    plt.close(fig)
    print(f"Prediction figure saved to {save_path}")


def main():
    parser = argparse.ArgumentParser(description="Train a LeNet-style CNN on MNIST")
    parser.add_argument("--device", choices=("auto", "cuda", "cpu"), default="auto")
    parser.add_argument("--model", type=Path, default=MODEL_PATH)
    subparsers = parser.add_subparsers(dest="command", required=True)

    training = subparsers.add_parser("train", help="Train and save loss, accuracy and prediction figures")
    training.add_argument("--epochs", type=int, default=20)
    training.add_argument("--batch-size", type=int, default=256)
    training.add_argument("--lr", type=float, default=0.001)
    training.add_argument("--seed", type=int, default=42)
    training.add_argument("--output-dir", type=Path, default=FIGURE_DIR)

    prediction = subparsers.add_parser("predict", help="Classify one digit and save its prediction figure")
    source = prediction.add_mutually_exclusive_group(required=True)
    source.add_argument("--image", type=Path)
    source.add_argument("--test-index", type=int)
    prediction.add_argument("--invert", action="store_true", help="Use for dark digits on a light background")
    prediction.add_argument("--save", type=Path, help="Output PNG path")

    args = parser.parse_args()
    if args.device == "cuda" and not torch.cuda.is_available():
        parser.error("CUDA is unavailable; use --device cpu or --device auto")
    device = torch.device("cuda" if args.device == "auto" and torch.cuda.is_available()
                          else "cpu" if args.device == "auto" else args.device)
    print(f"Using {torch.cuda.get_device_name(0) if device.type == 'cuda' else 'CPU'}", flush=True)
    if args.command == "train":
        if args.epochs < 1 or args.batch_size < 1 or args.lr <= 0:
            parser.error("epochs, batch-size and lr must be positive")
        train(args, device)
    else:
        predict(args, device)


if __name__ == "__main__":
    main()
