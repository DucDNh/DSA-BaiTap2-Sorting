#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void merge_2_Sorted_subArray(vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    int i = left, j = mid + 1;
    vector<int> b;

    while (i <= mid || j <= right) {
        if (i <= mid && (j > right || arr[i] <= arr[j])) {
            b.push_back(arr[i++]);
        } else {
            b.push_back(arr[j++]);
        }
    }
    copy(b.begin(), b.end(), arr.begin() + left);
}

void merge_sort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge_2_Sorted_subArray(arr, left, right);
}

int main() {
    const vector<int> sizes = {
        100, 1000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000,
        100000, 150000, 200000, 250000, 300000,
        500000, 600000, 700000, 800000, 900000, 1000000
    };
    ofstream fout("ket_qua_merge.txt");
    if (!fout) {
        cerr << "Khong mo duoc ket_qua_merge.txt\n";
        return 1;
    }
    const string header = "         n | Merge Sort trung binh (ms)\n";
    cout << header;
    fout << header;

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

        double totalMs = 0;
        for (int t = 0; t < numArrays; ++t) {
            vector<int> arr(n);
            for (int& value : arr) {
                if (!(fin >> value)) {
                    cerr << "Du lieu khong hop le tai mang " << t + 1 << ": " << path << '\n';
                    return 1;
                }
            }

            auto start = chrono::steady_clock::now();
            merge_sort(arr, 0, n - 1);
            auto end = chrono::steady_clock::now();
            totalMs += chrono::duration<double, milli>(end - start).count();

            if (!is_sorted(arr.begin(), arr.end())) {
                cerr << "Sap xep sai tai mang " << t + 1 << ": " << path << '\n';
                return 1;
            }
        }

        double avgMs = totalMs / numArrays;
        for (ostream* out : {&cout, static_cast<ostream*>(&fout)}) {
            *out << setw(10) << n << " | " << fixed << setprecision(3) << avgMs << '\n';
        }
    }
    return 0;
}
