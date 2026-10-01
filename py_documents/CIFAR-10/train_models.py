"""Re-run one selected model using its original training implementation."""

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--model", required=True, choices=("resnet18", "cnn", "mlp"))
    parser.add_argument("--archive", type=Path, default=ROOT.parent / "cifar-10-python.tar.gz")
    parser.add_argument("--output-dir", type=Path, help="Default: new_runs/<model>; existing archive results are protected")
    parser.add_argument("--epochs", type=int)
    parser.add_argument("--batch-size", type=int)
    parser.add_argument("--lr", type=float)
    parser.add_argument("--seed", type=int, default=42)
    args = parser.parse_args()
    archive = args.archive.resolve()
    if not archive.is_file():
        parser.error(f"CIFAR-10 archive does not exist: {archive}")
    output = (args.output_dir or ROOT / "new_runs" / args.model).resolve()
    protected = [ROOT / name for name in ("results", "code", "figures")]
    if output == ROOT or any(output == path or path in output.parents for path in protected):
        parser.error("Choose a training output outside the archived code, results and figures.")
    if output.exists() and any(output.iterdir()):
        parser.error("Training output already contains files; choose a new --output-dir.")
    is_resnet = args.model == "resnet18"
    epochs = args.epochs if args.epochs is not None else 80
    batch_size = args.batch_size if args.batch_size is not None else (512 if is_resnet else 256)
    lr = args.lr if args.lr is not None else (0.05 if is_resnet else 0.1)
    if epochs <= 0 or batch_size <= 0 or lr <= 0:
        parser.error("Epochs, batch size and learning rate must be positive.")
    output.mkdir(parents=True, exist_ok=True)
    sys.path.insert(0, str(ROOT / "code"))
    common = ["--epochs", str(epochs), "--batch-size", str(batch_size),
              "--lr", str(lr), "--seed", str(args.seed),
              "--archive", str(archive), "--output-dir", str(output)]
    if is_resnet:
        from train_resnet_cifar10 import main as train
        train(["--models", "resnet18", *common])
    else:
        from train_mlp_cifar10 import main as train
        train(["--model", args.model, *common])


if __name__ == "__main__":
    main()
