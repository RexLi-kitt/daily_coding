import torch
import numpy as np
from torch.utils import data
from torch import nn

def synthetic_data(w, b, num_examples):
    """按真实参数 w、b 生成模拟数据集（用于测试）。"""
    X = torch.normal(0,1,(num_examples,len(w)))   # 特征：num_examples 个样本，每个 len(w) 个特征（正态分布）
    y = torch.matmul(X, w) + b                    # 真实标签：y = X·w + b（不含噪声）
    y += torch.normal(0,0.01, y.shape)            # 加一点随机噪声，模拟现实数据的误差
    return X,y.reshape(-1,1)                      # 标签整理成列向量形状 (样本数,1)

true_w = torch.tensor([2,-3.4])   # 真实的权重（用来生成数据，训练目标是逼近它）
true_b = 3.0                      # 真实的偏置

features,labels = synthetic_data(true_w,true_b,1000)   # features 特征(1000,2)，labels 标签(1000,1)

def load_array(data_arrays, batch_size, is_Train = True):
    """把特征和标签打包成可分批、可打乱的数据迭代器。"""
    dataset = data.TensorDataset(*data_arrays)      # 把 (features, labels) 配对成一个数据集
    return data.DataLoader(dataset, batch_size, shuffle=is_Train)   # 按 batch_size 分批，训练时打乱

batch_size = 10
data_iter = load_array((features,labels), batch_size)   # 每个批次 10 个样本的迭代器

net = nn.Sequential(nn.Linear(2,1))   # 定义模型：Linear(输入2, 输出1)，即 y = w1*x1 + w2*x2 + b
                                      # net[0] = 里面那层 Linear；net(X) 走完整模型做预测

net[0].weight.data.normal_(0,0.01)   # 初始化权重：正态分布 均值0、标准差0.01（很小的随机值）
net[0].bias.data.fill_(0)            # 初始化偏置：全部填 0

loss = nn.HuberLoss()   # 损失函数：均方误差，衡量"预测值"和"真实值"的差距
trainer = torch.optim.SGD(net.parameters(),lr = 0.03)   # SGD 优化器：负责更新参数，学习率 0.03

num_epochs = 10
for i in range(num_epochs):          # 外层：跑 10 轮完整数据集
    for X,y in data_iter:            # 内层：每次取一批样本 (X 特征, y 真实标签)
        l = loss(net(X),y)           # net(X)=预测值ŷ，l = 预测与真实的差距
        trainer.zero_grad()          # 清空上一步的梯度（避免累加）
        l.backward()                 # 反向传播：算出每个参数该往哪调、调多少（存进 .grad）
        trainer.step()               # 真正更新参数：参数 = 参数 - lr×梯度，走一步
    l = loss(net(features),labels)   # 一个 epoch 结束后，用全部数据算一次总损失
    print(f'epoch{i + 1} loss {l:f}')   # 打印：第几轮，损失保留 6 位小数





