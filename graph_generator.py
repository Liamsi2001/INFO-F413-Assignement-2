import networkx as nx
import os

def generate_and_save_graphs():
    # Directory to save standard graphs
    if not os.path.exists("graphs"):
        os.makedirs("graphs")

    graph_types = [
        ("complete", lambda n: nx.complete_graph(n)),
        ("tree", lambda n: nx.balanced_tree(2, int(n ** 0.5))),
        ("circular_ladder", lambda n: nx.circular_ladder_graph(n)),
        ("line_complete", lambda n: nx.line_graph(nx.complete_graph(n))),
    ]

    sizes = [10, 20, 30, 40, 50, 100, 150, 200]  # Number of vertices

    for graph_type, generator in graph_types:
        for size in sizes:
            # Generate graph
            graph = generator(size)

            # Save graph to a file in edge list format
            filename = f"graphs/{graph_type}_{size}.txt"
            nx.write_edgelist(graph, filename, data=False)
            print(f"Graph saved: {filename}")

def generate_and_save_exotic_graphs():
    # Directory to save exotic graphs
    if not os.path.exists("exotic_graphs"):
        os.makedirs("exotic_graphs")

    exotic_graph_types = [
        ("barbell", lambda n: nx.barbell_graph(n // 2, 1)),
        ("wheel", lambda n: nx.wheel_graph(n)),
        ("chordal", lambda n: nx.interval_graph([(i, i + 1) for i in range(n)])),  # Corrected
        ("hierarchical", lambda n: nx.disjoint_union_all([nx.complete_graph(n // 3)] * 3)),
        ("mesh", lambda n: nx.grid_graph(dim=[n // 3, n // 3, n // 3])),
    ]


    sizes = [10, 20, 30, 40, 50, 100, 150, 200]  # Number of vertices

    for graph_type, generator in exotic_graph_types:
        for size in sizes:
            # Generate graph
            graph = generator(size)

            # Save graph to a file in edge list format
            filename = f"exotic_graphs/{graph_type}_{size}.txt"
            nx.write_edgelist(graph, filename, data=False)
            print(f"Exotic graph saved: {filename}")

if __name__ == "__main__":
    generate_and_save_graphs()
    generate_and_save_exotic_graphs()
# The generate_and_save_graphs() function generates standard graphs like complete, grid, tree, circular ladder, and line complete graphs for different sizes (number of vertices). The graphs are saved in the graphs directory in edge list format. The generate_and_save_exotic_graphs() function generates exotic graphs like barbell, wheel, chordal, hierarchical, and mesh graphs for different sizes. The exotic graphs are saved in the exotic_graphs directory in edge list format. The sizes list contains the number of vertices for which the graphs are generated. The graph generator script can be run to generate and save the graphs for later use in experiments or analysis.