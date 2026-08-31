
import torch
from torch.distributions import multinomial
fair_probs = torch.ones([6]) / 6
multinomial.Multinomial(1, fair_probs).sample()

counts = multinomial.Multinomial(10, fair_probs).sample((500,))
cum_counts = counts.cumsum(dim=0)
estimates = cum_counts / cum_counts.sum(dim=1, keepdims=True)
print(estimates)

import matplotlib.pyplot as plt

# 中文字体设置
plt.rcParams['font.sans-serif'] = ['Microsoft YaHei', 'SimHei']
plt.rcParams['axes.unicode_minus'] = False

plt.figure(figsize=(10, 6))

# 6 条曲线，每条对应一个骰子面
for i in range(6):
    plt.plot(range(1, 501), estimates[:, i].numpy(), label=f'面 {i+1}')

# 黑色虚线 = 真实概率 1/6
plt.axhline(y=1/6, color='black', linestyle='--', linewidth=1.5,
            label='真实概率 1/6')

plt.xlabel('实验组数')
plt.ylabel('估计概率')
plt.title('大数定律演示：频率 → 真实概率')
plt.legend(ncol=4)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.show()