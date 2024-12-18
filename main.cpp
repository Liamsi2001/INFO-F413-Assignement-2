#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "contract.hpp"
#include "deterministic_min_cut.hpp"

using namespace std;
using namespace std::chrono;

using Edge = pair<int, int>;
using Graph = vector<Edge>;

// Function to read a graph from a file
Graph readGraphFromFile(const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(EXIT_FAILURE);
    }

    Graph graph;
    int u, v;
    while (infile >> u >> v) {
        graph.emplace_back(u, v);
    }

    infile.close();
    return graph;
}

void processGraphs(const string& graphDirectory, const string& outputFileName) {
    vector<string> graphFiles;
    for (const auto& entry : filesystem::directory_iterator(graphDirectory)) {
        if (entry.path().extension() == ".txt") {
            graphFiles.push_back(entry.path().filename().string());
        }
    }

    ofstream outFile(outputFileName);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open " << outputFileName << " for writing" << endl;
        exit(EXIT_FAILURE);
    }

    // Write CSV header
    outFile << "Graph File,Minimum Cut,Contract Success Rate,Avg Iteration Time (ms),Theoretical Success Probability\n";

    for (const auto& file : graphFiles) {
        string filePath = graphDirectory + "/" + file;
        cout << "Processing file: " << filePath << endl;

        // Read the graph from the file
        Graph graph = readGraphFromFile(filePath);

        // Determine the number of vertices
        int maxVertex = 0;
        for (const auto& edge : graph) {
            maxVertex = max(maxVertex, max(edge.first, edge.second));
        }
        int numVertices = maxVertex + 1; // Vertices are assumed to start from 0

        // Run deterministic algorithm
        int exactMinCut = stoerWagnerMinCut(graph, numVertices);

        // Run Contract algorithm with a time limit
        int successCount = 0;
        int iterations = 0;
        auto startContract = high_resolution_clock::now();
        auto timeLimit = milliseconds(10000); // 1-second time limit

        while (true) {
            int contractMinCut = contractAlgorithm(graph, numVertices);
            if (contractMinCut == exactMinCut) {
                successCount++;
            }
            iterations++;

            auto now = high_resolution_clock::now();
            if (now - startContract > timeLimit) {
                break;
            }
        }

        auto endContract = high_resolution_clock::now();
        auto contractDuration = duration_cast<milliseconds>(endContract - startContract).count();

        double successRate = (double)successCount / iterations;
        double avgIterationTime = (double)contractDuration / iterations;
        double theoreticalSuccessProbability = 2.0 / (numVertices * (numVertices - 1));

        // Write results to CSV
        outFile << file << "," << exactMinCut << "," << successRate << "," << avgIterationTime << "," << theoreticalSuccessProbability << "\n";
        cout << "Graph: " << file << ", Exact Min Cut: " << exactMinCut << ", Success Rate: " << successRate
             << ", Avg Iteration Time: " << avgIterationTime << " ms, Theoretical Success Probability: "
             << theoreticalSuccessProbability << endl;
    }

    outFile.close();
    cout << "Results saved to " << outputFileName << endl;
}

int main() {
    srand(time(0));

    // Process standard graphs
    processGraphs("graphs", "contract_simple.csv");

    // Process exotic graphs
    processGraphs("exotic_graphs", "contract_exotic.csv");

    return 0;
}
