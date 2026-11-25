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
         << setw(22) << "Thoi gian (giay)"
         << "Trang thai\n";
    cout << string(82, '-') << "\n";

    for (size_t idx = 0; idx < sizes.size(); idx++) {
        int n = sizes[idx];
        int testNum = (int)idx + 1;

        ostringstream oss;
        oss << "testcase/test_" << setw(2) << setfill('0') << testNum
            << "_" << n << ".inp";
        string filepath = oss.str();

        ifstream inp(filepath);
        if (!inp.is_open()) {
            cerr << "Khong mo duoc file: " << filepath
                 << " (Hay chay Generate_TestCases.cpp truoc!)\n";
            return 1;
        }

        int fileN;
        inp >> fileN;
        vector<int> a(fileN);
        for (int i = 0; i < fileN; i++) {
            inp >> a[i];
        }
        inp.close();

        auto start = high_resolution_clock::now();
        if (fileN > 0) quickSort(a, 0, fileN - 1);
        auto stop = high_resolution_clock::now();

        double elapsedSec = duration_cast<duration<double>>(stop - start).count();
        bool ok = is_sorted(a.begin(), a.end());
        string status = ok ? "PASS" : "FAIL";

        ostringstream timeStr;
        timeStr << fixed << setprecision(6) << elapsedSec << "s";

        cout << left << setfill(' ') << setw(6) << testNum
             << setw(28) << filepath
             << setw(14) << fileN
             << setw(22) << timeStr.str()
             << status << "\n";

        csv << testNum << ","
            << filepath << ","
            << fileN << ","
            << fixed << setprecision(6) << elapsedSec << ","
            << status << "\n";
        csv.flush();
    }

    csv.close();
    cout << "\nDa luu bang thong ke ra file: ket_qua_quick_sort.csv\n";
    return 0;
}
