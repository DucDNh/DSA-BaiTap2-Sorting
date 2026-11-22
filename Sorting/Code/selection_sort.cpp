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
    cout << left << setw(15) << "Test Case" 
         << setw(15) << "So phan tu" 
         << "Thoi gian chay (giay)\n";
    cout << "--------------------------------------------------\n";

    for (int i = 1; i <= 10; ++i) {
        string filename = "Test_case/test_" + to_string(i) + ".txt";
        ifstream inFile(filename);
        if (!inFile) {
            cout << "Khong tim thay file " << filename << ". Hay chay gen_testcase truoc!\n";
            return 1;
        }
        int n;
        inFile >> n;
        cout << left << setw(25) << filename << setw(25) << n;
        if (n > MAX_SIZE_TO_RUN) {
            cout << "Qua thoi gian cho (> 5 phut - Bo qua)\n";
            inFile.close();
            continue;
        }

        vector<int> arr(n);
        for (int j = 0; j < n; ++j) {
            inFile >> arr[j];
        }
        inFile.close();

        // Bắt đầu bấm giờ
        auto start = high_resolution_clock::now();
        
        selectionSort(arr);
        
        // Dừng bấm giờ
        auto stop = high_resolution_clock::now();
        duration<double> elapsed = stop - start;

        cout << fixed << setprecision(6) << elapsed.count() << " s\n";
    }

    return 0;
}