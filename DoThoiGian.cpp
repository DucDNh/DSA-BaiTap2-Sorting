#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(arr[i], arr[min_idx]);
        }
    }
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void merge_2_Sorted_subArray(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    std::vector<int> b;
    int mid = left + (right - left) / 2,
        i = left,
        j = mid + 1;
    while (i <= mid || j <= right) {
        if (i <= mid && (j > right || arr[i] <= arr[j])) {
            b.push_back(arr[i]);
            i++;
        }
        else {
            b.push_back(arr[j]);
            j++;
        }
    }
    for (int h = 0; h < b.size(); h++) {
        arr[left + h] = b[h];
    }
}

void merge_sort(vector<int>& a, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    merge_sort(a, left, mid);
    merge_sort(a, mid + 1, right);
    merge_2_Sorted_subArray(a, left, right);
}

void xuat_mang(vector<int> a) {
    for (int i = 0; i < a.size(); i++) {
        cout << a[i] << ' ';
    }
    cout << '\n';
}

void QuickSort() {

}

void InsertionSort() {

}

// Danh sách kích thước (khớp với tên file testcase_<n>.txt)
const vector<int> SIZES = {
    100, 1000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000,
    100000, 150000, 200000, 250000, 300000,
    500000, 600000, 700000, 800000, 900000, 1000000
};

const string INPUT_DIR = "testcases/";      // thư mục chứa các file testcase
const string OUTPUT_FILE = "ket_qua.txt";   // file lưu bảng kết quả

// Selection Sort là O(n^2) nên với n quá lớn sẽ chạy rất lâu.
// Nếu n > giới hạn này thì bỏ qua Selection Sort (ghi -1). Đặt rất lớn để chạy tất cả.
const int SELECTION_SORT_LIMIT = 1000000;

// Đo thời gian (ms) của 1 lần sắp xếp trên bản sao của mảng
template <typename Func>
double measureMs(const vector<int>& original, Func sortFunc) {
    vector<int> a = original;   // copy để không làm hỏng dữ liệu gốc
    auto start = chrono::high_resolution_clock::now();
    sortFunc(a);
    auto end = chrono::high_resolution_clock::now();

    if (!is_sorted(a.begin(), a.end())) {
        cerr << "  [CANH BAO] Ket qua sap xep SAI!\n";
    }
    return chrono::duration<double, milli>(end - start).count();
}

// Ghi 1 dòng kết quả ra cả màn hình và file
void printRow(ostream& os, int n, double sel, double heap, double mer, double quick, double ins) {
    os << setw(10) << n << " |";
    if (sel < 0) os << setw(14) << "skip" << " |";
    else         os << setw(14) << fixed << setprecision(3) << sel << " |";
    os << setw(14) << fixed << setprecision(3) << heap << " |"
        << setw(14) << fixed << setprecision(3) << mer << " |"
        << setw(14) << fixed << setprecision(3) << quick << " |"
        << setw(14) << fixed << setprecision(3) << ins << "\n";
}

void printHeader(ostream& os) {
    os << "Thoi gian trung binh (ms)\n";
    os << setw(10) << "n" << " |" << setw(14) << "Selection" << " |" << setw(14) << "Heap" << " |"
        << setw(14) << "Merge" << " |" << setw(14) << "Quick" << " |" << setw(14) << "Insertion" << "\n";
    os << string(10 + 2 + 5 * 16, '-') << "\n";
}

int main() {
    ofstream fout(OUTPUT_FILE);
    printHeader(cout);
    printHeader(fout);

    for (int size : SIZES) {
        string path = INPUT_DIR + "testcase_" + to_string(size) + ".txt";
        ifstream fin(path);
        if (!fin) {
            cerr << "Khong mo duoc file: " << path << "\n";
            continue;
        }

        int n, numArrays;
        fin >> n >> numArrays;   // dòng đầu: kích thước và số mảng

        double sumSel = 0, sumHeap = 0, sumMerge = 0;
        bool runSelection = (n <= SELECTION_SORT_LIMIT);

        for (int t = 0; t < numArrays; t++) {
            vector<int> arr(n);
            for (int i = 0; i < n; i++) fin >> arr[i];

            if (runSelection) sumSel += measureMs(arr, [](vector<int>& a) { selectionSort(a); });
            sumHeap += measureMs(arr, [](vector<int>& a) { heapSort(a); });
            sumMerge += measureMs(arr, [](vector<int>& a) { merge_sort(a, 0, (int)a.size() - 1); });
        }

        double avgSel = runSelection ? sumSel / numArrays : -1;
        double avgHeap = sumHeap / numArrays;
        double avgMerge = sumMerge / numArrays;
        double avgQuick = 0;       // QuickSort chua cai dat -> ghi 0
        double avgIns = 0;       // InsertionSort chua cai dat -> ghi 0

        printRow(cout, n, avgSel, avgHeap, avgMerge, avgQuick, avgIns);
        printRow(fout, n, avgSel, avgHeap, avgMerge, avgQuick, avgIns);
        cout.flush();
        fout.flush();
    }

    cout << "\nDa luu ket qua vao " << OUTPUT_FILE << "\n";
    return 0;
}