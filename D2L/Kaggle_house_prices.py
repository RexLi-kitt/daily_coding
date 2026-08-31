
import os
import numpy as np
import pandas as pd
import torch
from torch import nn
from pathlib import Path
from torch.utils.data import TensorDataset, DataLoader

DATA_DIR = Path(__file__).resolve().parent / "data"

train_data = pd.read_csv(DATA_DIR / "train.csv")
test_data = pd.read_csv(DATA_DIR / "test.csv")

quality_map = {'Po' : 1,'Fa' : 2,'TA' : 3,'Gd' : 4,'Ex' : 5, 'NA' : 0}
whole_data = pd.concat([train_data,test_data],axis=0)

str_data = whole_data.select_dtypes(include='string').columns

quality_cols = [c for c in str_data 
                if set(whole_data[c].dropna().unique()) <= set(quality_map)
                and whole_data[c].notna().any()]

for c in quality_cols:
    whole_data[c] = whole_data[c].fillna('NA').map(quality_map).astype(float)

num_cols = [c for c in whole_data.select_dtypes(include='number').columns
            if c not in ('Id', 'SalePrice')]
whole_data[num_cols] = (whole_data[num_cols] - whole_data[num_cols].mean()) / whole_data[num_cols].std()
whole_data[num_cols] = whole_data[num_cols].fillna(0)

whole_data = pd.get_dummies(whole_data,dummy_na=True)
whole_data = whole_data.astype(float)


train_d, test_d = whole_data.iloc[:1460],whole_data.iloc[1460:]

test_d_a = test_d.drop(columns=['Id','SalePrice'])


batch_size = 64
net = nn.Sequential(nn.Flatten(),
                    nn.Linear(286,256),
                    nn.ReLU(),
                    nn.Dropout(0.3),
                    nn.Linear(256,256),
                    nn.Tanh(),
                    nn.Dropout(0.2),
                    nn.Linear(256,1))
def create_package(batch_size):
    y_train = torch.tensor(np.log1p(train_d['SalePrice']).to_numpy(dtype= float),dtype=torch.float32)
    X_train = torch.tensor(train_d.drop(columns=['Id','SalePrice']).to_numpy(dtype=float),dtype=torch.float32)

    return DataLoader(TensorDataset(X_train, y_train),batch_size=batch_size,shuffle=True)


def train_a(net, train_package, loss, updater):
    if isinstance(net,nn.Sequential):
        net.train()

    for X, y in train_package:
        y_hat = net(X).reshape(-1)
        l = loss(y_hat,y)
        updater.zero_grad()
        l.backward()
        updater.step()

def train_b(net, train_package, loss, updater, num_epochs):
    for _ in range(num_epochs):
        train_a(net,train_package,loss,updater)

def test_func(net, test_X):
    net.eval()
    with torch.no_grad():
        pred = net(test_X)
    pred = np.expm1(pred.numpy()).reshape(-1)

    return pred
        
num_epochs = 150
lr = 0.01
trainer = torch.optim.SGD(net.parameters(),lr = lr)
loss = nn.MSELoss()

if __name__ == '__main__':
    for layer in net:
        if isinstance(layer,nn.Linear):
            nn.init.xavier_uniform_(layer.weight)
            nn.init.zeros_(layer.bias)
        
    train_b(net,create_package(batch_size),loss,trainer,num_epochs)
    test_X = torch.tensor(test_d_a.to_numpy(dtype=float),dtype=torch.float32)

    test_p = test_func(net,test_X)

    submission = pd.concat([test_d['Id'],pd.Series(test_p.reshape(-1),name='SalePrice')], axis=1)
    submission['Id'] = submission['Id'].astype(int)
    submission.to_csv('submission.csv', index=False)   # index=False 是灵魂
    chk = pd.read_csv('submission.csv')
    print(chk.shape)          # (1459, 2)
    print(chk.columns)        # ['Id', 'SalePrice']
    print(chk.head())         # Id 从 1461 开始
    print(chk['SalePrice'].min())   # 应为正数（还原过的房价）





    

