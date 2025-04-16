#include <iostream>
#include <vector>
#include <climits>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <limits>

using namespace std;
using namespace chrono;

// Function to compute the matrix chain multiplication cost using DP.
long long matrixChainOrder(const vector<int>& p) {
    int n = p.size() - 1; // number of matrices
    // m[i][j] stores minimum scalar multiplication cost for matrices i through j
    vector<vector<long long>> m(n + 1, vector<long long>(n + 1, 0));
    
    // m[i][i] = 0 for all i (single matrix multiplication cost is zero)
    // l is the chain length.
    for (int l = 2; l <= n; l++) {
        for (int i = 1; i <= n - l + 1; i++) {
            int j = i + l - 1;
            m[i][j] = numeric_limits<long long>::max();
            for (int k = i; k < j; k++) {
                long long q = m[i][k] + m[k+1][j] + static_cast<long long>(p[i-1]) * p[k] * p[j];
                if(q < m[i][j])
                    m[i][j] = q;
            }
        }
    }
    
    return m[1][n];
}

int main() {
    // Create output file to store results
    ofstream outFile("chain_multiplication_results.txt");
    if(!outFile) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }
    
    outFile << "Vertices\tTime(ms)" << endl;
    cout << "Vertices\tTime(ms)" << endl;
    
    // We'll vary n (number of matrices) from a small value up to some max.
    // Each "n" means there are n matrices and (n+1) dimensions in array p.
    // WARNING: Matrix chain multiplication is O(n^3); choose n accordingly.
    for (int n = 50; n <= 500; n += 10) {
        int iterations = 10;
        long long totalDuration = 0;
        
        for (int trial = 0; trial < iterations; trial++) {
            // Create a random dimension array p of length n+1.
            // We assume matrix dimensions between 10 and 100.
            vector<int> p(n + 1);
            for (int i = 0; i < n + 1; i++) {
                p[i] = rand() % 91 + 10; // random int in [10, 100]
            }
            
            auto start = high_resolution_clock::now();
            long long cost = matrixChainOrder(p);
            auto end = high_resolution_clock::now();
            long long duration = duration_cast<microseconds>(end - start).count();
            totalDuration += duration;
            
            // Optionally, print the cost (not required for time complexity)
            // cout << "Cost: " << cost << endl;
        }
        
        double avgTime = totalDuration / static_cast<double>(iterations);
        outFile << n << "\t\t" << avgTime << endl;
        cout << n << "\t\t" << avgTime << " ms" << endl;
    }
    
    outFile.close();
    return 0;
}
