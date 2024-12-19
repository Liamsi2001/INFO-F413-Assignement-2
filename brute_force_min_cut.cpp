#include "brute_force_min_cut.hpp"
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream> // For debugging
#include <climits>

using namespace std;

// Graph representation
using Edge = pair<int, int>;
using Graph = vector<Edge>;

// Helper function to calculate the cut size
static int calculateCutSize(const Graph &graph, const unordered_set<int> &subset) {
    int cutSize = 0;
    for (const auto &edge : graph) {
        int u = edge.first, v = edge.second;
        if ((subset.count(u) && !subset.count(v)) || (!subset.count(u) && subset.count(v))) {
            cutSize++;
        }
    }
    return cutSize;
}


// Brute force minimum cut algorithm
int bruteForceMinCut(const Graph &graph, int n) {
    int minCut = INT_MAX;

    // Generate all subsets of the vertices
    for (int mask = 1; mask < (1 << n) - 1; ++mask) { // Avoid empty set and full set
        unordered_set<int> subset;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) { // Check if bit i is set
                subset.insert(i);
            }
        }

        // Debug: Print current subset
/*         cout << "Checking subset: ";
        for (int v : subset) {
            cout << v << " ";
        }
        cout << endl; */

        // Calculate the cut size for this subset
        int cutSize = calculateCutSize(graph, subset);

        // Debug: Print cut size
/*         cout << "Cut size: " << cutSize << endl; */

        minCut = min(minCut, cutSize);
    }

    return minCut;
}
