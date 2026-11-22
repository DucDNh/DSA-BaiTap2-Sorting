import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

# ====================== DỮ LIỆU (đơn vị: ms) ======================
# None = "Quá 5p" (không có số liệu)
N = [100, 1000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000,
     100000, 150000, 200000, 250000, 300000, 500000, 600000,
     700000, 800000, 900000, 1000000]

selection = [0.029, 2.260, 217.1, 821.2, 1910, 3397, 5144, 7511, 10340, 13290,
             19800, 43670, 60700, 76200, 110300, 305600, 439200,
             None, None, None, None]

heap = [0.019, 0.306, 3.693, 7.893, 12.60, 17.50, 21.21, 27.04, 32.16, 36.97,
        45.05, 69.19, 107.5, 196.5, 245.0, 418.2, 513.7,
        596.3, 721.0, 802.1, 929.9]

merge = [0.251, 2.892, 30.69, 62.17, 95.42, 129.4, 155.4, 192.1, 225.9, 261.3,
         307.4, 457.4, 714.5, 1227, 1546, 2583, 3158,
         3720, 4113, 4825, 5227]

quick = [0.0] * len(N)       # chưa cài đặt
insertion = [0.0] * len(N)   # chưa cài đặt

# Các điểm của Selection Sort từ n này trở lên là số ước lượng (không phải đo thật)
# -> vẽ nét đứt + marker rỗng. Đặt None nếu muốn coi tất cả là số đo thật.
SELECTION_ESTIMATED_FROM = 250000

SAVE_PNG = True   # lưu biểu đồ ra file .png


# ====================== HÀM HỖ TRỢ ======================
def fmt_n(x, _pos=None):
    """Định dạng nhãn trục n: 1000 -> 1K, 1000000 -> 1M"""
    if x >= 1_000_000:
        return f"{x / 1_000_000:g}M"
    if x >= 1_000:
        return f"{x / 1_000:g}K"
    return f"{x:g}"


def fmt_time(ms, _pos=None):
    """Định dạng nhãn trục thời gian: ms / s / phút"""
    if ms >= 60_000:
        return f"{ms / 60_000:.3g} phút"
    if ms >= 1_000:
        return f"{ms / 1_000:.3g} s"
    return f"{ms:.3g} ms"


def valid_points(xs, ys):
    """Bỏ các điểm None và điểm = 0 (không vẽ được trên thang log)."""
    px, py = [], []
    for x, y in zip(xs, ys):
        if y is not None and y > 0:
            px.append(x)
            py.append(y)
    return px, py


def plot_algo(ax, xs, ys, label, color, marker, estimated_from=None):
    px, py = valid_points(xs, ys)
    if not px:
        return
    if estimated_from is None:
        ax.plot(px, py, marker=marker, color=color, label=label, linewidth=1.8, markersize=5)
        return

    # tách phần đo thật / phần ước lượng
    real = [(x, y) for x, y in zip(px, py) if x < estimated_from]
    est = [(x, y) for x, y in zip(px, py) if x >= estimated_from]
    if real:
        rx, ry = zip(*real)
        ax.plot(rx, ry, marker=marker, color=color, label=label, linewidth=1.8, markersize=5)
    if est:
        ex, ey = zip(*est)
        if real:  # nối điểm cuối đo thật với điểm ước lượng đầu tiên
            ex = (real[-1][0],) + ex
            ey = (real[-1][1],) + ey
        ax.plot(ex, ey, linestyle="--", color=color, linewidth=1.8)
        ax.plot(ex[1:] if real else ex, ey[1:] if real else ey, marker=marker, linestyle="none",
                markerfacecolor="white", color=color, markersize=6,
                label=f"{label} (ước lượng)")


def style_axes(ax, title, log=False):
    ax.set_title(title, fontsize=13, fontweight="bold")
    ax.set_xlabel("Kích thước mảng (n)")
    ax.set_ylabel("Thời gian chạy trung bình")
    if log:
        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.grid(True, which="both", alpha=0.3)
    else:
        ax.grid(True, alpha=0.3)
    # đặt formatter SAU khi chọn thang đo (nếu không sẽ bị thang log ghi đè)
    ax.xaxis.set_major_formatter(FuncFormatter(fmt_n))
    ax.yaxis.set_major_formatter(FuncFormatter(fmt_time))
    if log:
        ax.xaxis.set_minor_formatter(FuncFormatter(lambda *_: ""))
        ax.yaxis.set_minor_formatter(FuncFormatter(lambda *_: ""))
    ax.legend()


COLORS = {"Selection": "#d62728", "Heap": "#1f77b4", "Merge": "#2ca02c",
          "Quick": "#ff7f0e", "Insertion": "#9467bd"}


# ====================== VẼ BIỂU ĐỒ ======================
def main():
    # --- Biểu đồ 1: tất cả thuật toán, thang tuyến tính ---
    fig1, ax1 = plt.subplots(figsize=(10, 6))
    plot_algo(ax1, N, selection, "Selection", COLORS["Selection"], "o", SELECTION_ESTIMATED_FROM)
    plot_algo(ax1, N, heap, "Heap", COLORS["Heap"], "s")
    plot_algo(ax1, N, merge, "Merge", COLORS["Merge"], "^")
    plot_algo(ax1, N, quick, "Quick", COLORS["Quick"], "d")
    plot_algo(ax1, N, insertion, "Insertion", COLORS["Insertion"], "x")
    style_axes(ax1, "So sánh thời gian chạy các thuật toán sắp xếp (thang tuyến tính)")
    fig1.tight_layout()

    # --- Biểu đồ 2: thang log-log (dễ so sánh khi chênh lệch quá lớn) ---
    fig2, ax2 = plt.subplots(figsize=(10, 6))
    plot_algo(ax2, N, selection, "Selection", COLORS["Selection"], "o", SELECTION_ESTIMATED_FROM)
    plot_algo(ax2, N, heap, "Heap", COLORS["Heap"], "s")
    plot_algo(ax2, N, merge, "Merge", COLORS["Merge"], "^")
    style_axes(ax2, "So sánh thời gian chạy (thang log-log)", log=True)
    fig2.tight_layout()

    # --- Biểu đồ 3: chỉ Heap vs Merge (Selection quá lớn nên che mất hai đường này) ---
    fig3, ax3 = plt.subplots(figsize=(10, 6))
    plot_algo(ax3, N, heap, "Heap", COLORS["Heap"], "s")
    plot_algo(ax3, N, merge, "Merge", COLORS["Merge"], "^")
    style_axes(ax3, "Heap Sort và Merge Sort (thang tuyến tính)")
    fig3.tight_layout()

    if SAVE_PNG:
        fig1.savefig("bieu_do_tat_ca.png", dpi=150)
        fig2.savefig("bieu_do_log.png", dpi=150)
        fig3.savefig("bieu_do_heap_merge.png", dpi=150)
        print("Đã lưu: bieu_do_tat_ca.png, bieu_do_log.png, bieu_do_heap_merge.png")

    plt.show()


if __name__ == "__main__":
    main()
