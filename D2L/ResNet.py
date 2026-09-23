import torch
from torch import nn
from torch.nn import functional as F
from d2l import torch as d2l
from matplotlib import pyplot as plt
from pathlib import Path


class Residual(nn.Module):  #@save
    def __init__(self, input_channels, num_channels,
                 use_1x1conv=False, strides=1):
        super().__init__()
        self.conv1 = nn.Conv2d(input_channels, num_channels,
                               kernel_size=3, padding=1, stride=strides)
        self.conv2 = nn.Conv2d(num_channels, num_channels,
                               kernel_size=3, padding=1)
        if use_1x1conv:
            self.conv3 = nn.Conv2d(input_channels, num_channels,
                                   kernel_size=1, stride=strides)
        else:
            self.conv3 = None
        self.bn1 = nn.BatchNorm2d(num_channels)
        self.bn2 = nn.BatchNorm2d(num_channels)

    def forward(self, X):
        Y = F.relu(self.bn1(self.conv1(X)))
        Y = self.bn2(self.conv2(Y))
        if self.conv3:
            X = self.conv3(X)
        Y += X
        return F.relu(Y)

b1 = nn.Sequential(nn.Conv2d(1, 64, kernel_size=7, stride=2, padding=3),
                   nn.BatchNorm2d(64), nn.ReLU(),
                   nn.MaxPool2d(kernel_size=3, stride=2, padding=1))

def resnet_block(input_channels, num_channels, num_residuals,
                 first_block=False):
    blk = []
    for i in range(num_residuals):
        if i == 0 and not first_block:
            blk.append(Residual(input_channels, num_channels,
                                use_1x1conv=True, strides=2))
        else:
            blk.append(Residual(num_channels, num_channels))
    return blk

b2 = nn.Sequential(*resnet_block(64, 64, 2, first_block=True))
b3 = nn.Sequential(*resnet_block(64, 128, 2))
b4 = nn.Sequential(*resnet_block(128, 256, 2))
b5 = nn.Sequential(*resnet_block(256, 512, 2))

net = nn.Sequential(b1, b2, b3, b4, b5,
                    nn.AdaptiveAvgPool2d((1,1)),
                    nn.Flatten(), 
                    nn.Dropout(0.2),
                    nn.Linear(512, 10))


def train_ch6(net, train_iter, test_iter, num_epochs, lr, device,
              image_path=None):
    """训练网络，并将每轮的损失和准确率保存为图片。"""
    if image_path is None:
        image_path = Path(__file__).with_name('ResNet_训练曲线.png')
    image_path = Path(image_path)

    def init_weights(m):
        if isinstance(m, (nn.Linear, nn.Conv2d)):
            nn.init.xavier_uniform_(m.weight)

    net.apply(init_weights)
    net.to(device)
    optimizer = torch.optim.SGD(net.parameters(), lr=lr,weight_decay=1e-5)
    loss = nn.CrossEntropyLoss()
    train_losses, train_accs, test_accs = [], [], []
    print(f'training on {device}')

    for epoch in range(num_epochs):
        net.train()
        loss_sum = correct = total = 0
        for X, y in train_iter:
            X, y = X.to(device), y.to(device)
            optimizer.zero_grad()
            y_hat = net(X)
            l = loss(y_hat, y)
            l.backward()
            optimizer.step()
            loss_sum += l.item() * y.numel()
            correct += (y_hat.argmax(dim=1) == y).sum().item()
            total += y.numel()

        train_losses.append(loss_sum / total)
        train_accs.append(correct / total)
        net.eval()
        correct = total_test = 0
        with torch.no_grad():
            for X, y in test_iter:
                X, y = X.to(device), y.to(device)
                correct += (net(X).argmax(dim=1) == y).sum().item()
                total_test += y.numel()
        test_accs.append(correct / total_test)
        print(f'epoch {epoch + 1}: loss {train_losses[-1]:.3f}, '
              f'train acc {train_accs[-1]:.3f}, test acc {test_accs[-1]:.3f}')

    epochs = range(1, num_epochs + 1)
    fig, ax = plt.subplots(figsize=(7, 5))
    ax.plot(epochs, train_losses, label='train loss')
    ax.plot(epochs, train_accs, label='train acc')
    ax.plot(epochs, test_accs, label='test acc')
    ax.set(xlabel='epoch', xlim=(1, max(2, num_epochs)))
    ax.legend()
    fig.tight_layout()
    fig.savefig(image_path)
    print(f'训练曲线已保存到 {image_path}')
    plt.show()


if __name__ == '__main__':
    lr, num_epochs, batch_size = 0.05, 15, 256
    train_iter, test_iter = d2l.load_data_fashion_mnist(batch_size, resize=96)
    train_ch6(net, train_iter, test_iter, num_epochs, lr, d2l.try_gpu())
