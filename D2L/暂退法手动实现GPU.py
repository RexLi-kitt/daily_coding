import torch
from torch import nn
from d2l import torch as d2l
from torch.utils.data import DataLoader
import torchvision
import torchvision.transforms as transforms

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

def dropout_layer(X, dropout):
    assert 0 <= dropout <= 1
    # 在本情况中，所有元素都被丢弃
    if dropout == 1:
        return torch.zeros_like(X)
    # 在本情况中，所有元素都被保留
    if dropout == 0:
        return X
    # 必须在 X 所在的设备上生成随机掩码，否则 mask(CPU) 与 X(GPU) 无法相乘
    mask = (torch.rand(X.shape, device=X.device) > dropout).float()
    return mask * X / (1.0 - dropout)

num_inputs, num_outputs, num_hiddens1, num_hiddens2 = 784, 10, 256, 256

dropout1, dropout2 = 0.2, 0.5

class Net(nn.Module):
    def __init__(self, num_inputs, num_outputs, num_hiddens1, num_hiddens2,
                 is_training = True):
        super(Net, self).__init__()
        self.num_inputs = num_inputs
        self.training = is_training
        self.lin1 = nn.Linear(num_inputs, num_hiddens1)
        self.lin2 = nn.Linear(num_hiddens1, num_hiddens2)
        self.lin3 = nn.Linear(num_hiddens2, num_outputs)
        self.relu = nn.ReLU()

    def forward(self, X):
        H1 = self.relu(self.lin1(X.reshape((-1, self.num_inputs))))
        # 只有在训练模型时才使用dropout
        if self.training == True:
            # 在第一个全连接层之后添加一个dropout层
            H1 = dropout_layer(H1, dropout1)
        H2 = self.relu(self.lin2(H1))
        if self.training == True:
            # 在第二个全连接层之后添加一个dropout层
            H2 = dropout_layer(H2, dropout2)
        out = self.lin3(H2)
        return out
    
net = Net(num_inputs, num_outputs, num_hiddens1, num_hiddens2)

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

def l2_penalty(w):
    return torch.sum(pow(w,2)) / 2

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

def train_ch3(net, train_iter, test_iter, loss, num_epochs, updater):  #@save

    for epoch in range(num_epochs):
        train_metrics = train_epoch_ch3(net, train_iter, loss, updater)
        test_acc = evaluate_accuracy(net, test_iter)
        print(f'epoch {epoch + 1}: loss={train_metrics[0]:.4f},'
              f'train acc = {train_metrics[1]:.4f}, test acc = {test_acc:.4f}')
    train_loss, train_acc = train_metrics

    assert train_loss < 0.5, train_loss
    assert train_acc <= 1 and train_acc > 0.7, train_acc
    assert test_acc <= 1 and test_acc > 0.7, test_acc

if __name__ == '__main__':
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