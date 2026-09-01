import math
from pathlib import Path
import torch
from torch import nn
from torch.nn import functional as F
from d2l import torch as d2l
from 语言模型和数据集 import load_data_time_machine
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

IMAGE_DIR = Path(__file__).resolve().parent / '图片'
IMAGE_DIR.mkdir(exist_ok=True)

batch_size, num_steps = 32, 35
train_iter, vocab = load_data_time_machine(batch_size, num_steps)

num_hiddens = 256
rnn_layer = nn.RNN(len(vocab),num_hiddens)

state = torch.zeros((1,batch_size,num_hiddens))

X = torch.rand(size=(num_steps, batch_size, len(vocab)))
Y, state_new = rnn_layer(X, state)

class RNNModel(nn.Module):
    def __init__(self, rnn_layer, vocab_size, **kwargs):
        super(RNNModel, self).__init__(**kwargs)
        self.rnn = rnn_layer
        self.vocab_size = vocab_size
        self.num_hiddens = self.rnn.hidden_size
        # 如果RNN是双向的（之后将介绍），num_directions应该是2，否则应该是1
        if not self.rnn.bidirectional:
            self.num_directions = 1
            self.linear = nn.Linear(self.num_hiddens, self.vocab_size)
        else:
            self.num_directions = 2
            self.linear = nn.Linear(self.num_hiddens * 2, self.vocab_size)

    def forward(self, inputs, state):
        X = F.one_hot(inputs.T.long(), self.vocab_size)
        X = X.to(torch.float32)
        Y, state = self.rnn(X, state)
        # 全连接层首先将Y的形状改为(时间步数*批量大小,隐藏单元数)
        # 它的输出形状是(时间步数*批量大小,词表大小)。
        output = self.linear(Y.reshape((-1, Y.shape[-1])))
        return output, state

    def begin_state(self, device, batch_size=1):
        if not isinstance(self.rnn, nn.LSTM):
            # nn.GRU以张量作为隐状态
            return  torch.zeros((self.num_directions * self.rnn.num_layers,
                                 batch_size, self.num_hiddens),
                                device=device)
        else:
            # nn.LSTM以元组作为隐状态
            return (torch.zeros((
                self.num_directions * self.rnn.num_layers,
                batch_size, self.num_hiddens), device=device),
                    torch.zeros((
                        self.num_directions * self.rnn.num_layers,
                        batch_size, self.num_hiddens), device=device))


def grad_clipping(net, theta=1):
    """限制梯度范数，避免 RNN 训练时梯度爆炸。"""
    params = [p for p in net.parameters() if p.requires_grad and p.grad is not None]
    if not params:
        return

    squared_norm = torch.zeros((), device=next(net.parameters()).device)
    for param in params:
        # 前面的筛选已经排除了 grad=None 的参数，这个断言也能帮助类型检查器确认这一点。
        assert param.grad is not None
        squared_norm = squared_norm + torch.sum(param.grad ** 2)
    norm = torch.sqrt(squared_norm)
    if norm > theta:
        for param in params:
            assert param.grad is not None
            param.grad.mul_(theta / norm)


def train_ch8(net, train_iter, vocab, lr, num_epochs, device):
    """训练字符级 RNN，并返回每个 epoch 的困惑度。"""
    loss = nn.CrossEntropyLoss()
    optimizer = torch.optim.SGD(net.parameters(), lr=lr)
    perplexities = []

    net.train()
    for epoch in range(num_epochs):
        state = None
        total_loss = 0.0
        total_tokens = 0

        for X, Y in train_iter:
            if state is None:
                state = net.begin_state(device=device, batch_size=X.shape[0])
            elif isinstance(state, tuple):
                state = tuple(s.detach() for s in state)
            else:
                state = state.detach()

            X, Y = X.to(device), Y.to(device)
            y = Y.T.reshape(-1).long()

            optimizer.zero_grad()
            y_hat, state = net(X, state)
            current_loss = loss(y_hat, y)
            current_loss.backward()
            grad_clipping(net)
            optimizer.step()

            total_loss += current_loss.item() * y.numel()
            total_tokens += y.numel()

        perplexity = math.exp(total_loss / total_tokens)
        perplexities.append(perplexity)

        if (epoch + 1) % 10 == 0 or epoch == 0:
            print(f'epoch {epoch + 1:>4}, perplexity {perplexity:.3f}')

    epochs = range(1, num_epochs + 1)
    plt.figure(figsize=(8, 5))
    plt.plot(epochs, perplexities, label='train perplexity')
    plt.xlabel('Epoch')
    plt.ylabel('Perplexity')
    plt.title('Epoch-Perplexity Curve')
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plot_path = IMAGE_DIR / 'epoch_perplexity.png'
    plt.savefig(plot_path, dpi=150)
    plt.close()
    print(f'曲线图已保存到 {plot_path}')
    return perplexities


device = d2l.try_gpu()
net = RNNModel(rnn_layer, vocab_size=len(vocab))
net = net.to(device)
num_epochs, lr = 500, 1
train_ch8(net, train_iter, vocab, lr, num_epochs, device)
