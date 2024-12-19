// contract.cpp
#include "contract.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <algorithm> // For std::remove_if
#include <random> // For std::mt19937 and std::uniform_int_distribution

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

int contractAlgorithm(Graph graph, int n) {
    unordered_map<int, unordered_set<int>> adjacency;
    for (const auto &edge : graph) {
        adjacency[edge.first].insert(edge.second);
        adjacency[edge.second].insert(edge.first);
    }

    // Initialize random number generator
    std::random_device rd;
    std::mt19937 eng(rd()); // Mersenne Twister RNG

    while (adjacency.size() > 2) {
        // Generate a random edge index
        std::uniform_int_distribution<> distr(0, graph.size() - 1);
        int randomIndex = distr(eng);

        int u = graph[randomIndex].first;
        int v = graph[randomIndex].second;

        // Contract the selected edge
        contract(graph, u, v, adjacency);
    }

    // Return the size of the cut
    return graph.size();
}