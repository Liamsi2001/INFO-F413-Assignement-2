// deterministic_min_cut.hpp
#ifndef DETERMINISTIC_MIN_CUT_HPP
#define DETERMINISTIC_MIN_CUT_HPP

#include <vector>
#include <utility>
#include <climits>
#include <algorithm>

using Edge = std::pair<int, int>;
using Graph = std::vector<Edge>;

// Function declaration for Stoer-Wagner Minimum Cut Algorithm
int stoerWagnerMinCut(const Graph &graph, int n);

#endif // DETERMINISTIC_MIN_CUT_HPP
