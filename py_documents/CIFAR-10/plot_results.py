"""Plot the recorded CIFAR-10 runs; no training or smoothing.

Chart contract: two multi-series line figures, one for cross-entropy and one
for top-1 accuracy. All 80/80/80 recorded epochs are retained. Model identity
uses explicit blue/orange/olive colors plus circle/square/triangle markers;
solid/open-marker lines denote train, dashed/filled-marker lines denote test.
The PNG and SVG exports use identical 12 x 7 inch layouts and linear axes.
"""

import csv
import hashlib
import json
import math
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
from matplotlib.ticker import MultipleLocator

ROOT = Path(__file__).resolve().parent
MODELS = {
    "resnet18": ("ResNet-18 BasicBlock", 80, "#2864B4", "o"),
    "cnn": ("CNN", 80, "#D17632", "s"),
    "mlp": ("MLP", 80, "#7A852C", "^"),
}
FIELDS = ("epoch", "train_loss", "test_loss", "train_acc", "test_acc")


def read_results():
    all_rows, summary_rows, checks = {}, [], []
    for model, (label, expected_epochs, color, marker) in MODELS.items():
        csv_path = ROOT / "results" / f"{model}_cifar10_metrics.csv"
        json_path = ROOT / "results" / f"{model}_cifar10_summary.json"
        with csv_path.open(newline="", encoding="utf-8-sig") as file:
            raw = list(csv.DictReader(file))
        if not raw or not set(FIELDS).issubset(raw[0]):
            raise ValueError(f"Missing required fields: {csv_path}")
        rows = [{key: int(value) if key == "epoch" else float(value)
                 for key, value in row.items()} for row in raw]
        if [row["epoch"] for row in rows] != list(range(1, expected_epochs + 1)):
            raise ValueError(f"Incomplete or duplicate epochs: {csv_path}")
        for row in rows:
            if not all(math.isfinite(value) for value in row.values()):
                raise ValueError(f"Non-finite metric: {csv_path}")
            if any(row[key] < 0 for key in ("train_loss", "test_loss")):
                raise ValueError(f"Negative loss: {csv_path}")
            if any(not 0 <= row[key] <= 1 for key in ("train_acc", "test_acc")):
                raise ValueError(f"Accuracy outside [0,1]: {csv_path}")
        source_summary = json.loads(json_path.read_text(encoding="utf-8-sig"))
        final, best = rows[-1], max(rows, key=lambda row: row["test_acc"])
        for key in FIELDS:
            if not math.isclose(final[key], source_summary["final"][key],
                                rel_tol=1e-10, abs_tol=1e-10):
                raise ValueError(f"CSV/JSON disagreement: {model}/{key}")
        if not math.isclose(best["test_acc"], source_summary["best_test_acc"], abs_tol=1e-10):
            raise ValueError(f"Best accuracy disagreement: {model}")
        if best["epoch"] != source_summary["best_test_epoch"]:
            raise ValueError(f"Best epoch disagreement: {model}")
        summary_rows.append({
            "model": model, "display_name": label, "epochs": len(rows),
            "final_train_loss": final["train_loss"],
            "final_test_loss": final["test_loss"],
            "final_train_acc_pct": final["train_acc"] * 100,
            "final_test_acc_pct": final["test_acc"] * 100,
            "best_test_acc_pct": best["test_acc"] * 100,
            "best_test_epoch": best["epoch"],
        })
        checks.append({"model": model, "rows": len(rows), "csv_json_match": True,
                       "metrics_sha256": hashlib.sha256(csv_path.read_bytes()).hexdigest(),
                       "summary_sha256": hashlib.sha256(json_path.read_bytes()).hexdigest()})
        all_rows[model] = rows
    return all_rows, summary_rows, checks


