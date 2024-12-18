// contract.cpp
#include "contract.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <algorithm> // For std::remove_if

using namespace std;

// Graph representation
using Edge = pair<int, int>;
using Graph = vector<Edge>;

// Function to contract an edge
void contract(Graph &graph, int u, int v, unordered_map<int, unordered_set<int>> &adjacency) {
    // Merge vertex v into vertex u
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
    graph.erase(remove_if(graph.begin(), graph.end(), [](const Edge &e) { return e.first == e.second; }), graph.end());
}

// Contract algorithm implementation
int contractAlgorithm(Graph graph, int n) {
    unordered_map<int, unordered_set<int>> adjacency;
    for (const auto &edge : graph) {
        adjacency[edge.first].insert(edge.second);
        adjacency[edge.second].insert(edge.first);
    }

    while (adjacency.size() > 2) {
        // Select a random edge
        int randomIndex = rand() % graph.size();
        int u = graph[randomIndex].first;
        int v = graph[randomIndex].second;

        // Contract the selected edge
        contract(graph, u, v, adjacency);
    }

    // Return the size of the cut
    return graph.size();
}