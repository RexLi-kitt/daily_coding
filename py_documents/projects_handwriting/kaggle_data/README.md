将 Kaggle Digit Recognizer 比赛下载的 `train.csv` 和 `test.csv` 放在本目录。
不要用原始 MNIST 的 `.gz` 文件替代，也不要把 sample_submission.csv 当作测试数据。

在 projects_handwriting 目录的 PowerShell 中运行：

```powershell
& "C:\Users\李\Desktop\D2L\.venv\Scripts\python.exe" .\kaggle_digit_recognizer.py
```

默认训练 20 轮，自动使用可用的 GPU；从 train.csv 随机划分 80% 训练、20% 验证。
优化器使用普通 SGD，不加动量和权重衰减，默认学习率为 0.1，可通过 --lr 修改。
每轮记录训练和验证指标，保存验证准确率最高的模型，用该模型预测 test.csv。
比赛测试数据没有标签，因此不计算测试准确率；上传后由 Kaggle 评分。

输出位于 projects_handwriting/kaggle_output：

- kaggle_cnn.pt：最佳验证模型。
- history.csv：每轮训练和验证损失、准确率、耗时。
- loss_curve.png、acc_curve.png：训练和验证曲线。
- submission.csv：测试集预测，只有 ImageId、Label 两列；编号从 1 开始，顺序对应 test.csv。

只重新导出预测，不训练：

```powershell
& "C:\Users\李\Desktop\D2L\.venv\Scripts\python.exe" .\kaggle_digit_recognizer.py --predict-only
```

自定义数据路径、轮数或输出目录：

```powershell
& "C:\Users\李\Desktop\D2L\.venv\Scripts\python.exe" .\kaggle_digit_recognizer.py --train-csv "D:\data\train.csv" --test-csv "D:\data\test.csv" --epochs 20 --output-dir .\kaggle_output_run2
```

重复运行会覆盖所选输出目录中的同名文件。保留不同训练结果时请指定新的 --output-dir。
