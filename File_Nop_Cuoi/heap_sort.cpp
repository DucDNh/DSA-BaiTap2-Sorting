#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

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

int main() {
    cout << "=== KET QUA DO THOI GIAN HEAP SORT ===\n";
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
            heapSort(arr);
            auto stop = high_resolution_clock::now();
            totalSec += duration<double>(stop - start).count();
        }

        cout << left << setw(32) << path << setw(15) << n
             << fixed << setprecision(6) << totalSec / numArrays << " s\n";
    }

    return 0;
}



