import matplotlib
matplotlib.use('TkAgg')   # 必须写在 import d2l 之前，否则无效
import torch
from torch import nn
from d2l import torch as d2l
from torch.utils.data import DataLoader
import torchvision
import torchvision.transforms as transforms

# 阻止 d2l 内部把后端改成 SVG（那是给 Jupyter 用的），否则 TkAgg 会被覆盖导致无法弹窗
d2l.use_svg_display = lambda: None

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")


def init_weights(m):
    if type(m) == nn.Linear:
        nn.init.normal_(m.weight, std=0.01)

def accuracy(y_hat, y):  #@save
    if len(y_hat.shape) > 1 and y_hat.shape[1] > 1:
        y_hat = y_hat.argmax(axis=1)
    cmp = y_hat.type(y.dtype) == y
    return float(cmp.type(y.dtype).sum())

def evaluate_accuracy(net, data_iter):
    if isinstance(net, torch.nn.Module):
        net.eval()
    metric = d2l.Accumulator(2) 
    with torch.no_grad():
        for X, y in data_iter:
            X, y = X.to(device), y.to(device)
            metric.add(accuracy(net(X), y), y.numel())

    return metric[0] / metric[1]

def train_epoch_ch3(net, train_iter, loss, updater): 
    # 将模型设置为训练模式
    if isinstance(net, torch.nn.Module):
        net.train()
    # 训练损失总和、训练准确度总和、样本数
    metric = d2l.Accumulator(3)
    for X, y in train_iter:
        X, y = X.to(device), y.to(device)
        y_hat = net(X)
        l = loss(y_hat, y)
        if isinstance(updater, torch.optim.Optimizer):
            updater.zero_grad()
            l.mean().backward()
            updater.step()
        else:
            l.sum().backward()
            updater(X.shape[0])
        metric.add(float(l.sum()), accuracy(y_hat, y), y.numel())
    return metric[0] / metric[2], metric[1] / metric[2]

def get_hidden_variance(net, test_iter):
    """eval 模式下计算隐藏层1和隐藏层2激活值的方差"""
    net.eval()                          # 幂等，确保 eval 模式（Dropout 关闭）
    X, _ = next(iter(test_iter))       # 取一个 batch
    X = X.to(device)
    with torch.no_grad():
        x = net[0](X)                  # Flatten → (batch, 784)
        h1 = net[2](net[1](x))         # 隐藏层1激活值 (ReLU输出) → (batch, 256)
        h2 = net[5](net[4](net[3](h1)))# 隐藏层2激活值 (ReLU输出) → (batch, 256)
        #h2 = net[4](net[3](h1))
    net.train()                         # 恢复训练模式（下一个 epoch 需要）
    return h1.var().item(), h2.var().item()

def train_ch3(net, train_iter, test_iter, loss, num_epochs, updater):  #@save
    animator = d2l.Animator(xlabel='epoch', xlim=[1, num_epochs], ylim=[0, 1],
                            legend=['train loss', 'train acc', 'test acc'])
    var_animator = d2l.Animator(xlabel='epoch', xlim=[1, num_epochs],
                                legend=['hidden1 var', 'hidden2 var'])
    for epoch in range(num_epochs):
        train_metrics = train_epoch_ch3(net, train_iter, loss, updater)
        test_acc = evaluate_accuracy(net, test_iter)
        v1, v2 = get_hidden_variance(net, test_iter)

        animator.add(epoch + 1, train_metrics + (test_acc,))
        var_animator.add(epoch + 1, (v1, v2))
    train_loss, train_acc = train_metrics

    assert train_loss < 0.5, train_loss
    assert train_acc <= 1 and train_acc > 0.7, train_acc
    assert test_acc <= 1 and test_acc > 0.7, test_acc
    d2l.plt.show()

if __name__ == '__main__':
    dropout1 = 0.3
    dropout2 = 0.5

    net = nn.Sequential(nn.Flatten(),
                    nn.Linear(784, 256),
                    nn.ReLU(),
                    nn.Dropout(dropout1),
                    nn.Linear(256, 256),
                    nn.ReLU(),
                    nn.Dropout(dropout2),
                    nn.Linear(256, 10))
    net.apply(init_weights)

    num_epochs, lr, batch_size = 25, 0.3, 256
    loss = nn.CrossEntropyLoss(reduction='none')

    # ===== 把模型搬到 GPU =====
    net.to(device)

    trans = transforms.ToTensor()

    mnist_train = torchvision.datasets.FashionMNIST(
        root="../data", train=True, transform=trans, download=True)
    mnist_test = torchvision.datasets.FashionMNIST(
        root="../data", train=False, transform=trans, download=True)

    # ===== 一次性把全部数据搬到 GPU（只需 ~170MB 显存） =====
    train_X = torch.stack([img for img, _ in mnist_train]).to(device)
    train_y = torch.tensor([lab for _, lab in mnist_train]).to(device)
    test_X = torch.stack([img for img, _ in mnist_test]).to(device)
    test_y = torch.tensor([lab for _, lab in mnist_test]).to(device)

    # 数据常驻 GPU，DataLoader 只做 GPU 内切片，无解码、无搬运
    train_iter = DataLoader(torch.utils.data.TensorDataset(train_X, train_y),
                            batch_size=batch_size, shuffle=True, num_workers=0)
    test_iter = DataLoader(torch.utils.data.TensorDataset(test_X, test_y),
                           batch_size=batch_size, shuffle=False, num_workers=0)

    trainer = torch.optim.SGD(net.parameters(), lr=lr, weight_decay=0.001)
    train_ch3(net, train_iter, test_iter, loss, num_epochs, trainer)
