#ifndef BRUTE_FORCE_MIN_CUT_HPP
#define BRUTE_FORCE_MIN_CUT_HPP

#include <vector>
using namespace std;

using Edge = pair<int, int>;
using Graph = vector<Edge>;

int bruteForceMinCut(const Graph &graph, int n);

#endif // BRUTE_FORCE_MIN_CUT_HPP

