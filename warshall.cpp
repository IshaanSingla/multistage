#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>
using namespace std;
using namespace chrono;

const double INF = 1e9;


void generateGraph(int n, vector<vector<double>>& g, double p, double w_min, double w_max) {
    g.assign(n, vector<double>(n, INF));
    for (int i = 0; i < n; i++)
        g[i][i] = 0;  // zero distance to self

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double r = (double) rand() / RAND_MAX;
            if (r < p) {
                double w = w_min + ((double) rand() / RAND_MAX) * (w_max - w_min);
                g[i][j] = w;
                g[j][i] = w;
            }
        }
    }
}

void floydWarshall(int n, vector<vector<double>>& dist) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

int main() {
   
    srand(time(NULL));

    int startV = 50;
    int endV = 400;
    int step = 10;
    int iter = 10;
    double p = 0.5, w_min = 1, w_max = 10;

    

    ofstream outFile("results.txt");
    outFile << "Vertices\tTime(ms)" << endl;

    for (int n = startV; n <= endV; n += step) {
        double totalTime = 0;
        vector<vector<double>> graph;
        generateGraph(n, graph, p, w_min, w_max);
        for (int t = 0; t < iter; t++) {
            

            
            auto start = high_resolution_clock::now();
            floydWarshall(n, graph);
            auto end = high_resolution_clock::now();

            double ms = duration_cast<milliseconds>(end - start).count();
            totalTime += ms;
        }

        double avg = totalTime / iter;
        outFile << n << "\t" << avg << endl;
    }

    outFile.close();
    return 0;
}
