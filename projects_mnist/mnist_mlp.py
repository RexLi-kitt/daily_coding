"""Train an MLP on the local MNIST IDX files, or classify a digit image.

Examples (from this directory):
    python mnist_mlp.py train
    python mnist_mlp.py predict --image digit.png --invert
    python mnist_mlp.py predict --test-index 42
"""

import argparse
import gzip
import struct
from pathlib import Path

import torch
from torch import nn
from torch.utils.data import DataLoader, TensorDataset


DATA_DIR = Path(__file__).resolve().parent
MODEL_PATH = DATA_DIR / "mnist_mlp.pt"


def make_model():
    # Same architecture as D2L/多层感知机简洁实现.py.
    return nn.Sequential(
        nn.Flatten(),
        nn.Linear(784, 256),
        nn.ReLU(),
        nn.Linear(256, 512),
        nn.ReLU(),
        nn.Linear(512, 512),
        nn.Tanh(),
        nn.Linear(512, 256),
        nn.ReLU(),
        nn.Linear(256, 10),
    )


def read_images(path):
    with gzip.open(path, "rb") as file:
        header = file.read(16)
        if len(header) != 16:
            raise ValueError(f"Invalid MNIST image header: {path}")
        magic, count, rows, cols = struct.unpack(">IIII", header)
        if magic != 2051 or (rows, cols) != (28, 28):
            raise ValueError(f"Expected 28x28 MNIST images: {path}")
        pixels = file.read()
    if len(pixels) != count * rows * cols:
        raise ValueError(f"Incorrect image data length: {path}")
    return torch.frombuffer(bytearray(pixels), dtype=torch.uint8).reshape(count, 1, rows, cols)


def read_labels(path):
    with gzip.open(path, "rb") as file:
        header = file.read(8)
        if len(header) != 8:
            raise ValueError(f"Invalid MNIST label header: {path}")
        magic, count = struct.unpack(">II", header)
        if magic != 2049:
            raise ValueError(f"Invalid MNIST label file: {path}")
        labels = file.read()
    if len(labels) != count:
        raise ValueError(f"Incorrect label data length: {path}")
    return torch.frombuffer(bytearray(labels), dtype=torch.uint8).long()


def load_split(prefix):
    images = read_images(DATA_DIR / f"{prefix}-images-idx3-ubyte.gz")
    labels = read_labels(DATA_DIR / f"{prefix}-labels-idx1-ubyte.gz")
    if len(images) != len(labels):
        raise ValueError(f"Image/label counts differ in {prefix} split")
    return images, labels


def evaluate(model, loader, device):
    model.eval()
    correct = total = 0
    with torch.inference_mode():
        for images, labels in loader:
            images = images.to(device, non_blocking=True).float().div_(255)
            labels = labels.to(device, non_blocking=True)
            correct += (model(images).argmax(dim=1) == labels).sum().item()
            total += labels.numel()
    return correct / total


def train(args, device):
    train_images, train_labels = load_split("train")
    test_images, test_labels = load_split("t10k")
    pin = device.type == "cuda"
    train_loader = DataLoader(
        TensorDataset(train_images, train_labels),
        batch_size=args.batch_size,
        shuffle=True,
        pin_memory=pin,
    )
    test_loader = DataLoader(
        TensorDataset(test_images, test_labels),
        batch_size=args.batch_size,
        pin_memory=pin,
    )

    model = make_model().to(device)
    loss_fn = nn.CrossEntropyLoss()
    optimizer = torch.optim.SGD(model.parameters(), lr=args.lr)

    for epoch in range(1, args.epochs + 1):
        model.train()
        loss_sum = correct = total = 0
        for images, labels in train_loader:
            images = images.to(device, non_blocking=True).float().div_(255)
            labels = labels.to(device, non_blocking=True)
            logits = model(images)
            loss = loss_fn(logits, labels)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            count = labels.numel()
            loss_sum += loss.item() * count
            correct += (logits.argmax(dim=1) == labels).sum().item()
            total += count
        test_acc = evaluate(model, test_loader, device)
        print(
            f"Epoch {epoch:02d}/{args.epochs}: "
            f"loss={loss_sum / total:.4f}, "
            f"train_acc={correct / total:.4%}, test_acc={test_acc:.4%}",
            flush=True,
        )

    torch.save(model.cpu().state_dict(), args.model)
    print(f"Model saved to {args.model}")


def predict(args, device):
    model = make_model().to(device)
    model.load_state_dict(torch.load(args.model, map_location=device, weights_only=True))
    model.eval()

    if args.test_index is not None:
        images, labels = load_split("t10k")
        if not 0 <= args.test_index < len(images):
            raise ValueError(f"Test index must be between 0 and {len(images) - 1}")
        image = images[args.test_index]
        print(f"True label: {labels[args.test_index].item()}")
    else:
        try:
            from PIL import Image, ImageOps
        except ImportError as exc:
            raise SystemExit("Install Pillow to classify image files: pip install pillow") from exc
        with Image.open(args.image) as source:
            image = ImageOps.grayscale(source).resize((28, 28))
            if args.invert:
                image = ImageOps.invert(image)
            image = torch.tensor(bytearray(image.tobytes()), dtype=torch.uint8).reshape(1, 28, 28)

    with torch.inference_mode():
        logits = model(image.unsqueeze(0).to(device).float().div_(255))
        probabilities = logits.softmax(dim=1)[0]
        digit = probabilities.argmax().item()
    print(f"Predicted digit: {digit} (confidence {probabilities[digit].item():.2%})")


def main():
    parser = argparse.ArgumentParser(description="MNIST MLP training and digit recognition")
    parser.add_argument("--device", choices=("cuda", "cpu"), default="cuda")
    parser.add_argument("--model", type=Path, default=MODEL_PATH)
    subparsers = parser.add_subparsers(dest="command", required=True)

    training = subparsers.add_parser("train")
    training.add_argument("--epochs", type=int, default=20)
    training.add_argument("--batch-size", type=int, default=256)
    training.add_argument("--lr", type=float, default=0.1)

    prediction = subparsers.add_parser("predict")
    source = prediction.add_mutually_exclusive_group(required=True)
    source.add_argument("--image", type=Path)
    source.add_argument("--test-index", type=int)
    prediction.add_argument("--invert", action="store_true", help="For dark digits on a light background")

    args = parser.parse_args()
    if args.device == "cuda" and not torch.cuda.is_available():
        parser.error("CUDA is unavailable in this PyTorch environment; install a CUDA build or use --device cpu")
    device = torch.device(args.device)
    print(f"Using {torch.cuda.get_device_name(0) if device.type == 'cuda' else 'CPU'}")
    if args.command == "train":
        train(args, device)
    else:
        predict(args, device)


if __name__ == "__main__":
    main()
