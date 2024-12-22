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

    for (const auto &edge : graph) {
        adjacency[edge.first].insert(edge.second);
        adjacency[edge.second].insert(edge.first);
    }

    while ((int)adjacency.size() > targetSize) {
        std::uniform_int_distribution<size_t> distr(0, graph.size() - 1);
        size_t randomIndex = distr(eng);
        int u = graph[randomIndex].first;
        int v = graph[randomIndex].second;

        for (int neighbor : adjacency[v]) {
            if (neighbor != u) {
                adjacency[u].insert(neighbor);
                adjacency[neighbor].erase(v);
                adjacency[neighbor].insert(u);
            }
        }
        adjacency[u].erase(v);
        adjacency.erase(v);

        for (auto &edge : graph) {
            if (edge.first == v) edge.first = u;
            if (edge.second == v) edge.second = u;
        }

        graph.erase(remove_if(graph.begin(), graph.end(), [](const Edge &e) {
            return e.first == e.second;
        }), graph.end());
    }
    return graph;
}

int contractAlgorithm(const Graph &graph, int n) {
    (void)n;  // Silence unused parameter warning
    Graph workingGraph = graph;
    workingGraph = contract(workingGraph, 2);
    return static_cast<int>(workingGraph.size());
}
