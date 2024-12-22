import networkx as nx
import os
import random

# Global seed for reproducibility
SEED = 42
random.seed(SEED)

def ensure_connected(graph):
    if not nx.is_connected(graph):
        components = list(nx.connected_components(graph))
        for i in range(1, len(components)):
            u = list(components[0])[0]
            v = list(components[i])[0]
            graph.add_edge(u, v)
    return graph

def generate_powerlaw_cluster(n, m, p, seed=None):
    if m >= n:
        raise ValueError("Parameter m must be smaller than n.")
    graph = nx.powerlaw_cluster_graph(n, m, p, seed=seed)
    return ensure_connected(graph)

def generate_grid_graph(n):
    side_1 = 4
    side_2 = n // side_1
    graph = nx.grid_2d_graph(side_1, side_2)
    graph = nx.convert_node_labels_to_integers(graph)
    if graph.number_of_nodes() > n:
        nodes_to_remove = list(range(n, graph.number_of_nodes()))
        graph.remove_nodes_from(nodes_to_remove)
    return graph

def generate_and_save_graphs():
    if not os.path.exists("graphs"):
        os.makedirs("graphs")

    graph_types = [
        ("complete", lambda n: nx.complete_graph(n)),
        ("bipartite", lambda n: nx.complete_bipartite_graph(3*n//5, 2*n//5)),
        ("barbell", lambda n: nx.barbell_graph(n//4, n//2)),
        ("grid", lambda n: generate_grid_graph(n)),
    ]

    sizes = list(range(20, 201, 20))

    for graph_type, generator in graph_types:
        for size in sizes:
            try:
                graph = generator(size)
                graph = ensure_connected(graph)  # Ensure the graph is connected
                filename = f"graphs/{graph_type}_{str(size).zfill(3)}.txt"
                nx.write_edgelist(graph, filename, data=False)
                print(f"Graph saved: {filename}")
            except Exception as e:
                print(f"Error generating {graph_type} with size {size}: {e}")

def generate_and_save_exotic_graphs():
    if not os.path.exists("exotic_graphs"):
        os.makedirs("exotic_graphs")

    exotic_graph_types = [
        ("erdos_renyi", lambda n: ensure_connected(nx.erdos_renyi_graph(n, 0.3, seed=SEED))),
        ("watts_strogatz", lambda n: ensure_connected(nx.watts_strogatz_graph(n, 8, 0.1, seed=SEED))),
        ("barabasi_albert", lambda n: ensure_connected(nx.barabasi_albert_graph(n, 2, seed=SEED))),
        ("Power-Law Cluster", lambda n: generate_powerlaw_cluster(n, 8, 0.9, seed=SEED)),
    ]

    sizes = list(range(20, 201, 20))

    for graph_type, generator in exotic_graph_types:
        for size in sizes:
            try:
                graph = generator(size)
                filename = f"exotic_graphs/{graph_type}_{str(size).zfill(3)}.txt"
                nx.write_edgelist(graph, filename, data=False)
                print(f"Exotic graph saved: {filename}")
            except Exception as e:
                print(f"Error generating {graph_type} with size {size}: {e}")

if __name__ == "__main__":
    generate_and_save_graphs()
    generate_and_save_exotic_graphs()
