// deterministic_min_cut.cpp
#include "deterministic_min_cut.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

using Edge = pair<int, int>;
using Graph = vector<Edge>;

// Function to compute the minimum cut using the Stoer-Wagner algorithm
int stoerWagnerMinCut(const Graph &graph, int n) {
    // Create adjacency matrix
    vector<vector<int>> capacity(n, vector<int>(n, 0));
    for (const auto &edge : graph) {
        capacity[edge.first][edge.second]++;
        capacity[edge.second][edge.first]++;
    }

    vector<int> vertices(n);
    for (int i = 0; i < n; ++i) vertices[i] = i;

    int minCut = INT_MAX;

    while (vertices.size() > 1) {
        vector<int> weights(n, 0);
        vector<bool> inA(n, false);

        int prev = -1, last = -1;
        for (int i = 0; i < vertices.size(); ++i) {
            // Select the most tightly connected vertex
            int maxWeight = -1;
            for (int v : vertices) {
                if (!inA[v] && weights[v] > maxWeight) {
                    maxWeight = weights[v];
                    last = v;
                }
            }

            // Add the vertex to set A
            inA[last] = true;

            if (i == vertices.size() - 1) {
                // Calculate the cut value
                minCut = min(minCut, weights[last]);

                // Merge the last two vertices
                for (int v : vertices) {
                    capacity[prev][v] += capacity[last][v];
                    capacity[v][prev] += capacity[v][last];
                }

                vertices.erase(remove(vertices.begin(), vertices.end(), last), vertices.end());
            }

            prev = last;

            // Update the weights
            for (int v : vertices) {
                if (!inA[v]) {
                    weights[v] += capacity[last][v];
                }
            }
        }
    }

    return minCut;
}