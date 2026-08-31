# -*- coding: utf-8 -*-
"""D2L 5.4 自定义层 练习：设计一个接受输入并计算张量降维的层

返回:  y_k = Σ_{i,j} w_{k,i,j} * x_i * x_j     （二次型）
输入:  X 形状 (B, n) —— B 个样本，每个样本 n 个特征分量
输出:  (B, m) —— 每个样本的 m 个二次型值

三种等价写法（均全向量化、无 for 循环）：
  1) einsum        : torch.einsum('kij,bi,bj->bk', W, X, X)
  2) 展开逐元素乘  : ((X @ W) * X).sum(dim=1)            （单输出时）
  3) 外积写法      : bmm 得到每个样本的 x_i*x_j 矩阵，再与 W 逐元素乘求和

注意：X @ X.T 得到的是"样本间内积"的 Gram 矩阵，不是本公式需要的
"同一样本内特征乘积 x_i * x_j"，不要混淆。
"""

import torch
from torch import nn


class QuadraticForm(nn.Module):
    """y_k = Σ_{i,j} w_{k,i,j} * x_i * x_j，einsum 一行实现。"""

    def __init__(self, in_features, out_features=1):
        super().__init__()
        self.in_features = in_features
        self.out_features = out_features
        self.weight = nn.Parameter(
            torch.randn(out_features, in_features, in_features) * 0.1
        )

    def forward(self, X):
        return torch.einsum('kij,bi,bj->bk', self.weight, X, X)


class QuadLayer(nn.Module):
    """单输出二次型 y = x^T W x，展开写法（修正自手写版本）。"""

    def __init__(self, in_features):
        super().__init__()
        self.weight = nn.Parameter(torch.randn(in_features, in_features) * 0.1)

    def forward(self, X):
        return ((X @ self.weight) * X).sum(dim=1)


class QuadOuter(nn.Module):
    """外积（Gram 风格）写法：先算每个样本的 x_i*x_j 矩阵，再配权重求和。"""

    def __init__(self, in_features, out_features=1):
        super().__init__()
        self.weight = nn.Parameter(
            torch.randn(out_features, in_features, in_features) * 0.1
        )

    def forward(self, X):
        outer = torch.bmm(X.unsqueeze(2), X.unsqueeze(1))   # (B,n,1)@(B,1,n) -> (B,n,n)
        return torch.einsum('kij,bij->bk', self.weight, outer)


if __name__ == '__main__':
    torch.manual_seed(0)

    # 1) 三种写法与手写四重循环逐一对照
    n, m, B = 3, 2, 4
    layer = QuadraticForm(n, m)
    X = torch.randn(B, n)
    Y = layer(X)
    print('einsum 输出形状:', tuple(Y.shape))

    W = layer.weight.data
    ref = torch.zeros(B, m)
    for b in range(B):
        for k in range(m):
            s = 0.0
            for i in range(n):
                for j in range(n):
                    s += W[k, i, j].item() * X[b, i].item() * X[b, j].item()
            ref[b, k] = s
    print('einsum 与四重循环一致:', torch.allclose(Y, ref, atol=1e-5))

    outer = (X.unsqueeze(2) * X.unsqueeze(1)).reshape(B, -1)
    Y2 = outer @ layer.weight.reshape(m, -1).T
    print('展平矩阵乘写法一致:    ', torch.allclose(Y2, ref, atol=1e-5))

    qo = QuadOuter(n, m)
    with torch.no_grad():
        qo.weight.copy_(layer.weight.data)
    Y3 = qo(X)
    print('外积 bmm 写法一致:     ', torch.allclose(Y3, ref, atol=1e-5))

    # 2) 梯度回传
    Xg = X.clone().requires_grad_(True)
    layer(Xg).sum().backward()
    print('X.grad 形状:', tuple(Xg.grad.shape), ' W.grad 形状:', tuple(layer.weight.grad.shape))

    # 3) 训练拟合已知二次型 y = x0^2 + 2*x0*x1  （目标 W = [[1,1],[1,0]]）
    torch.manual_seed(1)
    net = QuadLayer(2)
    opt = torch.optim.SGD(net.parameters(), lr=0.1)
    xt = torch.randn(256, 2)
    yt = xt[:, 0] ** 2 + 2 * xt[:, 0] * xt[:, 1]   # 形状 (B,)，与 net(xt) 对齐
    for _ in range(200):
        loss = ((net(xt) - yt) ** 2).mean()
        opt.zero_grad()
        loss.backward()
        opt.step()
    print('训练 200 步后损失: %.3e' % loss.item())
    W_learned = 0.5 * (net.weight.data + net.weight.data.t())   # 反对称部分不可辨识，对称化
    print('学到的 W (对称化):\n', W_learned)

    # 4) 对比有 bug 的手写版：跨样本污染演示
    Wt = torch.eye(2)

    def buggy(X, W):
        return sum(torch.mm(W, X @ X.T))

    def correct(X, W):
        return ((X @ W) * X).sum(1)

    X1 = torch.tensor([[1., 2.], [3., 4.]])
    X2 = torch.tensor([[1., 2.], [10., 20.]])   # 样本0不变，只换样本1
    print('buggy 版 (X1):', buggy(X1, Wt).tolist(), '  correct 版 (X1):', correct(X1, Wt).tolist())
    print('buggy 版 (X2):', buggy(X2, Wt).tolist(), '  correct 版 (X2):', correct(X2, Wt).tolist())
    print('注：样本0的真实二次型恒为 5；buggy 版输出被邻居样本污染')
