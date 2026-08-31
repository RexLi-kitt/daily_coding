import torch
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random

def synthetic_data(w, b, num_examples):
    X = torch.normal(0,1,(num_examples,len(w)))
    y = torch.matmul(X, w) + b
    y += torch.normal(0,0.01, y.shape)
    return X,y.reshape(-1,1)

true_w = torch.tensor([2,-3.4])
true_b = 4.2
features,labels = synthetic_data(true_w,true_b,1000)

def data_iter(batch_size, features, labels):
    num_examples = len(features)
    indicies = list(range(num_examples))
    random.shuffle(indicies)
    for i in range(0,num_examples,batch_size):
        batch_indices = torch.tensor(indicies[i:min(i + batch_size,num_examples)])
        yield features[batch_indices], labels[batch_indices]

w = torch.zeros((2,1),requires_grad=True)
b = torch.zeros(1,requires_grad=True)

def linearg(X,w,b):
    return torch.matmul(X,w) + b

def sqaure_loss(y_hat, y):
    return (y_hat - y) ** 2 / 2

def sgd(params, lr, batch_size):
    with torch.no_grad():
        for param in params:
            param -= lr * param.grad / batch_size
            param.grad.zero_()

lr = 0.03
num_epoch = 30
batch_size = 10
net = linearg
loss = sqaure_loss

for epoch in range(num_epoch):
    for X,y in data_iter(batch_size,features,labels):
        l = loss(net(X,w,b),y)
        l.sum().backward()
        sgd([w,b],lr,batch_size)
    with torch.no_grad():
        train_l = loss(net(features,w,b),labels)
        print(f'epoch {epoch + 1}, loss {float(train_l.mean()):f}')



