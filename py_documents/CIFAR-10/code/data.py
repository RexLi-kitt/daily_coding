"""CIFAR-10 loading and augmentation; no models, training or plotting."""


import pickle
import tarfile
from pathlib import Path

import numpy as np
import torch
from torch.nn import functional as F


def load_cifar10(archive_path: Path, device: torch.device):
    """Read the official CIFAR-10 Python batches without extracting files."""
    batches = []
    with tarfile.open(archive_path, "r:gz") as archive:
        for name in [*(f"data_batch_{i}" for i in range(1, 6)), "test_batch"]:
            member = archive.extractfile(f"cifar-10-batches-py/{name}")
            if member is None:
                raise FileNotFoundError(name)
            batch = pickle.load(member, encoding="bytes")
            images = batch[b"data"].reshape(-1, 3, 32, 32)
            labels = np.asarray(batch[b"labels"], dtype=np.int64)
            batches.append((images, labels))

    train_x = np.concatenate([images for images, _ in batches[:5]])
    train_y = np.concatenate([labels for _, labels in batches[:5]])
    test_x, test_y = batches[5]
    # Match the ToTensor() preprocessing in the reference D2L MLP.
    to_images = lambda x: torch.from_numpy(x).to(device=device, dtype=torch.float32).div_(255)
    to_labels = lambda y: torch.from_numpy(y).to(device=device)
    return to_images(train_x), to_labels(train_y), to_images(test_x), to_labels(test_y)


def augment_batch(images, mean, std):
    """Per-image random 32x32 crop from 4-pixel padding, then random flip."""
    batch_size = images.shape[0]
    device = images.device
    padded = F.pad(images, (4, 4, 4, 4))
    top = torch.randint(0, 9, (batch_size, 1, 1), device=device)
    left = torch.randint(0, 9, (batch_size, 1, 1), device=device)
    offset = torch.arange(32, device=device)
    spatial_indices = ((top + offset[None, :, None]) * 40 +
                       (left + offset[None, None, :])).reshape(batch_size, 1, 1024)
    cropped = padded.flatten(2).gather(2, spatial_indices.expand(-1, 3, -1))
    cropped = cropped.reshape(batch_size, 3, 32, 32)
    flip = torch.rand(batch_size, device=device) < 0.5
    cropped = torch.where(flip[:, None, None, None], cropped.flip(-1), cropped)
    return ((cropped - mean) / std).contiguous(memory_format=torch.channels_last)
