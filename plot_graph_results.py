import pandas as pd
import matplotlib.pyplot as plt
import os

def extract_columns(df):
    if 'Nodes' not in df.columns:
        # Use a more robust regex pattern to capture numbers in the file name
        df['Nodes'] = df['Graph File'].str.extract(r'_(\d+)\.').astype(float).fillna(-1).astype(int)
        if (df['Nodes'] == -1).any():
            print("Warning: Could not extract Nodes for some rows.")
    if 'Graph Type' not in df.columns:
        # Extract everything before the first underscore
        df['Graph Type'] = df['Graph File'].str.split('_').str[0]
    return df


def plot_success_probabilities(contract_simple, contract_exotic, fast_simple, fast_exotic, output_file):
    # Read the CSV files
    contract_simple_data = pd.read_csv(contract_simple)
    contract_exotic_data = pd.read_csv(contract_exotic)
    fast_simple_data = pd.read_csv(fast_simple)
    fast_exotic_data = pd.read_csv(fast_exotic)

    # Ensure Nodes and Graph Type columns exist
    contract_simple_data = extract_columns(contract_simple_data)
    contract_exotic_data = extract_columns(contract_exotic_data)
    fast_simple_data = extract_columns(fast_simple_data)
    fast_exotic_data = extract_columns(fast_exotic_data)

    # Extract theoretical probabilities
    theoretical_contract = contract_simple_data[['Nodes', 'Theoretical Success Probability']].drop_duplicates()
    theoretical_fastcut = fast_simple_data[['Nodes', 'Theoretical Success Probability']].drop_duplicates()

    # Plot success probabilities
    fig, ax = plt.subplots(2, 1, figsize=(14, 12))

    # Contract probabilities by graph type
    for graph_type in contract_simple_data['Graph Type'].unique():
        subset = contract_simple_data[contract_simple_data['Graph Type'] == graph_type]
        ax[0].plot(subset['Nodes'], subset['Success Rate'], label=f"{graph_type}", marker='D')
    for graph_type in contract_exotic_data['Graph Type'].unique():
        subset = contract_exotic_data[contract_exotic_data['Graph Type'] == graph_type]
        ax[0].plot(subset['Nodes'], subset['Success Rate'], label=f"{graph_type}", marker='o')
    ax[0].plot(theoretical_contract['Nodes'], theoretical_contract['Theoretical Success Probability'], linestyle='--', label="Contract Theoretical", color="black")

    # FastCut probabilities by graph type
    for graph_type in fast_simple_data['Graph Type'].unique():
        subset = fast_simple_data[fast_simple_data['Graph Type'] == graph_type]
        ax[1].plot(subset['Nodes'], subset['Success Rate'], label=f"{graph_type}", marker='D')
    for graph_type in fast_exotic_data['Graph Type'].unique():
        subset = fast_exotic_data[fast_exotic_data['Graph Type'] == graph_type]
        ax[1].plot(subset['Nodes'], subset['Success Rate'], label=f"{graph_type}", marker='o')
    ax[1].plot(theoretical_fastcut['Nodes'], theoretical_fastcut['Theoretical Success Probability'], linestyle='--', label="FastCut Theoretical", color="black")

    ax[0].set_xlabel("Number of Nodes")
    ax[0].set_ylabel("Success Probability")
    ax[0].set_title("Contract Success Probabilities")
    ax[0].legend()

    ax[1].set_xlabel("Number of Nodes")
    ax[1].set_ylabel("Success Probability")
    ax[1].set_title("FastCut Success Probabilities")
    ax[1].legend()

    plt.tight_layout()
    plt.savefig(f"plots/{output_file}.png")
    plt.close()

def plot_iteration_times(contract_simple, contract_exotic, fast_simple, fast_exotic, output_file):
    # Read the CSV files
    contract_simple_data = pd.read_csv(contract_simple)
    contract_exotic_data = pd.read_csv(contract_exotic)
    fast_simple_data = pd.read_csv(fast_simple)
    fast_exotic_data = pd.read_csv(fast_exotic)

    # Ensure Nodes and Graph Type columns exist
    contract_simple_data = extract_columns(contract_simple_data)
    contract_exotic_data = extract_columns(contract_exotic_data)
    fast_simple_data = extract_columns(fast_simple_data)
    fast_exotic_data = extract_columns(fast_exotic_data)
    
    #Theoretical running time
    theoretical_contract = contract_simple_data[['Nodes', 'Theoretical Running Time (ms)']].drop_duplicates()
    theoretical_fastcut = fast_simple_data[['Nodes', 'Theoretical Running Time (ms)']].drop_duplicates()

    # Plot average iteration times
    fig, ax = plt.subplots(2, 1, figsize=(14, 12))

    # Contract iteration times by graph type
    for graph_type in contract_simple_data['Graph Type'].unique():
        subset = contract_simple_data[contract_simple_data['Graph Type'] == graph_type]
        ax[0].plot(subset['Nodes'], subset['Avg Iteration Time (ms)'], label=f"{graph_type}", marker='D')
    for graph_type in contract_exotic_data['Graph Type'].unique():
        subset = contract_exotic_data[contract_exotic_data['Graph Type'] == graph_type]
        ax[0].plot(subset['Nodes'], subset['Avg Iteration Time (ms)'], label=f"{graph_type}", marker='o')
    ax[0].plot(theoretical_contract['Nodes'], theoretical_contract['Theoretical Running Time (ms)'], linestyle='--', label="Contract Theoretical 1/3000 n²", color="black")
    ax[0].set_xlabel("Number of Nodes")
    ax[0].set_ylabel("Avg Iteration Time (ms)")
    ax[0].set_title("Contract Average Iteration Time")
    ax[0].legend()

    # FastCut iteration times by graph type
    for graph_type in fast_simple_data['Graph Type'].unique():
        subset = fast_simple_data[fast_simple_data['Graph Type'] == graph_type]
        ax[1].plot(subset['Nodes'], subset['Avg Iteration Time (ms)'], label=f"{graph_type}", marker='D')
    for graph_type in fast_exotic_data['Graph Type'].unique():
        subset = fast_exotic_data[fast_exotic_data['Graph Type'] == graph_type]
        ax[1].plot(subset['Nodes'], subset['Avg Iteration Time (ms)'], label=f"{graph_type}", marker='o')
    ax[1].plot(theoretical_fastcut['Nodes'], theoretical_fastcut['Theoretical Running Time (ms)'], linestyle='--', label="FastCut Theoretical 1/300 n²log(n)", color="black")
    ax[1].set_xlabel("Number of Nodes")
    ax[1].set_ylabel("Avg Iteration Time (ms)")
    ax[1].set_title("FastCut Average Iteration Time")
    ax[1].legend()

    plt.tight_layout()
    plt.savefig(f"plots/{output_file}.png")
    plt.close()

if __name__ == "__main__":
    # Generate plots for success probabilities
    plot_success_probabilities(
        "contract_simple.csv", "contract_exotic.csv",
        "fast_simple.csv", "fast_exotic.csv",
        "success_probabilities"
    )

    # Generate plots for average iteration times
    plot_iteration_times(
        "contract_simple.csv", "contract_exotic.csv",
        "fast_simple.csv", "fast_exotic.csv",
        "iteration_times"
    )
