"""Train MLP/CNN and save CSV/JSON metrics; plotting is a separate command."""


import argparse
import csv
import json
import time
from pathlib import Path

import torch
from torch import nn

from models import make_mlp, make_cnn
from data import load_cifar10


ROOT = Path(__file__).resolve().parents[1]
ARCHIVE = ROOT.parent / "cifar-10-python.tar.gz"


@torch.no_grad()
def evaluate(model, images, labels, batch_size, loss_fn):
    model.eval()
    total_loss = 0.0
    correct = 0
    top5_correct = 0
    for start in range(0, labels.numel(), batch_size):
        x = images[start:start + batch_size]
        y = labels[start:start + batch_size]
        logits = model(x)
        total_loss += loss_fn(logits, y).item() * y.numel()
        correct += (logits.argmax(1) == y).sum().item()
        top5_correct += logits.topk(5, dim=1).indices.eq(y[:, None]).any(dim=1).sum().item()
    return total_loss / labels.numel(), correct / labels.numel(), 1 - top5_correct / labels.numel()


def main(argv=None):
    parser = argparse.ArgumentParser()
    parser.add_argument("--model", choices=("mlp", "cnn"), default="mlp")
    parser.add_argument("--epochs", type=int, default=80)
    parser.add_argument("--batch-size", type=int, default=256)
    parser.add_argument("--lr", type=float, default=0.1)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--archive", type=Path, default=ARCHIVE)
    parser.add_argument("--output-dir", type=Path, default=ROOT)
    args = parser.parse_args(argv)
    output_dir = args.output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)

    if not torch.cuda.is_available():
        raise RuntimeError("CUDA GPU is required for this run")
    device = torch.device("cuda")
    torch.manual_seed(args.seed)
    torch.cuda.manual_seed_all(args.seed)
    train_x, train_y, test_x, test_y = load_cifar10(args.archive, device)
    model = (make_mlp() if args.model == "mlp" else make_cnn()).to(device)
    optimizer = torch.optim.SGD(model.parameters(), lr=args.lr)
    loss_fn = nn.CrossEntropyLoss()
    rows = []
    print(f"Model: {args.model.upper()}; device: {torch.cuda.get_device_name(0)}; "
          f"train={len(train_y)}, test={len(test_y)}", flush=True)
    started = time.perf_counter()

    for epoch in range(1, args.epochs + 1):
        model.train()
        permutation = torch.randperm(len(train_y), device=device)
        total_loss = 0.0
        correct = 0
        top5_correct = 0
        for start in range(0, len(train_y), args.batch_size):
            indices = permutation[start:start + args.batch_size]
            x, y = train_x[indices], train_y[indices]
            optimizer.zero_grad(set_to_none=True)
            logits = model(x)
            loss = loss_fn(logits, y)
            loss.backward()
            optimizer.step()
            total_loss += loss.item() * y.numel()
            correct += (logits.argmax(1) == y).sum().item()
            top5_correct += logits.topk(5, dim=1).indices.eq(y[:, None]).any(dim=1).sum().item()

        test_loss, test_acc, test_top5_err = evaluate(
            model, test_x, test_y, args.batch_size, loss_fn
        )
        row = {
            "epoch": epoch,
            "train_loss": total_loss / len(train_y),
            "test_loss": test_loss,
            "train_acc": correct / len(train_y),
            "test_acc": test_acc,
            "train_top5_err": 1 - top5_correct / len(train_y),
            "test_top5_err": test_top5_err,
        }
        rows.append(row)
        print(f"epoch {epoch:02d}/{args.epochs}: train loss {row['train_loss']:.4f}, "
              f"test loss {test_loss:.4f}, train acc {row['train_acc']:.4f}, "
              f"test acc {test_acc:.4f}, train top-5 err {row['train_top5_err']:.4f}, "
              f"test top-5 err {test_top5_err:.4f}", flush=True)

    prefix = f"{args.model}_cifar10"
    with (output_dir / f"{prefix}_metrics.csv").open("w", newline="", encoding="utf-8") as file:
        writer = csv.DictWriter(file, fieldnames=rows[0].keys())
        writer.writeheader()
        writer.writerows(rows)
    summary = {
        "model": args.model,
        "architecture": (
            "3072-256-ReLU-512-ReLU-512-Tanh-256-ReLU-10"
            if args.model == "mlp" else
            "Conv(3,6,5,pad=2)-ReLU-AvgPool(2)-Conv(6,16,5)-ReLU-AvgPool(2)-Flatten-120-ReLU-84-ReLU-10"
        ),
        "preprocessing": "ToTensor (uint8 / 255)",
        "optimizer": "SGD",
        "learning_rate": args.lr,
        "batch_size": args.batch_size,
        "epochs": args.epochs,
        "seed": args.seed,
        "device": torch.cuda.get_device_name(0),
        "elapsed_seconds": round(time.perf_counter() - started, 2),
        "final": rows[-1],
        "best_test_acc": max(row["test_acc"] for row in rows),
        "best_test_epoch": max(rows, key=lambda row: row["test_acc"])["epoch"],
    }
    (output_dir / f"{prefix}_summary.json").write_text(
        json.dumps(summary, indent=2, ensure_ascii=False), encoding="utf-8"
    )
    print(json.dumps(summary, ensure_ascii=False), flush=True)


if __name__ == "__main__":
    main()
