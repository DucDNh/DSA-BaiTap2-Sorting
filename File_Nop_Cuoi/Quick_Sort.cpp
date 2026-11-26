#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;

mt19937 rng(42);

void quickSort(vector<int>& a, int left, int right) {
    if (left >= right) return;

    int pivot = a[left + rng() % (right - left + 1)];
    int i = left, j = right;

    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;
        if (i <= j) {
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }

    if (left < j) quickSort(a, left, j);
    if (i < right) quickSort(a, i, right);
}

int main() {
    vector<int> sizes = {
        100, 1000, 10000, 20000, 30000, 40000, 50000,
        60000, 70000, 80000, 100000, 150000, 200000,
        250000, 300000, 500000, 600000, 700000, 800000,
        900000, 1000000
    };

    ofstream csv("ket_qua_quick_sort.csv");
    if (!csv.is_open()) {
        cerr << "Loi: Khong the tao file ket_qua_quick_sort.csv!\n";
        return 1;
    }
    csv << "\xEF\xBB\xBF";
    csv << "STT,Ten_File,So_Phan_Tu_N,Thoi_Gian_Giay_s,Trang_Thai\n";

    cout << "=== THONG KE THOI GIAN CHAY: QUICK SORT (21 TEST CASES) ===\n\n";
    cout << left << setw(6) << "STT"
         << setw(28) << "Ten File"
         << setw(14) << "So phan tu"
         << setw(22) << "Thoi gian TB (giay)"
         << "Trang thai\n";
    cout << string(82, '-') << "\n";

    for (size_t idx = 0; idx < sizes.size(); idx++) {
        int size = sizes[idx];
        int testNum = (int)idx + 1;

        string path = "testcases/testcase_" + to_string(size) + ".txt";
        ifstream fin(path);
        if (!fin) {
            cerr << "Khong mo duoc file: " << path << '\n';
            continue;
        }
        int n, numArrays;
        if (!(fin >> n >> numArrays) || n != size || numArrays <= 0) {
            cerr << "Dong dau khong hop le: " << path << '\n';
            continue;
        }

        double totalSec = 0;
        bool ok = true;
        for (int t = 0; t < numArrays; ++t) {
            vector<int> arr(n);
            for (int& value : arr) {
                if (!(fin >> value)) {
                    cerr << "Du lieu khong hop le tai mang " << t + 1 << ": " << path << '\n';
                    return 1;
                }
            }

            auto start = high_resolution_clock::now();
            quickSort(arr, 0, n - 1);
            auto stop = high_resolution_clock::now();
            totalSec += duration<double>(stop - start).count();
            ok = ok && is_sorted(arr.begin(), arr.end());
        }

        double elapsedSec = totalSec / numArrays;
        string status = ok ? "PASS" : "FAIL";

        ostringstream timeStr;
        timeStr << fixed << setprecision(6) << elapsedSec << "s";

        cout << left << setfill(' ') << setw(6) << testNum
             << setw(28) << path
             << setw(14) << n
             << setw(22) << timeStr.str()
             << status << "\n";

        csv << testNum << ","
            << path << ","
            << n << ","
            << fixed << setprecision(6) << elapsedSec << ","
            << status << "\n";
        csv.flush();
    }

    csv.close();
    cout << "\nDa luu bang thong ke ra file: ket_qua_quick_sort.csv\n";
    return 0;
}



