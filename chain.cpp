#include <iostream>
#include <vector>
#include <limits>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;
using namespace std::chrono;


long long matrixChainOrder(const vector<int>& dims) {
    int n = dims.size() - 1;            
    vector<vector<long long>> cost(n, vector<long long>(n, 0));

    
    for (int l = 2; l <= n; ++l) {
        for (int i = 0; i + l - 1 < n; ++i) {
            int j = i + l - 1;
            cost[i][j] = numeric_limits<long long>::max();
            
            // try splitting between k and k+1
            for (int k = i; k < j; ++k) {
                long long q = cost[i][k] + cost[k+1][j]
                              + (long long)dims[i] * dims[k+1] * dims[j+1];
                if (q < cost[i][j]) {
                    cost[i][j] = q;
                }
            }
        }
    }
    return cost[0][n-1];
}

int main() {
   
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(10, 100);

   
    ofstream outFile("chain_multiplication_results.txt");
    if (!outFile) {
        cerr << "Error: cannot open output file." << endl;
        return 1;
    }

 
    
    outFile << "Matrices\tAvg Time (ms)" << endl;

    
    for (int n = 50; n <= 500; n += 10) {
        int trials = 10;
        double totalTime = 0;
        vector<int> dims(n + 1);
        for (int i = 0; i <= n; ++i)
            {dims[i] = dist(rng);}
        for (int t = 0; t < trials; ++t) {
            

            auto start = high_resolution_clock::now();
            long long minCost = matrixChainOrder(dims);
            auto end = high_resolution_clock::now();

            double ms = duration_cast<duration<double, milli>>(end - start).count();
            totalTime += ms;
        }

        double avgTime = totalTime / trials;
        
        
        outFile << n << "\t\t" << avgTime << endl;
    }

    outFile.close();
    return 0;
}