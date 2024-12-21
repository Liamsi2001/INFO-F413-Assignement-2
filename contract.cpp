#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include <map>

using namespace std;
using namespace std::chrono;

using Edge = pair<int, int>;
using Graph = vector<Edge>;

static std::mt19937 eng(std::random_device{}());

// Function to contract an edge in the graph
Graph contract(Graph graph, int targetSize) {
    unordered_map<int, unordered_set<int>> adjacency;

    // Build adjacency list
    for (const auto &edge : graph) {
        adjacency[edge.first].insert(edge.second);
        adjacency[edge.second].insert(edge.first);
    }

    // Contract until target size is reached
    while ((int)adjacency.size() > targetSize) {
        std::uniform_int_distribution<> distr(0, (int)graph.size() - 1);
        int randomIndex = distr(eng);
        int u = graph[randomIndex].first;
        int v = graph[randomIndex].second;

        // Perform contraction
        for (int neighbor : adjacency[v]) {
            if (neighbor != u) {
                adjacency[u].insert(neighbor);
                adjacency[neighbor].erase(v);
                adjacency[neighbor].insert(u);
            }
        }
        adjacency[u].erase(v);
        adjacency.erase(v);

        // Update graph edges
        for (auto &edge : graph) {
            if (edge.first == v) edge.first = u;
            if (edge.second == v) edge.second = u;
        }

        // Remove self-loops
        graph.erase(remove_if(graph.begin(), graph.end(), [](const Edge &e) {
            return e.first == e.second;
        }), graph.end());
    }
    
    return graph;
}


// Contract Algorithm Implementation
int contractAlgorithm(const Graph &graph, int n) {
    Graph workingGraph = graph;

    // Contract graph directly to size 2
    workingGraph = contract(workingGraph, 2);

    // Count remaining edges between the two supernodes
    return workingGraph.size();
}
