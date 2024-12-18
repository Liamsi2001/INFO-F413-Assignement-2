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
        ("toroidal_grid", lambda n: nx.grid_2d_graph(int(n ** 0.5), int(n ** 0.5), periodic=True)),
        ("cycle_with_chords", lambda n: add_chords_to_cycle(nx.cycle_graph(n))),
        ("binary_tree_extended", lambda n: add_edges_to_binary_tree(nx.balanced_tree(2, int(n ** 0.25))))
    ]

    sizes = list(range(50, 501, 50))  # Tailles de 50 à 500, par pas de 50

    for graph_type, generator in graph_types:
        for size in sizes:
            graph = generator(size)
            filename = f"graphs/{graph_type}_{str(size).zfill(3)}.txt"
            nx.write_edgelist(graph, filename, data=False)
            print(f"Graph saved: {filename}")

def generate_and_save_exotic_graphs():
    # Répertoire pour sauvegarder les graphes exotiques
    if not os.path.exists("exotic_graphs"):
        os.makedirs("exotic_graphs")

    # Types de graphes exotiques
    exotic_graph_types = [
        ("clique_with_noise", lambda n: add_noise_to_clique(nx.complete_graph(n))),
        ("hexagonal_toroidal", lambda n: nx.convert_node_labels_to_integers(
            nx.hexagonal_lattice_graph(max(2, int(n ** 0.5) // 2), max(2, (int(n ** 0.5) // 2) * 2), periodic=True))),
        ("small_world", lambda n: nx.watts_strogatz_graph(n, 4, 0.1, seed=SEED)),
        ("circular_ladder", lambda n: nx.circular_ladder_graph(max(3, n // 10)))
    ]

    sizes = list(range(50, 501, 50))

    for graph_type, generator in exotic_graph_types:
        for size in sizes:
            try:
                graph = generator(size)
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

if __name__ == "__main__":
    generate_and_save_graphs()
    generate_and_save_exotic_graphs()