def make_figure(all_rows, metric):
    fig, ax = plt.subplots(figsize=(12, 7), dpi=180)
    fig.subplots_adjust(left=0.09, right=0.975, bottom=0.17, top=0.72)
    title = "Train Loss & Test Loss" if metric == "loss" else "Train Accuracy & Test Accuracy"
    fig.text(0.09, 0.952, f"CIFAR-10 | {title}", fontsize=19, weight="bold", color="#20252B")
    fig.text(0.09, 0.903, "ResNet-18 BasicBlock / CNN / MLP: 80 epochs each", fontsize=11, color="#505862")
    fig.text(0.09, 0.869, "50,000 training images / 10,000 test images; recorded runs use different training protocols.", fontsize=10, color="#505862")
    handles = []
    for model, (label, count, color, marker) in MODELS.items():
        rows = all_rows[model]
        x = [row["epoch"] for row in rows]
        for split, style in (("train", "-"), ("test", "--")):
            values = [row[f"{split}_{metric}"] * (100 if metric == "acc" else 1) for row in rows]
            ax.plot(x, values, color=color, linestyle=style, linewidth=1.8,
                    marker=marker, markersize=4, markevery=list(range(0, len(rows), 8)),
                    markerfacecolor="white" if split == "train" else color,
                    markeredgecolor=color, alpha=0.95)
            ax.plot(x[-1], values[-1], marker=marker, color=color, markersize=5,
                    markerfacecolor="white" if split == "train" else color)
        handles.append(Line2D([], [], color=color, marker=marker, markersize=5,
                              linewidth=1.8, label=label))
    handles.extend([
        Line2D([], [], color="#434A52", linestyle="-", marker="o", markerfacecolor="white", label="Train"),
        Line2D([], [], color="#434A52", linestyle="--", marker="o", label="Test"),
    ])
    fig.legend(handles=handles, loc="upper left", bbox_to_anchor=(0.083, 0.833),
               ncol=5, frameon=False, fontsize=10, handlelength=2.3, columnspacing=1.4)
    ax.set_xlim(1, 81)
    ax.set_xticks([1, 10, 20, 30, 40, 50, 60, 70, 80])
    ax.set_xlabel("Epoch", labelpad=9)
    ax.set_ylabel("Cross-entropy loss (linear scale)" if metric == "loss" else "Top-1 accuracy (%)", labelpad=9)
    if metric == "acc":
        ax.set_ylim(0, 100)
        ax.yaxis.set_major_locator(MultipleLocator(20))
    else:
        ymax = max(row[f"{split}_loss"] for rows in all_rows.values()
                   for row in rows for split in ("train", "test"))
        ax.set_ylim(0, ymax * 1.06)
    ax.grid(axis="y", color="#DDE1E5", linewidth=0.7)
    ax.set_axisbelow(True)
    ax.spines[["top", "right"]].set_visible(False)
    ax.spines[["bottom", "left"]].set_color("#9098A1")
    fig.text(0.09, 0.073, "Solid / open markers: train; dashed / filled markers: test. Curves stop at the last recorded epoch.", fontsize=9, color="#505862")
    fig.text(0.09, 0.042, "Source: results/*_cifar10_metrics.csv. All observations retained; no smoothing or extrapolation.", fontsize=9, color="#505862")
    out = ROOT / "figures"
    out.mkdir(exist_ok=True)
    filename = "comparison_loss" if metric == "loss" else "comparison_accuracy"
    for extension in ("png", "svg"):
        fig.savefig(out / f"{filename}.{extension}", facecolor="white")
    plt.close(fig)


def write_summary(summary_rows, all_rows, checks):
    out = ROOT / "results"
    with (out / "comparison_summary.csv").open("w", newline="", encoding="utf-8-sig") as file:
        writer = csv.DictWriter(file, fieldnames=list(summary_rows[0]))
        writer.writeheader()
        writer.writerows(summary_rows)
    combined = []
    for model, rows in all_rows.items():
        for row in rows:
            combined.append({"model": model, **row})
    fieldnames = ["model", "epoch", "train_loss", "test_loss", "train_acc", "test_acc",
                  "train_top5_err", "test_top5_err", "learning_rate"]
    with (out / "comparison_metrics.csv").open("w", newline="", encoding="utf-8-sig") as file:
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(combined)
    (out / "comparison_summary.json").write_text(
        json.dumps(summary_rows, indent=2, ensure_ascii=False), encoding="utf-8")
    (out / "validation.json").write_text(json.dumps({
        "passed": True, "total_epochs": sum(len(rows) for rows in all_rows.values()),
        "checks": checks,
    }, indent=2), encoding="utf-8")
    table = ["| 模型 | 轮数 | Train Loss | Test Loss | Train Acc | Test Acc | 最佳 Test Acc | 最佳轮次 |",
             "|---|---:|---:|---:|---:|---:|---:|---:|"]
    for row in summary_rows:
        table.append(f"| {row['display_name']} | {row['epochs']} | {row['final_train_loss']:.4f} | {row['final_test_loss']:.4f} | {row['final_train_acc_pct']:.2f}% | {row['final_test_acc_pct']:.2f}% | {row['best_test_acc_pct']:.2f}% | {row['best_test_epoch']} |")
    readme = """# CIFAR-10：ResNet-18 BasicBlock、CNN 与 MLP

CNN 和 MLP 已按各自原配置从头重新训练 80 轮；ResNet-18 使用已有的 80 轮 BasicBlock 结果。所有曲线直接来自逐轮 CSV；记录的数值未修改。原 50 轮 CNN/MLP 记录保存在 `history/50epochs/`。

## 结果

""" + "\n".join(table) + """

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
& 'C:/Users/李/Desktop/D2L/.venv/Scripts/python.exe' .\\plot_results.py
```

重新训练指定模型（分别执行；仅写入新训练目录）：

```powershell
& 'C:/Users/李/Desktop/D2L/.venv/Scripts/python.exe' .\\train_models.py --model resnet18
& 'C:/Users/李/Desktop/D2L/.venv/Scripts/python.exe' .\\train_models.py --model cnn
& 'C:/Users/李/Desktop/D2L/.venv/Scripts/python.exe' .\\train_models.py --model mlp
```

训练结束后，需要图片时单独绘制，例如为新训练的 CNN 画图：

```powershell
& 'C:/Users/李/Desktop/D2L/.venv/Scripts/python.exe' .\\code\\plot_training_curves.py --models cnn --input-dir .\\new_runs\\cnn
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
"""
    (ROOT / "README.md").write_text(readme, encoding="utf-8")


def main():
    plt.rcParams.update({"font.family": "DejaVu Sans", "font.size": 11,
                         "svg.fonttype": "none", "axes.labelcolor": "#20252B",
                         "xtick.color": "#505862", "ytick.color": "#505862"})
    all_rows, summary_rows, checks = read_results()
    for metric in ("loss", "acc"):
        make_figure(all_rows, metric)
    write_summary(summary_rows, all_rows, checks)
    print(json.dumps({"validation": "passed", "results": summary_rows,
                      "figures": str(ROOT / "figures")}, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
