"""Model definitions and initialization; no data loading, training or plotting."""


import torch
from torch import nn
from torch.nn import functional as F


STAGES = {"resnet18": (2, 2, 2, 2), "resnet26": (2, 2, 2, 2), "plain50": (3, 4, 6, 3), "resnet50": (3, 4, 6, 3),
          "resnet152": (3, 8, 36, 3)}


MODEL_LABELS = {"resnet18": "ResNet-18 (BasicBlock)", "resnet26": "ResNet-26 (Bottleneck)",
                "plain50": "Plain-50 (no shortcuts)",
                "resnet50": "ResNet-50", "resnet152": "ResNet-152"}


class BasicBlock(nn.Module):
    expansion = 1

    def __init__(self, in_channels: int, channels: int, stride: int = 1):
        super().__init__()
        self.conv1 = nn.Conv2d(in_channels, channels, 3, stride=stride,
                               padding=1, bias=False)
        self.bn1 = nn.BatchNorm2d(channels)
        self.conv2 = nn.Conv2d(channels, channels, 3, padding=1, bias=False)
        self.bn2 = nn.BatchNorm2d(channels)
        self.shortcut = (
            nn.Sequential(
                nn.Conv2d(in_channels, channels, 1, stride=stride, bias=False),
                nn.BatchNorm2d(channels),
            )
            if stride != 1 or in_channels != channels else nn.Identity()
        )

    def forward(self, x):
        identity = self.shortcut(x)
        out = F.relu(self.bn1(self.conv1(x)), inplace=True)
        out = self.bn2(self.conv2(out))
        return F.relu(out + identity, inplace=True)


class Bottleneck(nn.Module):
    expansion = 4

    def __init__(self, in_channels: int, channels: int, stride: int = 1):
        super().__init__()
        out_channels = channels * self.expansion
        # ResNet v1: downsample in the first 1x1 convolution of each stage.
        self.conv1 = nn.Conv2d(in_channels, channels, 1, stride=stride, bias=False)
        self.bn1 = nn.BatchNorm2d(channels)
        self.conv2 = nn.Conv2d(channels, channels, 3, padding=1, bias=False)
        self.bn2 = nn.BatchNorm2d(channels)
        self.conv3 = nn.Conv2d(channels, out_channels, 1, bias=False)
        self.bn3 = nn.BatchNorm2d(out_channels)
        self.shortcut = (
            nn.Sequential(
                nn.Conv2d(in_channels, out_channels, 1, stride=stride, bias=False),
                nn.BatchNorm2d(out_channels),
            )
            if stride != 1 or in_channels != out_channels else nn.Identity()
        )

    def forward(self, x):
        identity = self.shortcut(x)
        out = F.relu(self.bn1(self.conv1(x)), inplace=True)
        out = F.relu(self.bn2(self.conv2(out)), inplace=True)
        out = self.bn3(self.conv3(out))
        return F.relu(out + identity, inplace=True)


class PlainBottleneck(nn.Module):
    """The same three-convolution branch as Bottleneck, without a shortcut."""

    expansion = 4

    def __init__(self, in_channels: int, channels: int, stride: int = 1):
        super().__init__()
        out_channels = channels * self.expansion
        self.conv1 = nn.Conv2d(in_channels, channels, 1, stride=stride, bias=False)
        self.bn1 = nn.BatchNorm2d(channels)
        self.conv2 = nn.Conv2d(channels, channels, 3, padding=1, bias=False)
        self.bn2 = nn.BatchNorm2d(channels)
        self.conv3 = nn.Conv2d(channels, out_channels, 1, bias=False)
        self.bn3 = nn.BatchNorm2d(out_channels)

    def forward(self, x):
        out = F.relu(self.bn1(self.conv1(x)), inplace=True)
        out = F.relu(self.bn2(self.conv2(out)), inplace=True)
        out = self.bn3(self.conv3(out))
        return F.relu(out, inplace=True)


class ResNet(nn.Module):
    def __init__(self, blocks: tuple[int, int, int, int], num_classes: int = 10,
                 block=Bottleneck):
        super().__init__()
        self.stem = nn.Sequential(
            nn.Conv2d(3, 64, 7, stride=2, padding=3, bias=False),
            nn.BatchNorm2d(64), nn.ReLU(inplace=True),
            nn.MaxPool2d(3, stride=2, padding=1),
        )
        in_channels = 64
        stages = []
        for stage_index, (channels, count) in enumerate(zip((64, 128, 256, 512), blocks)):
            stride = 1 if stage_index == 0 else 2
            stage = [block(in_channels, channels, stride)]
            in_channels = channels * block.expansion
            stage.extend(block(in_channels, channels) for _ in range(count - 1))
            stages.append(nn.Sequential(*stage))
        self.stages = nn.Sequential(*stages)
        self.avgpool = nn.AdaptiveAvgPool2d(1)
        self.fc = nn.Linear(512 * block.expansion, num_classes)
        self.apply(self._init_weights)

    @staticmethod
    def _init_weights(layer):
        if isinstance(layer, nn.Conv2d):
            nn.init.kaiming_normal_(layer.weight, mode="fan_out", nonlinearity="relu")
        elif isinstance(layer, nn.BatchNorm2d):
            nn.init.ones_(layer.weight)
            nn.init.zeros_(layer.bias)

    def forward(self, x):
        x = self.stem(x)
        x = self.stages(x)
        x = self.avgpool(x).flatten(1)
        return self.fc(x)


def make_mlp() -> nn.Module:
    # Same hidden layers and activations as 多层感知机简洁实现.py; CIFAR input is 3*32*32.
    return nn.Sequential(
        nn.Flatten(),
        nn.Linear(3 * 32 * 32, 256), nn.ReLU(),
        nn.Linear(256, 512), nn.ReLU(),
        nn.Linear(512, 512), nn.Tanh(),
        nn.Linear(512, 256), nn.ReLU(),
        nn.Linear(256, 10),
    )


def make_cnn() -> nn.Module:
    # 卷积神经网络.py: same layer order; adapt 1->3 input channels and 5*5->6*6 features.
    model = nn.Sequential(
        nn.Conv2d(3, 6, kernel_size=5, padding=2), nn.ReLU(),
        nn.AvgPool2d(kernel_size=2, stride=2),
        nn.Conv2d(6, 16, kernel_size=5), nn.ReLU(),
        nn.AvgPool2d(kernel_size=2, stride=2),
        nn.Flatten(),
        nn.Linear(16 * 6 * 6, 120), nn.ReLU(),
        nn.Linear(120, 84), nn.ReLU(),
        nn.Linear(84, 10),
    )
    # Match the Xavier initialization used by the reference CNN trainer.
    for layer in model.modules():
        if isinstance(layer, (nn.Conv2d, nn.Linear)):
            nn.init.xavier_uniform_(layer.weight)
    return model
