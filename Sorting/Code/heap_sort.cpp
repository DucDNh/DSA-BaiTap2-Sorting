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
        vector<int> arr(n);
        for (int j = 0; j < n; ++j) {
            inFile >> arr[j];
        }
        inFile.close();

        // Bắt đầu bấm giờ
        auto start = high_resolution_clock::now();
        
        heapSort(arr);
        
        // Dừng bấm giờ
        auto stop = high_resolution_clock::now();
        duration<double> elapsed = stop - start;

        cout << left << setw(25) << filename 
             << setw(25) << n 
             << fixed << setprecision(6) << elapsed.count() << " s\n";
    }

    return 0;
}