#ifndef FAST_CUT_HPP
#define FAST_CUT_HPP

#include <vector>
#include <utility>

// Define the graph as a vector of edges
using Edge = std::pair<int, int>;
using Graph = std::vector<Edge>;

// Function to reduce a graph to t vertices using the contract algorithm
Graph reduceGraph(const Graph &graph, int n, int t);

// FastCut algorithm implementation
int fastCut(Graph graph, int n);

#endif // FAST_CUT_HPP
