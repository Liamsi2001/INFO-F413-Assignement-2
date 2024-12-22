// deterministic_min_cut.cpp
#include "deterministic_min_cut.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <unordered_map>
using namespace std;

using Edge = pair<int, int>;
using Graph = vector<Edge>;

// Function to compute the minimum cut using the Stoer-Wagner algorithm
int stoerWagnerMinCut(const Graph &graph, int n) {
    vector<vector<int>> capacity(static_cast<size_t>(n), vector<int>(static_cast<size_t>(n), 0));
    for (const auto &edge : graph) {
        capacity[static_cast<size_t>(edge.first)][static_cast<size_t>(edge.second)]++;
        capacity[static_cast<size_t>(edge.second)][static_cast<size_t>(edge.first)]++;
    }

    vector<int> vertices(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) vertices[static_cast<size_t>(i)] = i;

    int minCut = INT_MAX;

    while (vertices.size() > 1) {
        vector<int> weights(static_cast<size_t>(n), 0);
        vector<bool> inA(static_cast<size_t>(n), false);

        int prev = -1, last = -1;
        for (size_t i = 0; i < vertices.size(); ++i) {
            int maxWeight = -1;
            for (int v : vertices) {
                if (!inA[static_cast<size_t>(v)] && weights[static_cast<size_t>(v)] > maxWeight) {
                    maxWeight = weights[static_cast<size_t>(v)];
                    last = v;
                }
            }

            inA[static_cast<size_t>(last)] = true;

            if (i == vertices.size() - 1) {
                minCut = min(minCut, weights[static_cast<size_t>(last)]);

                for (int v : vertices) {
                    capacity[static_cast<size_t>(prev)][static_cast<size_t>(v)] +=
                        capacity[static_cast<size_t>(last)][static_cast<size_t>(v)];
                    capacity[static_cast<size_t>(v)][static_cast<size_t>(prev)] +=
                        capacity[static_cast<size_t>(v)][static_cast<size_t>(last)];
                }

                vertices.erase(remove(vertices.begin(), vertices.end(), last), vertices.end());
            }

            prev = last;

            for (int v : vertices) {
                if (!inA[static_cast<size_t>(v)]) {
                    weights[static_cast<size_t>(v)] +=
                        capacity[static_cast<size_t>(last)][static_cast<size_t>(v)];
                }
            }
        }
    }

    return minCut;
}