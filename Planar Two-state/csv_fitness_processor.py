import pandas as pd
import measure_fitness_2state


def process_and_run_fitness(csv_path):
    # Load the CSV data
    data = pd.read_csv(csv_path)

    # Extract pose_x, pose_y, edges, and spring_dict from the CSV data
    nodes_x = data.iloc[:, 0].dropna().tolist()
    nodes_y = data.iloc[:, 1].dropna().tolist()
    edges = [tuple(map(int, edge.split('-'))) for edge in data.iloc[:, 2].dropna().tolist()]
    spring_values = data.iloc[:, 3].dropna().tolist()
    spring_dict = dict(zip(edges, spring_values))

    # Construct the adjacency matrix
    num_nodes = len(nodes_x)
    adj_mat = [[0 for _ in range(num_nodes)] for _ in range(num_nodes)]
    for edge in edges:
        adj_mat[edge[0]][edge[1]] = 1
        adj_mat[edge[1]][edge[0]] = 1

    # Applying the fitness function
    vis = True
    result = measure_fitness_2state.fitness(nodes_x, nodes_y, vis, adj_mat, edges, spring_dict)
    return result

if __name__ == "__main__":
    csv_path = "YOUR_CSV_FILE_PATH_HERE.csv"
    result = process_and_run_fitness(csv_path)
    print("Fitness Result:", result)
