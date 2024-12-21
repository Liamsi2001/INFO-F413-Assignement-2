// contract.hpp
#ifndef CONTRACT_HPP
#define CONTRACT_HPP

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using Edge = std::pair<int, int>;
using Graph = std::vector<Edge>;

// Function declarations
Graph contract(Graph graph, int targetSize);
int contractAlgorithm(const Graph &graph, int n);

#endif // CONTRACT_HPP
