import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_combined_results(simple_csv, exotic_csv, output_file):
    # Create the plots directory if it doesn't exist
    if not os.path.exists("plots"):
        os.makedirs("plots")

    # Read the CSV files
    simple_data = pd.read_csv(simple_csv)
    exotic_data = pd.read_csv(exotic_csv)

    # Add a "Graph Type" and "Nodes" column
    simple_data['Graph Type'] = simple_data['Graph File'].str.split('_').str[0]
    simple_data['Nodes'] = simple_data['Graph File'].str.extract('(\d+)').astype(int)
    exotic_data['Graph Type'] = exotic_data['Graph File'].str.split('_').str[0]
    exotic_data['Nodes'] = exotic_data['Graph File'].str.extract('(\d+)').astype(int)

    # Combine datasets for plotting
    combined_data = pd.concat([simple_data, exotic_data], ignore_index=True)
    graph_types = combined_data['Graph Type'].unique()

    # Colors for different graph types
    colors = plt.cm.tab10(range(len(graph_types)))
    color_map = dict(zip(graph_types, colors))

    # Create a combined plot
    fig, ax = plt.subplots(2, 1, figsize=(14, 12))

    # Plot success probabilities
    for graph_type in graph_types:
        subset = combined_data[combined_data['Graph Type'] == graph_type]
        ax[0].plot(subset['Nodes'], subset['Contract Success Rate'], label=f"{graph_type} Empirical", marker='o', color=color_map[graph_type])
        ax[0].plot(subset['Nodes'], subset['Theoretical Success Probability'], linestyle='--', label=f"{graph_type} Theoretical", color=color_map[graph_type])
    ax[0].set_xlabel("Number of Nodes")
    ax[0].set_ylabel("Success Probability")
    ax[0].set_title("Success Probabilities by Graph Type")
    ax[0].legend()

    # Plot average iteration times
    for graph_type in graph_types:
        subset = combined_data[combined_data['Graph Type'] == graph_type]
        ax[1].plot(subset['Nodes'], subset['Avg Iteration Time (ms)'], label=f"{graph_type} Avg Time", marker='o', color=color_map[graph_type])
    ax[1].set_xlabel("Number of Nodes")
    ax[1].set_ylabel("Avg Iteration Time (ms)")
    ax[1].set_title("Average Iteration Time by Graph Type")
    ax[1].legend()

    # Adjust layout
    plt.tight_layout()
    plt.savefig(f"plots/{output_file}.png")
    plt.close()

if __name__ == "__main__":
    # Plot combined results for simple and exotic graphs
    plot_combined_results("contract_simple.csv", "contract_exotic.csv", "combined_results")
