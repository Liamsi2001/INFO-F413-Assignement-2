import matplotlib.pyplot as plt
import networkx as nx
from math import log
# Définir les graphes à visualiser
def ensure_connected(graph):
    if not nx.is_connected(graph):
        components = list(nx.connected_components(graph))
        for i in range(1, len(components)):
            u = list(components[0])[0]
            v = list(components[i])[0]
            graph.add_edge(u, v)
    return graph

def generate_powerlaw_cluster(n, m, p, seed=None):
    graph = nx.powerlaw_cluster_graph(n, m, p, seed=seed)
    return ensure_connected(graph)

def generate_grid_graph(n):
    side_1 = int(n ** 0.5)
    side_2 = (n + side_1 - 1) // side_1
    graph = nx.grid_2d_graph(side_1, side_2)
    graph = nx.convert_node_labels_to_integers(graph)
    # Supprimer les sommets supplémentaires si le nombre dépasse n
    if graph.number_of_nodes() > n:
        nodes_to_remove = list(range(n, graph.number_of_nodes()))
        graph.remove_nodes_from(nodes_to_remove)
    return graph

graph_examples = [
    ("Complete Graph K_20", nx.complete_graph(100), "red"),
    ("Complete Bipartite Graph K_4_16", nx.complete_bipartite_graph(40, 60), "blue"),
    ("Barbell Graph (n1=8, n2=4)", nx.barbell_graph(40, 20), "green"),
    ("Grid Graph (exact n=20)", generate_grid_graph(100), "orange"),
    ("Erdős-Rényi Graph (n=15, p=0.15)", ensure_connected(nx.erdos_renyi_graph(100, log(100)/100, seed=42)), "purple"),
    ("Watts-Strogatz Graph (n=15, k=3, p=0.1)", ensure_connected(nx.watts_strogatz_graph(100, 25, 0.25, seed=42)), "cyan"),
    ("Barabási-Albert Graph (n=15, m=5)", ensure_connected(nx.barabasi_albert_graph(100, 20, seed=42)), "pink"),
    ("Power-Law Cluster Graph (n=15, m=3, p=0.2)", generate_powerlaw_cluster(100, 10, 0.25, seed=42), "yellow"),
]

# Définir la taille de la figure et le nombre de colonnes
fig, axes = plt.subplots(nrows=2, ncols=4, figsize=(20, 15))

# Parcourir les graphes et les afficher
for ax, (title, graph, color) in zip(axes.flatten(), graph_examples):
    pos = nx.spring_layout(graph, seed=42)  # Disposition des nœuds
    nx.draw(
        graph, pos, with_labels=False, node_size=20, node_color=color, edge_color="gray", ax=ax
    )
    ax.set_title(f"{title}\n(Nodes: {graph.number_of_nodes()})", fontsize=10)

# Ajuster l'espacement entre les sous-graphes
plt.tight_layout()

# Sauvegarder l'image
plt.savefig("graph_examples.png", dpi=300)
plt.show()
