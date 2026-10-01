"""Train the existing MNIST MLP and export epoch metrics and two figures.

Place beside mnist_mlp.py, then run: python train_mnist_curves.py
Optional: --epochs 20 --batch-size 256 --lr 0.1 --device cpu
Use --history path/to/history.csv to redraw without training.
Train/test curves are evaluated AFTER each epoch on the full splits.
Test results are monitoring only; no early stopping or test-based selection.
"""
import argparse
import csv
import importlib.util
import json
import random
import time
from datetime import datetime
from pathlib import Path

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
import torch
from torch import nn
from torch.utils.data import DataLoader, TensorDataset


@torch.inference_mode()
def evaluate(model, loader, device):
    model.eval()
    loss_sum, correct, total = 0.0, 0, 0
    for x, y in loader:
        x = x.to(device).float().div_(255)
        y = y.to(device)
        logits = model(x)
        loss_sum += nn.functional.cross_entropy(logits, y, reduction='sum').item()
        correct += (logits.argmax(1) == y).sum().item()
        total += y.numel()
    return loss_sum / total, correct / total, correct, total


def plot_history(rows, out):
    # Contract: two static line charts, one point per epoch; full-split
    # post-epoch mean CE and accuracy. Blue solid vs orange dashed/open markers.
    # No smoothing; neutral titles; PNG inspected before delivery.
    plt.rcParams.update({
        'font.family': 'serif', 'font.serif': ['STIXGeneral', 'DejaVu Serif'],
        'mathtext.fontset': 'stix', 'font.size': 10,
        'axes.labelsize': 11, 'axes.titlesize': 12, 'axes.linewidth': .8,
        'axes.spines.top': True, 'axes.spines.right': True,
        'xtick.direction': 'in', 'ytick.direction': 'in',
        'xtick.top': True, 'ytick.right': True,
        'xtick.labelsize': 9, 'ytick.labelsize': 9,
        'legend.fontsize': 10, 'pdf.fonttype': 42, 'ps.fonttype': 42,
        'svg.fonttype': 'none', 'savefig.facecolor': 'white',
    })
    epochs = [int(r['epoch']) for r in rows]
    for metric, ylabel, filename in [('loss', 'Mean cross-entropy loss', 'loss_curve.png'),
                                     ('acc', 'Accuracy (%)', 'accuracy_curve.png')]:
        fig, ax = plt.subplots(figsize=(5.2, 3.8))
        for split, color, marker, style in [('train', '#2563A6', 'o', '-'),
                                             ('test', '#CE732A', 's', '--')]:
            values = [float(r[f'{split}_{metric}']) * (100 if metric == 'acc' else 1) for r in rows]
            ax.plot(epochs, values, label='Training' if split == 'train' else 'Test',
                    color=color, marker=marker, linestyle=style, linewidth=1.3,
                    markersize=3.4, markeredgewidth=.8,
                    markerfacecolor='white' if split == 'test' else color)
        fig.suptitle(f'MNIST: {"cross-entropy loss" if metric == "loss" else "classification accuracy"}', fontsize=12, y=.98)
        ax.set_title(f'Post-epoch evaluation; train $n={int(rows[0]["train_n"]):,}$, test $n={int(rows[0]["test_n"]):,}$', fontsize=8, pad=9)
        ax.set_xlabel('Epoch')
        ax.set_ylabel(ylabel)
        ax.xaxis.set_major_locator(MaxNLocator(nbins=6, integer=True))
        ax.set_xlim(min(epochs)-.4, max(epochs)+.4)
        if metric == 'acc':
            # Explicit focused scale makes the generalization gap readable.
            minimum = min(float(r[f'{s}_acc'])*100 for r in rows for s in ('train', 'test'))
            lower = max(0, 5 * (int(minimum)//5))
            ax.set_ylim(lower, 100.5)
            ax.set_ylabel(f'Accuracy (%) [{lower}\u2013100% scale]')
        else:
            ax.set_ylim(bottom=0)
        ax.set_axisbelow(True)
        ax.grid(axis='y', color='#dedede', linewidth=.5, linestyle=':')
        ax.legend(frameon=False, loc='upper right' if metric == 'loss' else 'lower right')
        fig.tight_layout(rect=(0, 0, 1, .94), pad=.7)
        for suffix in ('png', 'pdf', 'svg'):
            fig.savefig(out / Path(filename).with_suffix('.' + suffix), dpi=600,
                        bbox_inches='tight', pad_inches=.05)
        plt.close(fig)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--project-dir', type=Path, default=Path(__file__).resolve().parent)
    p.add_argument('--output-dir', type=Path)
    p.add_argument('--history', type=Path)
    p.add_argument('--epochs', type=int, default=20)
    p.add_argument('--batch-size', type=int, default=256)
    p.add_argument('--lr', type=float, default=.1)
    p.add_argument('--seed', type=int, default=42)
    p.add_argument('--device', choices=['auto', 'cpu', 'cuda'], default='auto')
    a = p.parse_args()
    if a.epochs < 1 or a.batch_size < 1 or a.lr <= 0:
        p.error('epochs, batch-size and lr must be positive')
    out = a.output_dir or a.project_dir / 'curve_runs' / datetime.now().strftime('%Y%m%d_%H%M%S')
    out.mkdir(parents=True, exist_ok=True)
    if a.history:
        with a.history.open(newline='') as f:
            plot_history(list(csv.DictReader(f)), out)
        return
    if (out / 'history.csv').exists():
        p.error('Output already contains history.csv; choose a new output directory')
    spec = importlib.util.spec_from_file_location('mnist_source', a.project_dir / 'mnist_mlp.py')
    source = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(source)
    random.seed(a.seed)
    torch.manual_seed(a.seed)
    torch.set_num_threads(4)
    device = torch.device('cuda' if a.device == 'auto' and torch.cuda.is_available() else 'cpu' if a.device == 'auto' else a.device)
    if device.type == 'cuda':
        torch.cuda.manual_seed_all(a.seed)
    train = TensorDataset(*source.load_split('train'))
    test = TensorDataset(*source.load_split('t10k'))
    loader = DataLoader(train, batch_size=a.batch_size, shuffle=True,
                        generator=torch.Generator().manual_seed(a.seed))
    train_eval = DataLoader(train, batch_size=1024)
    test_eval = DataLoader(test, batch_size=1024)
    model = source.make_model().to(device)
    optimizer = torch.optim.SGD(model.parameters(), lr=a.lr)
    config = {k: str(v) if isinstance(v, Path) else v for k, v in vars(a).items()}
    config.update(torch_version=torch.__version__, device_used=str(device),
                  gpu=torch.cuda.get_device_name() if device.type == 'cuda' else None,
                  train_n=len(train), test_n=len(test), optimizer='SGD', model=str(model),
                  metric_definition='Full-split post-epoch evaluation; batch_loss is online training mean',
                  test_usage='Monitoring only; fixed epoch count, no test-based model selection')
    (out / 'config.json').write_text(json.dumps(config, indent=2), encoding='utf-8')
    print(f'Device: {device}; train={len(train)}, test={len(test)}; output={out}', flush=True)
    rows = []
    fields = ['epoch', 'batch_loss', 'train_loss', 'test_loss', 'train_acc', 'test_acc',
              'train_correct', 'test_correct', 'train_n', 'test_n', 'seconds']
    with (out / 'history.csv').open('w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=fields)
        writer.writeheader()
        for epoch in range(1, a.epochs + 1):
            start = time.perf_counter()
            model.train()
            loss_sum = 0.0
            for x, y in loader:
                x, y = x.to(device).float().div_(255), y.to(device)
                optimizer.zero_grad(set_to_none=True)
                loss = nn.functional.cross_entropy(model(x), y)
                loss.backward()
                optimizer.step()
                loss_sum += loss.item() * len(y)
            tr_loss, tr_acc, tr_ok, tr_n = evaluate(model, train_eval, device)
            te_loss, te_acc, te_ok, te_n = evaluate(model, test_eval, device)
            row = dict(zip(fields, [epoch, loss_sum/len(train), tr_loss, te_loss, tr_acc, te_acc,
                                    tr_ok, te_ok, tr_n, te_n, time.perf_counter()-start]))
            rows.append(row)
            writer.writerow(row)
            f.flush()
            print(f'Epoch {epoch:02d}/{a.epochs} | loss {tr_loss:.4f}/{te_loss:.4f} | acc {tr_acc:.2%}/{te_acc:.2%} | {row["seconds"]:.1f}s', flush=True)
    torch.save(model.cpu().state_dict(), out / 'mnist_mlp_curves.pt')
    plot_history(rows, out)
    print('Saved history.csv, config.json, model and both PNG figures.', flush=True)


if __name__ == '__main__':
    main()
