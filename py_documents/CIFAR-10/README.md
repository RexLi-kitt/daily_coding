# CIFAR-10：ResNet-18 BasicBlock、CNN 与 MLP

CNN 和 MLP 已按各自原配置从头重新训练 80 轮；ResNet-18 使用已有的 80 轮 BasicBlock 结果。所有曲线直接来自逐轮 CSV；记录的数值未修改。原 50 轮 CNN/MLP 记录保存在 `history/50epochs/`。

## 结果

| 模型 | 轮数 | Train Loss | Test Loss | Train Acc | Test Acc | 最佳 Test Acc | 最佳轮次 |
|---|---:|---:|---:|---:|---:|---:|---:|
| ResNet-18 BasicBlock | 80 | 0.0870 | 0.6905 | 96.96% | 84.41% | 84.50% | 60 |
| CNN | 80 | 0.3573 | 2.5128 | 87.49% | 55.35% | 57.96% | 37 |
| MLP | 80 | 0.4510 | 2.5301 | 84.08% | 44.69% | 52.53% | 36 |

## 曲线

- `figures/comparison_loss.png`：三个模型的 Train Loss 与 Test Loss，线性纵轴。
- `figures/comparison_accuracy.png`：三个模型的 Train Acc 与 Test Acc，单位 %。
- 同名 SVG 为可缩放版本。模型用颜色和点形区分；实线空心点为训练，虚线实心点为测试。
- 三个模型均绘至 80 轮；没有补齐、外推或平滑。

## 代码与结构

`code/` 已拆分模型、数据、训练与绘图；训练实现及超参数保持不变：

- `models.py`：仅定义网络结构及初始化，包含 ResNet、CNN、MLP。ResNet-18 使用 `[2,2,2,2]` BasicBlock，17 个主路径卷积层加 1 个全连接层；11,181,642 参数。起始层为 7×7、stride 2 卷积及最大池化，阶段通道为 64/128/256/512。捷径有实际相加。
- `data.py`：读取 CIFAR-10、转换像素值，并提供 ResNet 的图像增广。
- `train_resnet_cifar10.py`：ResNet 的训练、评估和 CSV/JSON/最终权重保存；不导入 Matplotlib，不自动绘图。
- `train_mlp_cifar10.py`：MLP/CNN 的训练、评估和 CSV/JSON 保存；不导入 Matplotlib，不自动绘图。
- `compare_resnet_cifar10.py`：保留原文件名的薄入口，转交给 ResNet 训练模块；不含训练循环或绘图实现。原 `--plot-only` 功能迁至独立绘图入口。
- `plot_training_curves.py`：读取指定目录的 CSV，独立绘制每个模型的 Loss、准确率和 Top-5 曲线；不导入 PyTorch。
- CNN：Conv(3→6,5×5,pad=2) → ReLU → AvgPool(2) → Conv(6→16,5×5) → ReLU → AvgPool(2) → Flatten → Linear(576→120) → ReLU → Linear(120→84) → ReLU → Linear(84→10)。卷积和全连接层使用 Xavier 初始化。
- MLP：Flatten(3072) → Linear(256) → ReLU → Linear(512) → ReLU → Linear(512) → Tanh → Linear(256) → ReLU → Linear(10)。
- `train_models.py`：三模型统一训练入口，沿用各自原配置，新的结果默认写入 `new_runs/`；通过明确的参数调用训练模块。
- `plot_results.py`：读取整理后的已有结果，核对数据后重新生成两张图和汇总。

## 原训练配置

| 项目 | ResNet-18 BasicBlock | CNN / MLP |
|---|---|---|
| 数据 | CIFAR-10：50,000 训练 / 10,000 测试；32×32 RGB | 相同 |
| 轮数 / batch size / seed | 80 / 512 / 42 | 80 / 256 / 42 |
| 优化器 | SGD，momentum=0.9，weight_decay=0.0001 | SGD，momentum=0，weight_decay=0 |
| 学习率 | 1轮0.01；2轮0.03；3–48轮0.05；49–64轮0.005；65–80轮0.0005 | 固定0.1 |
| 数据处理 | 像素/255，再按训练集通道均值与标准差标准化 | 像素/255 |
| 增广 | 补4像素后随机裁剪回32×32；随机水平翻转 | 无 |
| 精度 | BF16 autocast | FP32 |

Loss 为按样本数加权的平均交叉熵；Acc 为 top-1 正确分类比例。训练指标在训练过程中累计，测试指标为每轮结束后的 eval 模式评估。ResNet 的训练指标还包含随机增广，因此训练与测试值不是完全相同口径。三者轮数相同，但优化器、batch size、学习率、增广和精度方案不同，这些结果用于描述已有运行，不能单独归因于网络结构。每个模型仅有一次选定运行。

最佳 Test Acc 是反复评估测试集的历史最大值，仅作描述；严谨调参应使用独立验证集。现有 ResNet-18 权重保存在原项目 `resnet18_basicblock_80epochs/resnet18_cifar10_final.pt`，是第80轮权重；原 MLP/CNN 脚本未保存权重，本包不声称含有其模型权重。

## 使用方法

在本目录内，用原 GPU Python 环境重画已有结果：

```powershell
& 'C:/Users/李/Desktop/D2L/.venv/Scripts/python.exe' .\plot_results.py
```

重新训练指定模型（分别执行；仅写入新训练目录）：

```powershell
& 'C:/Users/李/Desktop/D2L/.venv/Scripts/python.exe' .\train_models.py --model resnet18
& 'C:/Users/李/Desktop/D2L/.venv/Scripts/python.exe' .\train_models.py --model cnn
& 'C:/Users/李/Desktop/D2L/.venv/Scripts/python.exe' .\train_models.py --model mlp
```

训练结束后，需要图片时单独绘制，例如为新训练的 CNN 画图：

```powershell
& 'C:/Users/李/Desktop/D2L/.venv/Scripts/python.exe' .\code\plot_training_curves.py --models cnn --input-dir .\new_runs\cnn
```

同理，`--models mlp` 或 `--models resnet18` 配合对应输入目录。`plot_results.py` 用于本包归档的三模型对比图；`code/plot_training_curves.py` 用于任意一次新训练的指标。训练过程不再自动产生图片。绘图可以在没有 CUDA 的环境中运行，且不依赖 PyTorch。

训练需要 GPU 版 PyTorch；绘图仅需要 Matplotlib。数据默认从本包上一级的 `cifar-10-python.tar.gz` 读取，也可使用 `--archive` 指定路径。完整参数见 `train_models.py --help`。新训练可能因 GPU 非确定性与历史值不同。

## 数据来源与核对

- ResNet-18：原项目 `resnet18_basicblock_80epochs/resnet18_cifar10_metrics.csv` 及同目录 summary JSON。
- CNN / MLP：本包 `new_runs/cnn_80epochs/` 和 `new_runs/mlp_80epochs/` 中本次重训的逐轮记录和 summary JSON，同时同步到原项目根目录。
- `results/comparison_metrics.csv`：240行合并记录；原始准确率单位为0–1，MLP/CNN没有学习率字段，该列留空。
- `results/comparison_summary.csv` / `.json`：最终结果及最佳测试准确率。
- `results/provenance.json`：结果来源、文件 SHA-256，以及代码拆分来源。
- `results/refactor_verification.json`：拆分前后实现一致性与独立训练/绘图检查。
- `results/validation.json`：轮次连续性、有限值、数值范围、CSV/JSON最终结果和最佳轮次一致性检查。
