#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <climits>
#include <unordered_map>

using namespace std;

using Edge = pair<int, int>;
using Graph = vector<Edge>;

// Optimized brute-force min-cut calculation
int bruteForceMinCut(const Graph &graph, int n) {
    if (n <= 1) return 0;  // No cut possible for single vertex or empty graph

    int minCut = INT_MAX;

    // Collect all unique vertices
    unordered_set<int> vertices;
    for (const auto &edge : graph) {
        vertices.insert(edge.first);
        vertices.insert(edge.second);
    }

    vector<int> vertexList(vertices.begin(), vertices.end());
    int vertexCount = vertexList.size();
    int subsetSize = 1 << vertexCount;

    // Map vertex to index for faster lookup
    unordered_map<int, int> vertexIndex;
    for (int i = 0; i < vertexCount; ++i) {
        vertexIndex[vertexList[i]] = i;
    }

    // Iterate over all subsets except empty and full set
    for (int mask = 1; mask < subsetSize - 1; ++mask) {
        int cutSize = 0;
        for (const auto &edge : graph) {
            int uIndex = vertexIndex[edge.first];
            int vIndex = vertexIndex[edge.second];

            bool uInSubset = mask & (1 << uIndex);
            bool vInSubset = mask & (1 << vIndex);

            if (uInSubset != vInSubset) {
                ++cutSize;
            }
        }
        minCut = min(minCut, cutSize);
    }

    return minCut;
}


