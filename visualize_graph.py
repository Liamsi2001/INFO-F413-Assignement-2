import matplotlib.pyplot as plt
import networkx as nx

# Define the graphs and their generators
graph_examples = [
    # Simple Graphs
    ("Complete Graph", nx.complete_graph(10)),  # Simple and fully connected
    ("Grid Graph", nx.convert_node_labels_to_integers(nx.grid_2d_graph(4, 4))),  # Planar grid
    ("Tree Graph", nx.balanced_tree(2, 3)),  # Binary tree with height 3
    ("Cycle with Chords", nx.cycle_graph(10).copy()),  # Cycle graph with added complexity

    # Complex Graphs
    ("Clique with Noise", nx.Graph(nx.complete_graph(10)).copy()),  # Start with a clique, modify later
    ("Hexagonal Grid", nx.convert_node_labels_to_integers(nx.hexagonal_lattice_graph(2, 2))),  # Hexagonal lattice
    ("Small-World Network", nx.watts_strogatz_graph(15, 4, 0.1)),  # Small-world with 15 nodes
    ("Triangular Grid", nx.convert_node_labels_to_integers(nx.triangular_lattice_graph(3, 3))),  # Triangular lattice
]

# Modify the "Clique with Noise" graph
clique_with_noise = graph_examples[4][1]
clique_with_noise.remove_edge(0, 1)  # Remove an edge to introduce noise
clique_with_noise.add_edge(0, 5)  # Add a random edge to introduce complexity

# Replace the noisy clique in the list
graph_examples[4] = ("Clique with Noise", clique_with_noise)

# Create and display visualizations
for title, graph in graph_examples:
    plt.figure(figsize=(6, 6))
    nx.draw(graph, with_labels=True, node_size=500, font_size=10)
    plt.title(title)
    plt.show()
# The code snippet above visualizes a set of simple and complex graphs using NetworkX and Matplotlib. The graphs are defined and generated using NetworkX, and then visualized using Matplotlib. The code iterates over a list of graph examples, each containing a title and a NetworkX graph object. For each graph example, a figure is created using Matplotlib, and the graph is drawn using the nx.draw function. The figure is displayed using plt.show().