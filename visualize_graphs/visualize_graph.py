import matplotlib.pyplot as plt
import networkx as nx
from math import log

# Connect the graph if it is not connected
def ensure_connected(graph):
    if not nx.is_connected(graph):
        components = list(nx.connected_components(graph))
        for i in range(1, len(components)):
            u = list(components[0])[0]
            v = list(components[i])[0]
            graph.add_edge(u, v)
    return graph

# Generate a power-law cluster graph
def generate_powerlaw_cluster(n, m, p, seed=None):
    graph = nx.powerlaw_cluster_graph(n, m, p, seed=seed)
    return ensure_connected(graph)

# Generate a grid graph
def generate_grid_graph(n):
    side_1 = 4
    side_2 = 25
    graph = nx.grid_2d_graph(side_1, side_2)
    graph = nx.convert_node_labels_to_integers(graph)
    # Remove nodes to match the desired number of nodes
    if graph.number_of_nodes() > n:
        nodes_to_remove = list(range(n, graph.number_of_nodes()))
        graph.remove_nodes_from(nodes_to_remove)
    return graph

graph_examples = [
    ("Complete Graph K_100", nx.complete_graph(100), "red"),
    ("Complete Bipartite Graph K_60_40", nx.complete_bipartite_graph(60, 40), "blue"),
    ("Barbell Graph (n1=25, n2=50)", nx.barbell_graph(25, 50), "green"),
    ("Grid Graph (exact n=100)", generate_grid_graph(100), "orange"),
    ("Erdős-Rényi Graph (n=100, p=0.3)", ensure_connected(nx.erdos_renyi_graph(100, 0.3, seed=42)), "purple"),
    ("Watts-Strogatz Graph (n=100, k=8, p=0.1)", ensure_connected(nx.watts_strogatz_graph(100, 8, 0.1, seed=42)), "cyan"),
    ("Barabási-Albert Graph (n=100, m=2)", ensure_connected(nx.barabasi_albert_graph(100, 2, seed=42)), "pink"),
    ("Power-Law Cluster Graph (n=100, m=8, p=0.9)", generate_powerlaw_cluster(100, 8, 0.9, seed=42), "yellow"),
]

# Create a grid of subplots
fig, axes = plt.subplots(nrows=2, ncols=4, figsize=(15, 8))

# Draw each graph on its own subplot
for ax, (title, graph, color) in zip(axes.flatten(), graph_examples):
    pos = nx.spring_layout(graph, seed=42)  # Seed for reproducibility
    nx.draw(
        graph, pos, with_labels=False, node_size=20, node_color=color, edge_color="gray", ax=ax
    )
    ax.set_title(f"{title}\n(Nodes: {graph.number_of_nodes()})", fontsize=10)

# Remove the empty subplot
plt.tight_layout()

# Save the plot
plt.savefig("graph_examples.png", dpi=300)
plt.show()
