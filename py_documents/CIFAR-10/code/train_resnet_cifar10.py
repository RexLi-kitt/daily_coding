"""Train ResNet models and save metrics/checkpoints; plotting is a separate command."""


import argparse
import csv
import json
import time
from pathlib import Path

import torch
from torch import nn

from models import STAGES, MODEL_LABELS, BasicBlock, Bottleneck, PlainBottleneck, ResNet
from data import load_cifar10, augment_batch


ROOT = Path(__file__).resolve().parents[1]
ARCHIVE = ROOT.parent / "cifar-10-python.tar.gz"


PAPER_URL = "https://www.microsoft.com/en-us/research/wp-content/uploads/2021/07/Deep-Residual-Learning-for-Image-Recognition.pdf"


@torch.inference_mode()
def evaluate(model, images, labels, mean, std, batch_size, loss_fn):
    model.eval()
    loss_sum = correct = top5_correct = 0
    for start in range(0, len(labels), batch_size):
        x = ((images[start:start + batch_size] - mean) / std)
        x = x.contiguous(memory_format=torch.channels_last)
        y = labels[start:start + batch_size]
        with torch.autocast("cuda", dtype=torch.bfloat16):
            logits = model(x)
            loss = loss_fn(logits, y)
        loss_sum += loss.item() * len(y)
        correct += (logits.argmax(1) == y).sum().item()
        top5_correct += logits.topk(5, dim=1).indices.eq(y[:, None]).any(dim=1).sum().item()
    return loss_sum / len(labels), correct / len(labels), 1 - top5_correct / len(labels)


def write_metrics(path: Path, rows):
    with path.open("w", newline="", encoding="utf-8") as file:
        writer = csv.DictWriter(file, fieldnames=rows[0].keys())
        writer.writeheader()
        writer.writerows(rows)


