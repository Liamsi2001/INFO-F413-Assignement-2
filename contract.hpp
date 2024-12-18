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
void contract(Graph &graph, int u, int v, std::unordered_map<int, std::unordered_set<int>> &adjacency);
int contractAlgorithm(Graph graph, int n);

#endif // CONTRACT_HPP
