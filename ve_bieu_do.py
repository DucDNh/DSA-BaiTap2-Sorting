"""Vẽ ba biểu đồ từ bảng thời gian chạy và lưu cạnh tệp chương trình."""
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

# Mỗi dòng gồm n và thời gian theo thứ tự của ALGORITHMS.
# ms = mili giây, s = giây, >5p = quá 5 phút.
ALGORITHMS = ["Selection Sort", "Heap Sort", "Merge Sort", "Quick Sort", "Insertion Sort"]
DATA = """
100     0.029ms 0.019ms 0.251ms 0.015ms 0.019ms
1000    2.260ms 0.306ms 2.892ms 0.134ms 2.202ms
10000   217.1ms 3.693ms 30.69ms 1.636ms 236.6ms
20000   821.2ms 7.893ms 62.17ms 4.075ms 939.8ms
30000   1.910s 12.60ms 95.42ms 6.128ms 2.007s
40000   3.397s 17.50ms 129.4ms 8.009ms 3.613s
50000   5.144s 21.21ms 155.4ms 12.01ms 5.595s
60000   7.511s 27.04ms 192.1ms 18.49ms 10.54s
70000   10.34s 32.16ms 225.9ms 20.78ms 14.81s
80000   13.29s 36.97ms 261.3ms 23.07ms 20.02s
100000  19.80s 45.05ms 307.4ms 22.29ms 29.49s
150000  43.67s 69.19ms 457.4ms 35.29ms 67.79s
200000  60.7s 107.5ms 714.5ms 49.33ms 117.5s
250000  76.2s 196.5ms 1.227s 59.48ms 180.8s
300000  110.3s 245.0ms 1.546s 69.08ms 250.9s
500000  305.6s 418.2ms 2.583s 127.7ms >5p
600000  439.2s 513.7ms 3.158s 151.7ms >5p
700000  >5p 596.3ms 3.720s 178.1ms >5p
800000  >5p 721.0ms 4.113s 201.8ms >5p
900000  >5p 802.1ms 4.825s 235.9ms >5p
1000000 >5p 929.9ms 5.227s 281.9ms >5p
"""
COLORS = ["#b64c12", "#176990", "#704aa1", "#00816b", "#ae3269"]
MARKERS = ["o", "s", "D", "^", "x"]


def seconds(value):
    if value == ">5p":
        return float("nan")
    if value.endswith("ms"):
        return float(value[:-2]) / 1000
    if value.endswith("s"):
        return float(value[:-1])
    raise ValueError(value)


def main():
    rows = [line.split() for line in DATA.strip().splitlines()]
    sizes = [int(row[0]) for row in rows]
    fig, (overview, detail, slow) = plt.subplots(3, 1, figsize=(10, 13), layout="constrained")
    for i, name in enumerate(ALGORITHMS):
        times = [seconds(row[i + 1]) for row in rows]
        style = dict(label=name, color=COLORS[i], marker=MARKERS[i], markersize=4)
        overview.plot(sizes, times, **style)
        if name in ("Heap Sort", "Quick Sort", "Merge Sort"):
            detail.plot(sizes, times, **style)
        else:
            slow.plot(sizes, times, **style)

    overview.set(xscale="log", yscale="log", ylabel="Thời gian (giây)",
                 title="Cả 5 thuật toán · thang logarit trên hai trục")
    overview.legend(ncol=2)
    overview.set_xticks([100, 1000, 10000, 100000, 1000000])
    detail.set(ylabel="Thời gian (giây)", ylim=(0, None),
               title="Heap Sort, Merge Sort và Quick Sort · thang tuyến tính")
    detail.legend()
    slow.set(ylabel="Thời gian (giây)", ylim=(0, None), xlim=(0, max(sizes)),
             title="Selection Sort và Insertion Sort · thang tuyến tính")
    slow.legend()
    for ax in (overview, detail, slow):
        ax.set_xlabel("Kích thước mảng (phần tử)")
        ax.xaxis.set_major_formatter(FuncFormatter(lambda x, _: f"{x:,.0f}".replace(",", ".")))
        ax.grid(alpha=0.22)

    # Ghi rõ các ô chỉ có cận dưới, giữ nguyên các số đo chính xác trên 300 giây.
    notes = []
    for i, name in enumerate(ALGORITHMS):
        over = [n for n, row in zip(sizes, rows) if row[i + 1] == ">5p"]
        if over:
            notes.append(f"{name} từ {min(over):,} phần tử".replace(",", "."))
    overview.set_xlabel("Kích thước mảng (phần tử)\nQuá 5 phút: " + " · ".join(notes))
    slow.set_xlabel(overview.get_xlabel())
    fig.suptitle("So sánh thời gian chạy của các thuật toán sắp xếp", fontsize=17)
    output = Path(__file__).with_name("thoi_gian_sap_xep.png")
    fig.savefig(output, dpi=170)
    plt.close(fig)
    print(f"Đã lưu {output}")


if __name__ == "__main__":
    main()



