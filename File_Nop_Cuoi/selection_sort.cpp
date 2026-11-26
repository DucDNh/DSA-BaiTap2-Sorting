#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

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

int main() {
    const int MAX_SIZE_TO_RUN = 200000; 

    cout << "=== KET QUA DO THOI GIAN SELECTION SORT ===\n";
    cout << left << setw(32) << "Test Case" 
         << setw(15) << "So phan tu" 
         << "Thoi gian TB (giay)\n";
    cout << "--------------------------------------------------\n";

    const vector<int> sizes = {
        100, 1000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000,
        100000, 150000, 200000, 250000, 300000,
        500000, 600000, 700000, 800000, 900000, 1000000
    };

    for (int size : sizes) {
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

        if (n > MAX_SIZE_TO_RUN) {
            cout << left << setw(32) << path << setw(15) << n << "Bo qua (n > 200000)\n";
            continue;
        }

        double totalSec = 0;
        for (int t = 0; t < numArrays; ++t) {
            vector<int> arr(n);
            for (int& value : arr) {
                if (!(fin >> value)) {
                    cerr << "Du lieu khong hop le tai mang " << t + 1 << ": " << path << '\n';
                    return 1;
                }
            }

            auto start = high_resolution_clock::now();
            selectionSort(arr);
            auto stop = high_resolution_clock::now();
            totalSec += duration<double>(stop - start).count();
        }

        cout << left << setw(32) << path << setw(15) << n
             << fixed << setprecision(6) << totalSec / numArrays << " s\n";
    }

    return 0;
}



