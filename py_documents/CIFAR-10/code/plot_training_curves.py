"""Plot saved per-model CSV results; no PyTorch import or training dependency."""


import argparse
import csv
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


ROOT = Path(__file__).resolve().parents[1]


MODEL_LABELS = {"resnet18": "ResNet-18 (BasicBlock)", "resnet26": "ResNet-26 (Bottleneck)",
                "plain50": "Plain-50 (no shortcuts)",
                "resnet50": "ResNet-50", "resnet152": "ResNet-152"}


def save_plots(rows, output_dir, model_name):
    epochs = [row["epoch"] for row in rows]
    prefix = f"{model_name}_cifar10"
    fig, ax = plt.subplots(figsize=(8, 5), dpi=160)
    ax.plot(epochs, [row["train_loss"] for row in rows], label="Train loss", linewidth=2)
    ax.plot(epochs, [row["test_loss"] for row in rows], label="Test loss", linewidth=2)
    ax.set(xlabel="Epoch", ylabel="Cross-entropy loss", title=f"CIFAR-10 {model_name.upper()}: Loss")
    ax.grid(alpha=0.25)
    ax.legend()
    fig.tight_layout()
    fig.savefig(output_dir / f"{prefix}_loss.png")
    plt.close(fig)

    fig, ax = plt.subplots(figsize=(8, 5), dpi=160)
    ax.plot(epochs, [100 * row["train_acc"] for row in rows], label="Train acc", linewidth=2)
    ax.plot(epochs, [100 * row["test_acc"] for row in rows], label="Test acc", linewidth=2)
    ax.set(xlabel="Epoch", ylabel="Accuracy (%)", title=f"CIFAR-10 {model_name.upper()}: Accuracy")
    ax.grid(alpha=0.25)
    ax.legend()
    fig.tight_layout()
    fig.savefig(output_dir / f"{prefix}_accuracy.png")
    plt.close(fig)

    fig, ax = plt.subplots(figsize=(8, 5), dpi=160)
    ax.plot(epochs, [100 * row["train_top5_err"] for row in rows],
            label="Train top-5 err", linewidth=2)
    ax.plot(epochs, [100 * row["test_top5_err"] for row in rows],
            label="Test top-5 err", linewidth=2)
    ax.set(xlabel="Epoch", ylabel="Top-5 error (%)",
           title=f"CIFAR-10 {model_name.upper()}: Top-5 Error")
    ax.grid(alpha=0.25)
    ax.legend()
    fig.tight_layout()
    fig.savefig(output_dir / f"{prefix}_top5_error.png")
    plt.close(fig)


def plot_comparison(all_rows, output_dir=ROOT):
    colors = {"resnet18": "#16a34a", "resnet26": "#d97706", "plain50": "#71717a", "resnet50": "#2563eb", "resnet152": "#dc2626"}
    names = list(all_rows)
    filename_prefix = names[0] + "".join("_vs_" + name.removeprefix("resnet")
                                        for name in names[1:])
    title = " vs ".join(MODEL_LABELS[name] for name in names)
    if len(names) > 2:
        title = "ResNet comparison"
    fig, ax = plt.subplots(figsize=(9, 5.5), dpi=160)
    for name, rows in all_rows.items():
        epochs = [row["epoch"] for row in rows]
        label = MODEL_LABELS[name]
        ax.plot(epochs, [row["train_loss"] for row in rows],
                color=colors[name], label=f"{label} train", linewidth=2)
        ax.plot(epochs, [row["test_loss"] for row in rows],
                color=colors[name], label=f"{label} test", linewidth=2, linestyle="--")
    ax.set_yscale("log")
    ax.set(xlabel="Epoch", ylabel="Cross-entropy loss (log scale)",
           title=f"CIFAR-10: {title} — Loss")
    ax.grid(alpha=0.25)
    ax.legend(ncol=2)
    fig.tight_layout()
    fig.savefig(output_dir / f"{filename_prefix}_loss.png")
    plt.close(fig)

    fig, (ax, ax_top5) = plt.subplots(2, 1, figsize=(9, 8), dpi=160, sharex=True)
    for name, rows in all_rows.items():
        epochs = [row["epoch"] for row in rows]
        label = MODEL_LABELS[name]
        ax.plot(epochs, [100 * row["train_acc"] for row in rows],
                color=colors[name], label=f"{label} train", linewidth=2)
        ax.plot(epochs, [100 * row["test_acc"] for row in rows],
                color=colors[name], label=f"{label} test", linewidth=2, linestyle="--")
        ax_top5.plot(epochs, [100 * row["train_top5_err"] for row in rows],
                     color=colors[name], label=f"{label} train", linewidth=2)
        ax_top5.plot(epochs, [100 * row["test_top5_err"] for row in rows],
                     color=colors[name], label=f"{label} test", linewidth=2, linestyle="--")
    ax.set(xlabel="Epoch", ylabel="Top-1 accuracy (%)",
           title=f"CIFAR-10: {title} — Accuracy")
    ax.grid(alpha=0.25)
    ax.legend(ncol=2)
    ax_top5.set(xlabel="Epoch", ylabel="Top-5 error (%)")
    ax_top5.grid(alpha=0.25)
    ax_top5.legend(ncol=2)
    fig.tight_layout()
    fig.savefig(output_dir / f"{filename_prefix}_accuracy.png")
    plt.close(fig)


def main(argv=None):
    parser = argparse.ArgumentParser(description="Draw saved training metrics without loading PyTorch or retraining.")
    parser.add_argument("--models", nargs="+", required=True, choices=tuple(MODEL_LABELS) + ("cnn", "mlp"))
    parser.add_argument("--input-dir", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path)
    args = parser.parse_args(argv)
    output_dir = (args.output_dir or args.input_dir).resolve()
    output_dir.mkdir(parents=True, exist_ok=True)
    residual_rows = {}
    for name in args.models:
        with (args.input_dir / f"{name}_cifar10_metrics.csv").open(newline="", encoding="utf-8-sig") as file:
            rows = [{key: int(value) if key == "epoch" else float(value) for key, value in row.items()} for row in csv.DictReader(file)]
        if not rows:
            raise ValueError(f"No metrics for {name}")
        if name in ("cnn", "mlp"):
            save_plots(rows, output_dir, name)
        else:
            residual_rows[name] = rows
    if residual_rows:
        plot_comparison(residual_rows, output_dir)
    print(f"Saved plots in {output_dir}")



if __name__ == "__main__":
    main()
