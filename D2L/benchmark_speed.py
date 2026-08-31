"""验证 GPU 预加载 + TensorDataset 方案的速度"""
import time
import torch
from torch import nn
from d2l import torch as d2l
from torch.utils.data import DataLoader, TensorDataset
import torchvision
import torchvision.transforms as transforms


def main():
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"设备: {device}")

    net = nn.Sequential(nn.Flatten(),
                        nn.Linear(784, 256), nn.ReLU(),
                        nn.Linear(256, 512), nn.ReLU(),
                        nn.Linear(512, 512), nn.Tanh(),
                        nn.Linear(512, 256), nn.ReLU(),
                        nn.Linear(256, 10)).to(device)

    batch_size = 256
    loss = nn.CrossEntropyLoss(reduction='none')
    trainer = torch.optim.SGD(net.parameters(), lr=0.1)

    trans = transforms.ToTensor()
    mnist_train = torchvision.datasets.FashionMNIST(
        root="../data", train=True, transform=trans, download=True)
    mnist_test = torchvision.datasets.FashionMNIST(
        root="../data", train=False, transform=trans, download=True)

    # ===== 一次性把数据搬到 GPU =====
    t0 = time.time()
    train_X = torch.stack([img for img, _ in mnist_train]).to(device)
    train_y = torch.tensor([lab for _, lab in mnist_train]).to(device)
    test_X = torch.stack([img for img, _ in mnist_test]).to(device)
    test_y = torch.tensor([lab for _, lab in mnist_test]).to(device)
    torch.cuda.synchronize()
    print(f"预加载到GPU耗时: {time.time()-t0:.2f} 秒, "
          f"train_X: {tuple(train_X.shape)}, test_X: {tuple(test_X.shape)}")

    # 数据已常驻 GPU，DataLoader 直接切片
    train_iter = DataLoader(TensorDataset(train_X, train_y),
                            batch_size=batch_size, shuffle=True, num_workers=0)
    test_iter = DataLoader(TensorDataset(test_X, test_y),
                           batch_size=batch_size, shuffle=False, num_workers=0)

    # ===== 完整训练 1 个 epoch（复用 d2l 的训练函数） =====
    def train_epoch_ch3(net, train_iter, loss, updater):
        if isinstance(net, torch.nn.Module):
            net.train()
        metric = d2l.Accumulator(3)
        for X, y in train_iter:
            X, y = X.to(device), y.to(device)
            y_hat = net(X)
            l = loss(y_hat, y)
            if isinstance(updater, torch.optim.Optimizer):
                updater.zero_grad()
                l.mean().backward()
                updater.step()
            metric.add(float(l.sum()), 0, y.numel())
        return metric[0] / metric[2]

    # 预热
    train_epoch_ch3(net, train_iter, loss, trainer)
    torch.cuda.synchronize()

    t0 = time.time()
    tl = train_epoch_ch3(net, train_iter, loss, trainer)
    torch.cuda.synchronize()
    print(f"完整 epoch(预加载GPU): {time.time()-t0:.2f} 秒, loss={tl:.4f}")

    # 测试集同样
    def eval_acc(net, data_iter):
        net.eval()
        metric = d2l.Accumulator(2)
        with torch.no_grad():
            for X, y in data_iter:
                X, y = X.to(device), y.to(device)
                cmp = (net(X).argmax(axis=1).type(y.dtype) == y)
                metric.add(float(cmp.type(y.dtype).sum()), y.numel())
        return metric[0] / metric[1]

    t0 = time.time()
    acc = eval_acc(net, test_iter)
    print(f"测试集评估: {time.time()-t0:.2f} 秒, acc={acc:.4f}")


if __name__ == '__main__':
    main()
