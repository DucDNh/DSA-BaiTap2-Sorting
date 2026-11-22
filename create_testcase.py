import os
import random

# ====== CẤU HÌNH ======
SIZES = [
    100, 1000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000,
    100000, 150000, 200000, 250000, 300000,
    500000, 600000, 700000, 800000, 900000, 1000000,
]
NUM_ARRAYS = 20            # số mảng test case cho mỗi kích thước (>= 20)
MIN_VAL = -1_000_000_000       # giá trị nhỏ nhất
MAX_VAL = 1_000_000_000        # giá trị lớn nhất
OUTPUT_DIR = "testcases"   # thư mục chứa file .txt
SEED = 12345               # đặt None nếu muốn mỗi lần chạy ra dữ liệu khác nhau
SINGLE_FILE = False       # True: gộp tất cả vào 1 file duy nhất (rất nặng, ~1GB)

# Dùng numpy nếu có (nhanh hơn nhiều), không có thì dùng random thuần
try:
    import numpy as np
    HAS_NP = True
except ImportError:
    HAS_NP = False


def gen_array(n, rng_np=None):
    if HAS_NP:
        return rng_np.integers(MIN_VAL, MAX_VAL + 1, size=n)
    return [random.randint(MIN_VAL, MAX_VAL) for _ in range(n)]


def write_block(f, n, num_arrays, rng_np=None):
    """Ghi 1 khối: dòng 'n', rồi num_arrays mảng (mỗi mảng 1 dòng)."""
    for _ in range(num_arrays):
        arr = gen_array(n, rng_np)
        f.write(" ".join(map(str, arr.tolist() if HAS_NP else arr)))
        f.write("\n")


def main():
    if SEED is not None:
        random.seed(SEED)
    rng_np = np.random.default_rng(SEED) if HAS_NP else None

    os.makedirs(OUTPUT_DIR, exist_ok=True)

    if SINGLE_FILE:
        path = os.path.join(OUTPUT_DIR, "all_testcases.txt")
        with open(path, "w") as f:
            for n in SIZES:
                f.write(f"{n} {NUM_ARRAYS}\n")   # header: kích thước, số mảng
                write_block(f, n, NUM_ARRAYS, rng_np)
                print(f"Đã ghi n = {n}")
        print("Xong:", path)
    else:
        for n in SIZES:
            path = os.path.join(OUTPUT_DIR, f"testcase_{n}.txt")
            with open(path, "w") as f:
                f.write(f"{n} {NUM_ARRAYS}\n")   # header: kích thước, số mảng
                write_block(f, n, NUM_ARRAYS, rng_np)
            size_mb = os.path.getsize(path) / 1024 / 1024
            print(f"Đã tạo {path} ({size_mb:.1f} MB)")
        print("Hoàn tất!")


if __name__ == "__main__":
    main()
