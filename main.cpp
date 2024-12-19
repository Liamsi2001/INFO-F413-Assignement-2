#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
#include <chrono>
#include <math.h>
#include "contract.hpp"
#include "fast_cut.hpp"
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

// Function to extract the number of vertices from the file name
int extractNumVertices(const string& filename) {
    regex numRegex(R"_(\d+)_");  // Extracts numbers in the file name
    smatch match;
    if (regex_search(filename, match, numRegex)) {
        return stoi(match.str(0));
    }
    cerr << "Error: Unable to extract number of vertices from file name: " << filename << endl;
    exit(EXIT_FAILURE);
}

// Function to process graphs in a given directory
void processGraphs(const string& graphDirectory, const string& contractOutputFile, const string& fastCutOutputFile) {
    vector<filesystem::directory_entry> graphFiles;
    for (const auto& entry : filesystem::directory_iterator(graphDirectory)) {
        if (entry.path().extension() == ".txt") {
            graphFiles.push_back(entry);
        }
    }

    ofstream contractOut(contractOutputFile);
    ofstream fastCutOut(fastCutOutputFile);

    if (!contractOut.is_open() || !fastCutOut.is_open()) {
        cerr << "Error: Unable to open output files for writing" << endl;
        exit(EXIT_FAILURE);
    }

    // Write CSV headers
    contractOut << "Graph File,Minimum Cut,Success Rate,Avg Iteration Time (ms),Theoretical Success Probability,Iterations\n";
    fastCutOut << "Graph File,Minimum Cut,Success Rate,Avg Iteration Time (ms),Theoretical Success Probability,Iterations\n";

    for (const auto& entry : graphFiles) {
        string filePath = entry.path().string();
        string fileName = entry.path().filename().string();
        cout << "Processing file: " << filePath << endl;

        int numVertices = extractNumVertices(fileName);
        double contractTheoreticalSuccessProbability = 2.0 / (numVertices * (numVertices - 1));
        double fastCutTheoreticalProbability = 1 / log2(numVertices); // FastCut theoretical probability

        // Read the graph from the file
        Graph graph = readGraphFromFile(filePath);

        // Deterministic Min-Cut
        int exactMinCut = stoerWagnerMinCut(graph, numVertices);

        // Contract Algorithm
        int contractSuccessCount = 0;
        int contractIterations = 0;
        auto contractStart = high_resolution_clock::now();
        auto timeLimit = milliseconds(100);

        while (true) {
            int contractMinCut = contractAlgorithm(graph, numVertices);
            if (contractMinCut == exactMinCut) {
                contractSuccessCount++;
            }
            contractIterations++;

            auto now = high_resolution_clock::now();
            if (now - contractStart > timeLimit) {
                break;
            }
        }

        auto contractEnd = high_resolution_clock::now();
        double contractDuration = duration_cast<microseconds>(contractEnd - contractStart).count();
        double contractSuccessRate = (double)contractSuccessCount / contractIterations;
        double contractAvgTime = (contractDuration / 1000.0) / contractIterations; // Convert to milliseconds

        contractOut << fileName << "," << exactMinCut << "," << contractSuccessRate << "," << contractAvgTime << "," << contractTheoreticalSuccessProbability << "," << contractIterations << "\n";

        // FastCut Algorithm
        int fastCutSuccessCount = 0;
        int fastCutIterations = 0;
        auto fastCutStart = high_resolution_clock::now();

        while (true) {
            int fastCutMinCut = fastCut(graph, numVertices);
            if (fastCutMinCut == exactMinCut) {
                fastCutSuccessCount++;
            }
            fastCutIterations++;

            auto now = high_resolution_clock::now();
            if (now - fastCutStart > timeLimit) {
                break;
            }
        }

        auto fastCutEnd = high_resolution_clock::now();
        double fastCutDuration = duration_cast<microseconds>(fastCutEnd - fastCutStart).count();
        double fastCutSuccessRate = (double)fastCutSuccessCount / fastCutIterations;
        double fastCutAvgTime = (fastCutDuration / 1000.0) / fastCutIterations; // Convert to milliseconds

        fastCutOut << fileName << "," << exactMinCut << "," << fastCutSuccessRate << "," << fastCutAvgTime << "," << fastCutTheoreticalProbability << "," << fastCutIterations << "\n";

        cout << "Graph: " << fileName << ", Min Cut: " << exactMinCut
             << ", Contract Success Rate: " << contractSuccessRate
             << ", FastCut Success Rate: " << fastCutSuccessRate
             << endl;
    }

    contractOut.close();
    fastCutOut.close();
    cout << "Results saved to " << contractOutputFile << " and " << fastCutOutputFile << endl;
}

int main() {
    srand(time(0));

    // Process standard graphs
    processGraphs("graphs", "contract_simple.csv", "fast_simple.csv");

    // Process exotic graphs
    processGraphs("exotic_graphs", "contract_exotic.csv", "fast_exotic.csv");

    return 0;
}
