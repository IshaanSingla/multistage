#include <iostream>
#include <vector>
#include <climits>
#include <chrono>
#include <cstdlib>
#include <fstream>

using namespace std;
using namespace chrono;

const int INF = INT_MAX;

// FGraph algorithm
void FGraph(int n, int k, vector<vector<int>> &c, vector<int> &p) {
    vector<int> cost(n + 1, INF);
    vector<int> d(n + 1, -1);
    cost[n] = 0;

    for (int j = n - 1; j >= 1; --j) {
        for (int r = 1; r <= n; ++r) {
            if (c[j][r] != INF && cost[j] > c[j][r] + cost[r]) {
                cost[j] = c[j][r] + cost[r];
                d[j] = r;
            }
        }
    }

    p[1] = 1;
    p[k] = n;
    for (int j = 2; j <= k - 1; ++j)
        p[j] = d[p[j - 1]];
}

// Function to generate a random complete multistage graph
void generateCompleteGraph(int n, vector<vector<int>> &c) {
    for (int i = 1; i <= n - 1; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            c[i][j] = rand() % 10 + 1;
        }
    }
}

int main() {
    srand(time(0));
    ofstream outFile("results.txt");
    outFile << "Vertices\tTime(ms)" << endl;

    for (int n = 50; n <= 30000; n += 50) {
        int k = 4;
        long long totalDuration = 0;

        for (int trial = 0; trial < 100; ++trial) {
            vector<vector<int>> c(n + 1, vector<int>(n + 1, INF));
            generateCompleteGraph(n, c);
            vector<int> p(k + 1);

            auto start = high_resolution_clock::now();
            FGraph(n, k, c, p);
            auto end = high_resolution_clock::now();
            totalDuration += duration_cast<microseconds>(end - start).count();
        }

        double averageTime = totalDuration / 100.0;
        outFile << n << "\t\t" << averageTime << endl;
        cout << n << "\t\t" << averageTime << " ms" << endl;
    }

    outFile.close();
    return 0;
}
