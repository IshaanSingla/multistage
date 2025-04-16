#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <ctime>

using namespace std;
using namespace chrono;

// Warshall's algorithm to compute the transitive closure.
void warshall(vector<vector<int>> &graph) {
    int n = graph.size();
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // If there is a path from i to k and from k to j, then i can reach j.
                graph[i][j] = graph[i][j] || (graph[i][k] && graph[k][j]);
            }
        }
    }
}

// Function to generate a random directed graph with n vertices.
// Each off-diagonal entry is 1 with probability "edgeProb", and 0 otherwise.
// The diagonal is set to 1, since every vertex is reachable from itself.
void generateRandomGraph(int n, vector<vector<int>> &graph, int edgeProb = 30) {
    // edgeProb is the percent probability (e.g., 30 means 30% chance)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                graph[i][j] = 1;    // Ensure self-reachability
            else
                graph[i][j] = (rand() % 100 < edgeProb) ? 1 : 0;
        }
    }
}

int main() {
    // Seed the random number generator.
    srand(static_cast<unsigned int>(time(0)));

    // Create output file to store results.
    ofstream outFile("warshall_results.txt");
    if (!outFile) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }
    outFile << "Vertices\tTime(ms)" << endl;
    cout << "Vertices\tTime(ms)" << endl;

    // Experiment parameters:
    int iterations = 10;         // Number of trials per vertex count.
    int startVertices = 50;      // Starting number of vertices.
    int endVertices = 500;       // Ending number of vertices.
    int step = 10;               // Increment step.

    // For each problem size (number of vertices)
    for (int n = startVertices; n <= endVertices; n += step) {
        long long totalDuration = 0;
        for (int trial = 0; trial < iterations; ++trial) {
            // Create a graph: n x n matrix.
            vector<vector<int>> graph(n, vector<int>(n, 0));
            generateRandomGraph(n, graph);

            // Measure execution time of Warshall's algorithm.
            auto startTime = high_resolution_clock::now();
            warshall(graph);
            auto endTime = high_resolution_clock::now();
            
            long long duration = duration_cast<milliseconds>(endTime - startTime).count();
            totalDuration += duration;
        }
        
        // Compute average time over iterations.
        double avgTime = totalDuration / static_cast<double>(iterations);
        outFile << n << "\t\t" << avgTime << endl;
        cout << n << "\t\t" << avgTime << " ms" << endl;
    }

    outFile.close();
    return 0;
}
