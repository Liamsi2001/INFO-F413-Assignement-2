#include "fast_cut.hpp"
#include "contract.hpp"
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
        contract(reducedGraph, u, v, adjacency);
    }

    return reducedGraph;
}

// FastCut algorithm implementation
int fastCut(Graph graph, int n) {
    if (n <= 6) {
        // Base case: use the standard contract algorithm
        return contractAlgorithm(graph, n);
    }

    // Compute the threshold t
    int t = static_cast<int>(ceil(1 + n / sqrt(2)));

    // Reduce the graph to t vertices twice
    Graph reducedGraph1 = reduceGraph(graph, n, t);
    Graph reducedGraph2 = reduceGraph(graph, n, t);

    // Recursively apply FastCut to the reduced graphs
    int minCut1 = fastCut(reducedGraph1, t);
    int minCut2 = fastCut(reducedGraph2, t);

    // Return the minimum of the two cuts
    return min(minCut1, minCut2);
}
