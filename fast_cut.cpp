#include "fast_cut.hpp"
#include "contract.hpp"
#include "brute_force_min_cut.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <algorithm> // For std::remove_if

using namespace std;

// Graph representation
using Edge = pair<int, int>;
using Graph = vector<Edge>;

// Function to reduce a graph to t vertices using the contract algorithm
Graph reduceGraph(const Graph &graph, int n, int t) {
    Graph reducedGraph = graph;
    unordered_map<int, unordered_set<int>> adjacency;

    // Build adjacency list
    for (const auto &edge : reducedGraph) {
        adjacency[edge.first].insert(edge.second);
        adjacency[edge.second].insert(edge.first);
    }

    while (adjacency.size() > t) {
        // Select a random edge
        int randomIndex = rand() % reducedGraph.size();
        int u = reducedGraph[randomIndex].first;
        int v = reducedGraph[randomIndex].second;

        // Contract the selected edge
        if (adjacency[u].empty() || adjacency[v].empty()) {
            continue; // Skip invalid contractions
        }
        contract(reducedGraph, u, v, adjacency);

        // Ensure graph validity
        reducedGraph.erase(remove_if(reducedGraph.begin(), reducedGraph.end(),
            [](const Edge &e) { return e.first == e.second; }),
            reducedGraph.end());
    }

    if (adjacency.size() < t) {
        cerr << "Error: Graph reduced below target size t=" << t << endl;
        exit(EXIT_FAILURE);
    }

    return reducedGraph;
}

static Graph reindexGraph(const Graph &graph, unordered_map<int, int> &newIndexMap) {
    Graph reindexedGraph;
    int index = 0;
    for (const auto &edge : graph) {
        int u = edge.first;
        int v = edge.second;

        // Assign new indices if not already assigned
        if (newIndexMap.find(u) == newIndexMap.end()) {
            newIndexMap[u] = index++;
        }
        if (newIndexMap.find(v) == newIndexMap.end()) {
            newIndexMap[v] = index++;
        }

        // Add the edge with reindexed vertices
        reindexedGraph.emplace_back(newIndexMap[u], newIndexMap[v]);
    }
    return reindexedGraph;
}



// FastCut algorithm implementation
int fastCut(Graph graph, int n) {
    if (n <= 6) {
        /* cout << "Base case reached with n = " << n << " and graph size = " << graph.size() << endl; */

        // Reindex the graph
        unordered_map<int, int> newIndexMap;
        Graph reindexedGraph = reindexGraph(graph, newIndexMap);

        // Debug: Print reindexed graph
        /* cout << "Reindexed graph:" << endl;
        for (const auto &edge : reindexedGraph) {
            cout << "Edge: " << edge.first << " - " << edge.second << endl;
        } */

        return bruteForceMinCut(reindexedGraph, newIndexMap.size());
    }

    // Compute the threshold t
    int t = static_cast<int>(ceil(1 + n / sqrt(2)));

    // Reduce the graph to t vertices twice
    Graph reducedGraph1 = reduceGraph(graph, n, t);
/*     cout << "Reduced graph 1 has " << reducedGraph1.size() << " edges after reduction." << endl; */

    Graph reducedGraph2 = reduceGraph(graph, n, t);
/*     cout << "Reduced graph 2 has " << reducedGraph2.size() << " edges after reduction." << endl; */


    // Recursively apply FastCut to the reduced graphs
    int minCut1 = fastCut(reducedGraph1, t);
    int minCut2 = fastCut(reducedGraph2, t);

/*     cout << "MinCut1: " << minCut1 << ", MinCut2: " << minCut2 << endl; */


    // Return the minimum of the two cuts
    return min(minCut1, minCut2);
}