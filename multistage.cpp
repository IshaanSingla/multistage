#include <iostream>
#include <vector>
#include <limits>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;
using namespace std::chrono;

const int INF = numeric_limits<int>::max();

void FGraph(int n, int k,
            const vector<vector<int>>& costMatrix,
            const vector<vector<int>>& stages,
            vector<int>& p) {
    vector<int> cost(n + 1, INF);
    vector<int> nextV(n + 1, -1);
    cost[n] = 0;

    for (int stage = k; stage >= 1; --stage) {
        for (int u : stages[stage]) {
            if (u == n) continue;
            int best = INF;
            int bestV = -1;
           
            if (stage < k) {
                for (int v : stages[stage + 1]) {
                    if (costMatrix[u][v] < INF && cost[v] + costMatrix[u][v] < best) {
                        best = cost[v] + costMatrix[u][v];
                        bestV = v;
                    }
                }
            }
            cost[u] = best;
            nextV[u] = bestV;
        }
    }

   
    p.resize(k + 1);
    p[1] = 1;
    for (int stage = 2; stage <= k; ++stage) {
        p[stage] = nextV[p[stage - 1]];
    }
}
void generateMultistageGraph(int n, int k,
                             vector<vector<int>>& costMatrix,
                             vector<vector<int>>& stages) {
    int remaining = n - 2;
    int base = remaining / (k - 2);
    int extra = remaining % (k - 2);

    stages.assign(k + 1, {});
    stages[1].push_back(1);
    stages[k].push_back(n);

    int vid = 2;
    for (int stage = 2; stage <= k - 1; ++stage) {
        int cnt = base + (extra-- > 0 ? 1 : 0);
        for (int i = 0; i < cnt; ++i) {
            stages[stage].push_back(vid++);
        }
    }

    costMatrix.assign(n + 1, vector<int>(n + 1, INF));
    
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(1, 10);

    for (int stage = 1; stage < k; ++stage) {
        for (int u : stages[stage]) {
            for (int v : stages[stage + 1]) {
                costMatrix[u][v] = dist(rng);
            }
        }
    }
}

int main() {
                        
    ofstream outFile("multistage_fixedk.txt");
    outFile << "Vertices\tTime (ms)" << endl;
    

    for (int n = 50; n <= 2000; n += 10) {
        int k = 10;
        long long totalDur = 0;
        vector<vector<int>> costMatrix;
        vector<vector<int>> stages;
        generateMultistageGraph(n, k, costMatrix, stages);
        vector<int> path;
        for (int trial = 0; trial < 100; ++trial) {
            

            auto start = high_resolution_clock::now();
            FGraph(n, k, costMatrix, stages, path);
            auto end = high_resolution_clock::now();
            
            totalDur += duration_cast<microseconds>(end - start).count();
        }

        double avgMs = totalDur / 100.0;
        
        outFile << n << "\t\t" << avgMs << endl;
    }

    outFile.close();
    return 0;
}
