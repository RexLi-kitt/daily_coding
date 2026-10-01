"""Draw a flowchart for the LeNet-style model in mnist_cnn.py."""

from pathlib import Path

import matplotlib

matplotlib.use("Agg")
from matplotlib import pyplot as plt
from matplotlib.font_manager import FontProperties
from matplotlib.patches import FancyArrowPatch, FancyBboxPatch, Rectangle


OUTPUT_DIR = Path(__file__).resolve().parent / "figures" / "CNN"
WIDTH, HEIGHT = 480, 1000
NODE_HEIGHT = 40
NODE_Y = [28 + 83 * i for i in range(12)]

# Colors sampled from the supplied MLP diagram.
FILL = "#FFFFFF"
STROKE = "#6E7479"
TEXT = "#323940"
ARROW = "#343434"
REGULAR = FontProperties(fname="C:/Windows/Fonts/arial.ttf", size=16)
BOLD = FontProperties(fname="C:/Windows/Fonts/arialbd.ttf", size=14)


def draw_node(ax, y, label, width=274, rounded=False):
    x = (WIDTH - width) / 2
    if rounded:
        patch = FancyBboxPatch(
            (x, y), width, NODE_HEIGHT,
            boxstyle="round,pad=0,rounding_size=19",
            linewidth=1.5, edgecolor=STROKE, facecolor=FILL,
        )
    else:
        patch = Rectangle((x, y), width, NODE_HEIGHT,
                          linewidth=1.5, edgecolor=STROKE, facecolor=FILL)
    ax.add_patch(patch)
    ax.text(WIDTH / 2, y + NODE_HEIGHT / 2, label, ha="center", va="center",
            color=TEXT, fontproperties=REGULAR)


def draw_arrow(ax, upper_y, lower_y, label=None):
    start = upper_y + NODE_HEIGHT
    end = lower_y - 2
    ax.add_patch(FancyArrowPatch((WIDTH / 2, start), (WIDTH / 2, end),
                                 arrowstyle="-|>", mutation_scale=16,
                                 linewidth=1.9, color=ARROW))
    if label:
        ax.text(WIDTH / 2 + 14, (start + end) / 2, label,
                ha="left", va="center", color=ARROW, fontproperties=BOLD)


def main():
    matplotlib.rcParams["svg.fonttype"] = "none"
    fig = plt.figure(figsize=(WIDTH / 100, HEIGHT / 100), dpi=100)
    ax = fig.add_axes((0, 0, 1, 1))
    ax.set_xlim(0, WIDTH)
    ax.set_ylim(HEIGHT, 0)
    ax.set_axis_off()

    labels = [
        "[batch_size,1,28,28]",
        "Conv2d(1,6,5,pad=2)",
        "[batch_size,6,28,28]",
        "[batch_size,6,14,14]",
        "Conv2d(6,16,5)",
        "[batch_size,16,10,10]",
        "[batch_size,16,5,5]",
        "[batch_size,400]",
        "Linear(400,120)",
        "Linear(120,84)",
        "Linear(84,10)",
        "probabilities = logits.softmax(dim=1)[0]",
    ]
    arrow_labels = [
        None, "ReLU", "AvgPool2d(2,2)", None, "ReLU",
        "AvgPool2d(2,2)", "Flatten", None, "ReLU", "ReLU", None,
    ]
    for index, (y, label) in enumerate(zip(NODE_Y, labels)):
        width = 216 if index == 0 else 424 if index == len(labels) - 1 else 274
        draw_node(ax, y, label, width=width,
                  rounded=index == 0 or index == len(labels) - 1)
        if index < len(labels) - 1:
            draw_arrow(ax, y, NODE_Y[index + 1], arrow_labels[index])

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    for extension, dpi in (("png", 200), ("svg", 100)):
        fig.savefig(OUTPUT_DIR / f"cnn_architecture.{extension}",
                    dpi=dpi, transparent=True)
    plt.close(fig)
    print(f"Saved CNN architecture diagram to {OUTPUT_DIR}")


if __name__ == "__main__":
    main()
