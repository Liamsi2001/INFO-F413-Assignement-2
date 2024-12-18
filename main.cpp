#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <regex>
#include "contract.hpp"
#include "deterministic_min_cut.hpp"

using namespace std;
using namespace std::chrono;

using Edge = pair<int, int>;
using Graph = vector<Edge>;

// Fonction pour lire un graphe depuis un fichier
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

// Fonction pour extraire le nombre de sommets à partir du nom du fichier
int extractNumVertices(const string& filename) {
    regex numRegex(R"_(\d+)_");  // Trouve les nombres dans le nom de fichier
    smatch match;
    if (regex_search(filename, match, numRegex)) {
        return stoi(match.str(0));
    }
    cerr << "Error: Unable to extract number of vertices from file name: " << filename << endl;
    exit(EXIT_FAILURE);
}

// Fonction pour traiter les graphes dans un dossier donné
void processGraphs(const string& graphDirectory, const string& outputFileName) {
    vector<filesystem::directory_entry> graphFiles;
    for (const auto& entry : filesystem::directory_iterator(graphDirectory)) {
        if (entry.path().extension() == ".txt") {
            graphFiles.push_back(entry);
        }
    }

    ofstream outFile(outputFileName);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open " << outputFileName << " for writing" << endl;
        exit(EXIT_FAILURE);
    }

    // Écrire l'en-tête du fichier CSV
    outFile << "Graph File,Minimum Cut,Contract Success Rate,Avg Iteration Time (ms),Theoretical Success Probability\n";

    for (const auto& entry : graphFiles) {
        string filePath = entry.path().string();
        string fileName = entry.path().filename().string();
        cout << "Processing file: " << filePath << endl;

        // Extraire le nombre de sommets depuis le nom du fichier
        int numVertices = extractNumVertices(fileName);

        // Lire le graphe depuis le fichier
        Graph graph = readGraphFromFile(filePath);

        // Exécuter l'algorithme déterministe
        int exactMinCut = stoerWagnerMinCut(graph, numVertices);

        // Exécuter l'algorithme de contraction (Karger) avec une limite de temps
        int successCount = 0;
        int iterations = 0;
        auto startContract = high_resolution_clock::now();
        auto timeLimit = milliseconds(100); // 1 seconde

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

        // Calcul des statistiques
        double successRate = (double)successCount / iterations;
        double avgIterationTime = (double)contractDuration / iterations;
        double theoreticalSuccessProbability = 2.0 / (numVertices * (numVertices - 1));

        // Écrire les résultats dans le fichier CSV
        outFile << fileName << "," << exactMinCut << "," << successRate << "," << avgIterationTime << "," << theoreticalSuccessProbability << "\n";
        cout << "Graph: " << fileName << ", Exact Min Cut: " << exactMinCut << ", Success Rate: " << successRate
             << ", Avg Iteration Time: " << avgIterationTime << " ms, Theoretical Success Probability: "
             << theoreticalSuccessProbability << endl;
    }

    outFile.close();
    cout << "Results saved to " << outputFileName << endl;
}

int main() {
    srand(time(0));

    // Traiter les graphes standards
    processGraphs("graphs", "contract_simple.csv");

    // Traiter les graphes exotiques
    processGraphs("exotic_graphs", "contract_exotic.csv");

    return 0;
}

