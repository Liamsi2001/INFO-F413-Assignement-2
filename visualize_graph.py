import matplotlib.pyplot as plt
import networkx as nx

# Fonction d'ajout de chords
def add_chords_to_cycle(graph):
    nodes = list(graph.nodes)
    for i in range(0, len(nodes) - 2, 3):
        graph.add_edge(nodes[i], nodes[i + 2])
    return graph

# Fonction pour ajouter du bruit à un graphe clique
def add_noise_to_clique(graph):
    edges = list(graph.edges)
    if edges:
        graph.remove_edge(*edges[0])  # Supprimer une arête
    if len(graph.nodes) > 2:
        graph.add_edge(0, len(graph.nodes) - 1)  # Ajouter une nouvelle arête
    return graph

# Définir les graphes et leurs générateurs
graph_examples = [
    # Graphes simples
    ("Complete Graph", nx.complete_graph(10)),  # Complet
    ("Grid Graph", nx.convert_node_labels_to_integers(nx.grid_2d_graph(4, 4))),  # Grille plane
    ("Path Graph", nx.path_graph(10)),  # Chemin simple
    ("Cycle with Chords", add_chords_to_cycle(nx.cycle_graph(10))),  # Cycle avec chords

    # Graphes complexes
    ("Clique with Noise", add_noise_to_clique(nx.complete_graph(10))),  # Clique modifiée
    ("Small-World Network", nx.watts_strogatz_graph(15, 4, 0.1, seed=42)),  # Réseau small-world
    ("Hypercube Graph", nx.hypercube_graph(3)),  # Hypercube en 3D
    ("Cycle Graph", nx.cycle_graph(15)),  # Cycle standard
]

# Générer et afficher les visualisations
for title, graph in graph_examples:
    plt.figure(figsize=(6, 6))
    nx.draw(graph, with_labels=True, node_size=500, font_size=10)
    plt.title(title)
    plt.show()
