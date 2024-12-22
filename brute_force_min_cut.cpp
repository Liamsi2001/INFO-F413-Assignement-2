#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <climits>
#include <unordered_map>

using namespace std;

using Edge = pair<int, int>;
using Graph = vector<Edge>;


// Calculate the minimum cut in a graph using brute force
int bruteForceMinCut(const Graph &graph, int n) {
    if (n <= 1) return 0;

    int minCut = INT_MAX;

    unordered_set<int> vertices;
    for (const auto &edge : graph) {
        vertices.insert(edge.first);
        vertices.insert(edge.second);
    }

    vector<int> vertexList(vertices.begin(), vertices.end());
    size_t vertexCount = vertexList.size();
    int subsetSize = 1 << vertexCount;

    unordered_map<int, size_t> vertexIndex;
    for (size_t i = 0; i < vertexCount; ++i) {
        vertexIndex[vertexList[i]] = i;
    }

    // Construct the adjacency matrix
    vector<vector<int>> adjacency(vertexCount, vector<int>(vertexCount, 0));
    for (const auto &edge : graph) {
        size_t u = vertexIndex[edge.first];
        size_t v = vertexIndex[edge.second];
        adjacency[u][v]++;
        adjacency[v][u]++;
    }

    // Try all possible subsets of vertices
    for (int mask = 1; mask < (subsetSize / 2); ++mask) {  // Avoid duplicates by iterating only half of the subsets
        int cutSize = 0;

        for (size_t u = 0; u < vertexCount; ++u) {
            for (size_t v = u + 1; v < vertexCount; ++v) {
                bool uInSubset = mask & (1 << u);
                bool vInSubset = mask & (1 << v);

                if (uInSubset != vInSubset) {
                    cutSize += adjacency[u][v];
                }
            }
        }

        minCut = min(minCut, cutSize);
    }

    return minCut;
}

