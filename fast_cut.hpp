#ifndef FAST_CUT_HPP
#define FAST_CUT_HPP

#include <vector>
#include <utility>

// Define the graph as a vector of edges
using Edge = std::pair<int, int>;
using Graph = std::vector<Edge>;


// FastCut algorithm implementation
int fastCut(const Graph &graph, int n);

#endif // FAST_CUT_HPP
