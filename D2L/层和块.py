import torch
from torch import nn
from torch.nn import functional as F

class MLP(nn.Module):
    def __init__(self):
        super().__init__()
        self.hidden = nn.Linear(20,256)
        self.out = nn.Linear(256,10)


    def forward(self, X):
        return self.out(F.relu(self.hidden(X)))

class MySeq(nn.Module):
    def __init__(self, *args):
        super().__init__()
        for idx, module in enumerate(args):
            self._modules[str(idx)] = module

    def forward(self,X):
        for block in self._modules.values():
            X = block(X)
        return X


class FixedHidden(nn.Module):
    def __init__(self, *args):
        super().__init__()
        self.randm_weight = torch.rand((20,20),requires_grad=False)
        self.linear = nn.Linear(20,20)

    def forward(self, X):
        X = self.linear(X)
        X = F.relu(torch.mm(X, self.randm_weight) + 1)
        X = self.linear(X)
        while X.abs().sum() > 1:
            X /= 2
        return X.sum()

class nestMLP(nn.Module):
    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(nn.Linear(20, 64), nn.ReLU(),
                                 nn.Linear(64, 32), nn.ReLU())
        self.linear = nn.Linear(32, 16)

    def forward(self, X):
        return self.linear(self.net(X))

class ParallelBlock(nn.Module):
    def __init__(self,*args):
        super().__init__()
        for idx,model in enumerate(args):
            self.add_module(f'branch{idx}',model)

    def forward(self, X):
        outputs = [block(X) for block in self.children()]
        return torch.concat(outputs,dim=1)