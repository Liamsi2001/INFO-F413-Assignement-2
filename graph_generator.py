import networkx as nx
import os
import random

# Seed globale pour la reproductibilité
SEED = 42
random.seed(SEED)

def generate_and_save_graphs():
    # Répertoire pour sauvegarder les graphes standards
    if not os.path.exists("graphs"):
        os.makedirs("graphs")

    graph_types = [
        ("complete", lambda n: nx.complete_graph(n)),
        ("cycle_with_chords", lambda n: add_chords_to_cycle(nx.cycle_graph(n))),
        ("path", lambda n: nx.path_graph(n)),
        ("grid", lambda n: nx.convert_node_labels_to_integers(nx.grid_2d_graph(int(n**0.5), int(n**0.5))))
    ]

    sizes = list(range(50, 501, 50))  # Tailles de 50 à 500, par pas de 50

    for graph_type, generator in graph_types:
        for size in sizes:
            try:
                graph = generator(size)
                if not nx.is_connected(graph):
                    raise ValueError(f"Generated graph {graph_type} with size {size} is not connected.")
                filename = f"graphs/{graph_type}_{str(size).zfill(3)}.txt"
                nx.write_edgelist(graph, filename, data=False)
                print(f"Graph saved: {filename}")
            except Exception as e:
                print(f"Error generating {graph_type} with size {size}: {e}")

def generate_and_save_exotic_graphs():
    # Répertoire pour sauvegarder les graphes exotiques
    if not os.path.exists("exotic_graphs"):
        os.makedirs("exotic_graphs")

    # Types de graphes exotiques
    exotic_graph_types = [
        ("small_world", lambda n: nx.watts_strogatz_graph(n, 4, 0.1, seed=SEED)),
        ("clique_with_noise", lambda n: add_noise_to_clique(nx.complete_graph(n))),
        ("hypercube", lambda n: ensure_connected(nx.convert_node_labels_to_integers(nx.hypercube_graph(int(n**0.25))))),
        ("kneser", lambda n: nx.cycle_graph(n))
    ]

    sizes = list(range(50, 501, 50))

    for graph_type, generator in exotic_graph_types:
        for size in sizes:
            try:
                graph = generator(size)
                if not nx.is_connected(graph):
                    raise ValueError(f"Generated exotic graph {graph_type} with size {size} is not connected.")
                filename = f"exotic_graphs/{graph_type}_{str(size).zfill(3)}.txt"
                nx.write_edgelist(graph, filename, data=False)
                print(f"Exotic graph saved: {filename}")
            except Exception as e:
                print(f"Error generating {graph_type} with size {size}: {e}")

def add_chords_to_cycle(graph):
    """Ajouter des chords prédéfinis à un graphe cycle."""
    nodes = list(graph.nodes)
    for i in range(0, len(nodes) - 2, 3):
        graph.add_edge(nodes[i], nodes[i + 2])
    return graph

def add_edges_to_binary_tree(tree):
    """Ajouter des arêtes supplémentaires à un arbre pour le rendre plus complexe."""
    nodes = list(tree.nodes)
    for i in range(0, len(nodes) - 2, 4):
        if i + 4 < len(nodes):
            tree.add_edge(nodes[i], nodes[i + 4])
    return tree

def add_noise_to_clique(graph):
    """Modifier un graphe complet pour ajouter du bruit tout en conservant la connectivité."""
    edges = list(graph.edges)
    if edges:
        graph.remove_edge(*edges[0])  # Supprime une arête fixe
    if len(graph.nodes) > 2:
        graph.add_edge(0, len(graph.nodes) - 1)  # Ajoute une nouvelle arête
    return graph

def ensure_connected(graph):
    """Vérifier et rendre un graphe connecté si nécessaire."""
    if nx.is_connected(graph):
        return graph
    components = list(nx.connected_components(graph))
    for i in range(1, len(components)):
        u = list(components[0])[0]
        v = list(components[i])[0]
        graph.add_edge(u, v)
    return graph

if __name__ == "__main__":
    generate_and_save_graphs()
    generate_and_save_exotic_graphs()
