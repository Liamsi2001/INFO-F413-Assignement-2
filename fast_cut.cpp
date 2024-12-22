#include "contract.hpp"
#include "brute_force_min_cut.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <future>  // For parallelization

using namespace std;
using namespace std::chrono;

using Edge = pair<int, int>;
using Graph = vector<Edge>;

static std::mt19937 eng(std::random_device{}());

// FastCut with parallelization
int fastCut(const Graph &graph, int n) {
    if (n <= 6) {
        return bruteForceMinCut(graph, n);
    }

    int t = static_cast<int>(ceil(1 + n / sqrt(2)));

    Graph reducedGraph1 = contract(graph, t);
    Graph reducedGraph2 = contract(graph, t);

    int result;

    if (t > 50) {
        auto futureMinCut1 = async(launch::async, fastCut, reducedGraph1, t);
        int minCut2 = fastCut(reducedGraph2, t);
        int minCut1 = futureMinCut1.get();
        result = min(minCut1, minCut2);
    } else {
        int minCut1 = fastCut(reducedGraph1, t);
        int minCut2 = fastCut(reducedGraph2, t);
        result = min(minCut1, minCut2);
    }

    return result;
}