def train_one(name, train_x, train_y, test_x, test_y, mean, std, args,
              initial_state=None, training_seed=None):
    torch.manual_seed(args.seed)
    torch.cuda.manual_seed_all(args.seed)
    block = (BasicBlock if name == "resnet18" else
             PlainBottleneck if name == "plain50" else Bottleneck)
    model = ResNet(STAGES[name], block=block).cuda().to(memory_format=torch.channels_last)
    if initial_state is not None:
        model_keys = set(model.state_dict())
        missing = model_keys - set(initial_state)
        if missing:
            raise ValueError(f"Paired initialization is missing tensors: {sorted(missing)}")
        model.load_state_dict({key: initial_state[key] for key in model_keys}, strict=True)
    optimizer = torch.optim.SGD(model.parameters(), lr=args.lr,
                                momentum=0.9, weight_decay=1e-4)
    milestones = [round(args.epochs * 0.6), round(args.epochs * 0.8)]
    loss_fn = nn.CrossEntropyLoss()
    parameter_count = sum(parameter.numel() for parameter in model.parameters())
    print(f"{MODEL_LABELS[name]} ({name}): {parameter_count:,} parameters", flush=True)
    if training_seed is not None:
        # Decouple data randomness from the different model-construction RNG draws.
        torch.manual_seed(training_seed)
        torch.cuda.manual_seed_all(training_seed)
    started = time.perf_counter()
    rows = []

    for epoch in range(1, args.epochs + 1):
        # Short warmup stabilizes the deep network when trained from scratch on 32x32 images.
        warmup_epochs = min(3, args.epochs)
        if epoch <= warmup_epochs:
            lr = args.lr * (0.2 + 0.8 * (epoch - 1) / max(1, warmup_epochs - 1))
        else:
            lr = args.lr * (0.1 ** sum(epoch > milestone for milestone in milestones))
        for group in optimizer.param_groups:
            group["lr"] = lr
        model.train()
        permutation = torch.randperm(len(train_y), device="cuda")
        loss_sum = correct = top5_correct = 0
        for start in range(0, len(train_y), args.batch_size):
            indices = permutation[start:start + args.batch_size]
            x = augment_batch(train_x[indices], mean, std)
            y = train_y[indices]
            optimizer.zero_grad(set_to_none=True)
            with torch.autocast("cuda", dtype=torch.bfloat16):
                logits = model(x)
                loss = loss_fn(logits, y)
            loss.backward()
            optimizer.step()
            loss_sum += loss.item() * len(y)
            correct += (logits.argmax(1) == y).sum().item()
            top5_correct += logits.topk(5, dim=1).indices.eq(y[:, None]).any(dim=1).sum().item()

        test_loss, test_acc, test_top5_err = evaluate(
            model, test_x, test_y, mean, std, args.batch_size, loss_fn
        )
        row = {
            "epoch": epoch,
            "train_loss": loss_sum / len(train_y),
            "test_loss": test_loss,
            "train_acc": correct / len(train_y),
            "test_acc": test_acc,
            "train_top5_err": 1 - top5_correct / len(train_y),
            "test_top5_err": test_top5_err,
            "learning_rate": lr,
        }
        rows.append(row)
        write_metrics(args.output_dir / f"{name}_cifar10_metrics.csv", rows)
        print(f"{name} epoch {epoch:02d}/{args.epochs}: "
              f"train loss {row['train_loss']:.4f}, test loss {test_loss:.4f}, "
              f"train acc {row['train_acc']:.4f}, test acc {test_acc:.4f}, "
              f"test top-5 err {test_top5_err:.4f}, lr {lr:g}", flush=True)

    torch.cuda.synchronize()
    checkpoint_name = f"{name}_cifar10_final.pt"
    torch.save({"model_state_dict": model.state_dict(), "model": name,
                "block_type": block.__name__, "blocks": STAGES[name],
                "num_classes": 10, "epoch": args.epochs,
                "residual_connections": name != "plain50",
                "mean": mean.detach().cpu(), "std": std.detach().cpu()},
               args.output_dir / checkpoint_name)
    summary = {
        "model": name,
        "display_name": MODEL_LABELS[name],
        "depth": 2 + (2 if block is BasicBlock else 3) * sum(STAGES[name]),
        "architecture_note": (
            "Custom 26-layer [2,2,2,2] Bottleneck model"
            if name == "resnet26" else
            "50-layer plain Bottleneck convolutional network; no shortcut branches or additions"
            if name == "plain50" else f"Original paper ResNet v1 {block.__name__} architecture"
        ),
        "paper": PAPER_URL,
        "blocks": STAGES[name],
        "block_type": block.__name__,
        "residual_connections": name != "plain50",
        "parameters": parameter_count,
        "checkpoint": checkpoint_name,
        "input_size": "3x32x32",
        "classifier_classes": 10,
        "optimizer": "SGD, momentum=0.9, weight_decay=0.0001",
        "learning_rate": args.lr,
        "lr_milestones": milestones,
        "warmup_epochs": min(3, args.epochs),
        "batch_size": args.batch_size,
        "epochs": args.epochs,
        "seed": args.seed,
        "training_seed": args.seed if training_seed is None else training_seed,
        "initialization": "shared-tensor paired initialization" if initial_state is not None else "default initialization",
        "augmentation": "random crop from 4-pixel padding, random horizontal flip",
        "normalization": "CIFAR-10 training-set channel mean and std",
        "precision": "bfloat16 autocast",
        "device": torch.cuda.get_device_name(0),
        "elapsed_seconds": round(time.perf_counter() - started, 2),
        "final": rows[-1],
        "best_test_acc": max(row["test_acc"] for row in rows),
        "best_test_epoch": max(rows, key=lambda row: row["test_acc"])["epoch"],
    }
    (args.output_dir / f"{name}_cifar10_summary.json").write_text(
        json.dumps(summary, indent=2, ensure_ascii=False), encoding="utf-8"
    )
    del model, optimizer
    torch.cuda.empty_cache()
    return rows


def main(argv=None):
    parser = argparse.ArgumentParser()
    parser.add_argument("--epochs", type=int, default=80)
    parser.add_argument("--output-dir", type=Path, default=ROOT,
                        help="directory for metrics, summaries and checkpoints")
    parser.add_argument("--batch-size", type=int, default=512)
    parser.add_argument("--lr", type=float, default=0.05)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--models", nargs="+", choices=tuple(STAGES),
                        default=["resnet18", "resnet50"],
                        help="resnet18: BasicBlock; resnet26: custom Bottleneck; plain50: 50 layers without shortcuts")
    parser.add_argument("--archive", type=Path, default=ARCHIVE)
    args = parser.parse_args(argv)
    args.output_dir = args.output_dir.resolve()
    args.output_dir.mkdir(parents=True, exist_ok=True)
    if not torch.cuda.is_available():
        raise RuntimeError("CUDA GPU is required")
    torch.backends.cudnn.benchmark = True
    train_x, train_y, test_x, test_y = load_cifar10(args.archive, torch.device("cuda"))
    mean = train_x.mean(dim=(0, 2, 3), keepdim=True)
    std = train_x.std(dim=(0, 2, 3), keepdim=True)
    print(f"GPU: {torch.cuda.get_device_name(0)}; "
          f"train={len(train_y)}, test={len(test_y)}", flush=True)
    for name in args.models:
        train_one(name, train_x, train_y, test_x, test_y, mean, std, args)
    print(f"Saved training metrics, summaries and checkpoints in {args.output_dir}", flush=True)


if __name__ == "__main__":
    main()
