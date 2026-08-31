import torch
from torch import nn
from d2l import torch as d2l
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
batch_size = 2048
train_iter, test_iter = d2l.load_data_fashion_mnist(batch_size)

num_inputs, num_outputs, num_hiddens = 784, 10, 512

W1 = nn.Parameter(torch.randn(num_inputs, num_hiddens, requires_grad=True, device=device) * 0.01)
b1 = nn.Parameter(torch.zeros(num_hiddens, requires_grad=True, device=device))
W2 = nn.Parameter(torch.randn(num_hiddens, num_outputs, requires_grad=True, device= device) * 0.01)
b2 = nn.Parameter(torch.zeros(num_outputs, requires_grad=True,device=device))

params = [W1, b1, W2, b2]

def relu(X):
    a = torch.zeros_like(X)
    return torch.max(X,a)

def net(X):
    X = X.reshape(-1,num_inputs)
    H = relu(X @ W1 + b1)
    return H @ W2 + b2

loss = nn.CrossEntropyLoss(reduction='none')

num_epochs, lr = 15, 0.1
updater = torch.optim.SGD([
    {'params': [W1, b1], 'lr': 0.5},    # 第一层 lr × 5
    {'params': [W2, b2], 'lr': 0.1},    # 第二层 lr 不变
])

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
        for X,y in data_iter:
            X = X.to(device)
            y = y.to(device)
            metric.add(accuracy(net(X),y),y.numel())

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
    train_ch3(net, train_iter, test_iter, loss, num_epochs, updater)


