#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
#endif

using namespace std;

void createFolder(const string& folderName) {
#ifdef _WIN32
    _mkdir(folderName.c_str());
#else
    mkdir(folderName.c_str(), 0777);
#endif
}

int main() {
    createFolder("testcase");

    // 21 kich thuoc mang 1 chieu theo dung de bai DSA - Sorting_2.pdf
    vector<int> sizes = {
        100, 1000, 10000, 20000, 30000, 40000, 50000,
        60000, 70000, 80000, 100000, 150000, 200000,
        250000, 300000, 500000, 600000, 700000, 800000,
        900000, 1000000
    };

    mt19937 rng(42);
    uniform_int_distribution<int> dist(-1000000000, 1000000000);

    cout << "Dang tao 21 file testcase ngau nhien trong thu muc 'testcase/'...\n\n";

    for (size_t idx = 0; idx < sizes.size(); idx++) {
        int n = sizes[idx];
        int testNum = (int)idx + 1;

        ostringstream oss;
        oss << "testcase/test_" << setw(2) << setfill('0') << testNum
            << "_" << n << ".inp";
        string filepath = oss.str();

        ofstream out(filepath);
        if (!out.is_open()) {
            cerr << "Loi: Khong the tao file " << filepath << "\n";
            return 1;
        }

        out << n << '\n';
        for (int i = 0; i < n; i++) {
            out << dist(rng) << (i + 1 < n ? ' ' : '\n');
        }
        out.close();

        cout << "[" << setw(2) << setfill('0') << testNum << "/21] Da tao: "
             << filepath << " (" << n << " phan tu)\n";
    }

    cout << "\nHoan tat! Tat ca 21 file testcase da san sang trong thu muc 'testcase/'.\n";
    return 0;
}
