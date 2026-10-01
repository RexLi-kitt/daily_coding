"""Legacy ResNet training entry point.

Use train_resnet_cifar10.py for training and plot_training_curves.py for plots.
This compatibility entry point contains no plotting implementation.
"""

from models import STAGES, MODEL_LABELS, BasicBlock, Bottleneck, PlainBottleneck, ResNet
from data import load_cifar10, augment_batch
from train_resnet_cifar10 import ARCHIVE, ROOT, PAPER_URL, evaluate, write_metrics, train_one, main


if __name__ == "__main__":
    main()
