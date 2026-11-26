#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;

// Gioi han thoi gian cho toi da 5 phut (300 giay) theo luu y trong de bai
const double TIMEOUT_SECONDS = 300.0;

// Tra ve true neu hoan thanh, false neu vuot qua 5 phut
bool insertionSort(vector<int>& a, high_resolution_clock::time_point startTime) {
    int n = (int)a.size();
    for (int i = 1; i < n; i++) {
        // Kiem tra thoi gian moi 2000 buoc de tranh overhead
        if ((i & 2047) == 0) {
            auto now = high_resolution_clock::now();
            double elapsed = duration_cast<duration<double>>(now - startTime).count();
            if (elapsed > TIMEOUT_SECONDS) {
                return false;
            }
        }

        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
    return true;
}

int main() {
    vector<int> sizes = {
        100, 1000, 10000, 20000, 30000, 40000, 50000,
        60000, 70000, 80000, 100000, 150000, 200000,
        250000, 300000, 500000, 600000, 700000, 800000,
        900000, 1000000
    };

    ofstream csv("ket_qua_insertion_sort.csv");
    if (!csv.is_open()) {
        cerr << "Loi: Khong the tao file ket_qua_insertion_sort.csv!\n";
        return 1;
    }
    csv << "\xEF\xBB\xBF";
    csv << "STT,Ten_File,So_Phan_Tu_N,Thoi_Gian_Giay_s,Trang_Thai\n";

    cout << "=== THONG KE THOI GIAN CHAY: INSERTION SORT (21 TEST CASES) ===\n\n";
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
        bool completed = true;
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
            completed = insertionSort(arr, start);
            auto stop = high_resolution_clock::now();
            totalSec += duration<double>(stop - start).count();
            if (!completed) break;
            ok = ok && is_sorted(arr.begin(), arr.end());
        }

        if (!completed) {
            cout << left << setfill(' ') << setw(6) << testNum
                 << setw(28) << path
                 << setw(14) << n
                 << setw(22) << "> 300.0000s"
                 << "TIMEOUT (> 5 phut)\n";

            csv << testNum << ","
                << path << ","
                << n << ","
                << "> 300s (Qua 5 phut),"
                << "TIMEOUT\n";
        } else {
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
        }
        csv.flush();
    }

    csv.close();
    cout << "\nDa luu bang thong ke ra file: ket_qua_insertion_sort.csv\n";
    return 0;
}



