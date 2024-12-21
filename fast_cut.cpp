#include "contract.hpp"
#include "brute_force_min_cut.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <future>  // Pour l'exécution parallèle
#include <mutex>   // Pour protéger le cache en multithread

using namespace std;
using namespace std::chrono;

using Edge = pair<int, int>;
using Graph = vector<Edge>;

static std::mt19937 eng(std::random_device{}());

// Cache pour la mémoïsation
unordered_map<string, int> memo;
mutex cacheMutex;  // Mutex pour protéger l'accès concurrent au cache

// Fonction pour hasher un graphe (permet de le mémoriser efficacement)
string graphHash(const Graph &graph) {
    vector<pair<int, int>> edges = graph;
    sort(edges.begin(), edges.end());
    string hash;
    for (const auto &e : edges) {
        hash += to_string(e.first) + "," + to_string(e.second) + ";";
    }
    return hash;
}

// FastCut avec mémoïsation et parallélisation conditionnelle
int fastCut(const Graph &graph, int n) {
    // Base case : si le graphe est petit, on utilise la méthode brute-force
    if (n <= 6) {
        return bruteForceMinCut(graph, n);
    }

    // Vérification du cache pour éviter des calculs redondants
    string hash = graphHash(graph);
    {
        lock_guard<mutex> lock(cacheMutex);
        if (memo.find(hash) != memo.end()) {
            return memo[hash];
        }
    }

    // Calcul de la taille cible pour la contraction (Karger-Stein)
    int t = ceil(1 + n / sqrt(2));

    // Créer deux graphes réduits de manière indépendante
    Graph reducedGraph1 = contract(graph, t);
    Graph reducedGraph2 = contract(graph, t);

    int result;

    // Parallélisation conditionnelle pour les grands graphes
    if (n > 10) {
        // Lancer les deux réductions en parallèle
        auto futureMinCut1 = async(launch::async, fastCut, reducedGraph1, t);
        int minCut2 = fastCut(reducedGraph2, t);
        int minCut1 = futureMinCut1.get();
        result = min(minCut1, minCut2);
    } else {
        // Exécution séquentielle pour les petits graphes
        int minCut1 = fastCut(reducedGraph1, t);
        int minCut2 = fastCut(reducedGraph2, t);
        result = min(minCut1, minCut2);
    }

    // Stocker le résultat dans le cache pour éviter de recalculer
    {
        lock_guard<mutex> lock(cacheMutex);
        memo[hash] = result;
    }

    return result;
}
