import random
import matplotlib.pyplot as plt
import utils  # import the module containing your function

def test_check_all_edges_from_node():
    # Create some nodes
    nodes_x = [random.uniform(0,1)+.5 for _ in range(4)]
    nodes_y = [random.uniform(0,1)+.5 for _ in range(4)]
    
    # Create some edges
    edges = [(0, 1), (1, 2), (2, 3)]

    # Prepare the plot
    fig, ax = plt.subplots()
    ax.set_title('Node Intersections')

    # Test the function on each node and plot the results
    for node in range(4):
        result = utils.check_all_edges_from_node(node, nodes_x, nodes_y, edges)
        print(f'Node {node} intersects with another edge: {result}')
        
        # Plot the node, use red color for nodes with intersections and blue for others
        color = 'red' if result else 'blue'
        ax.plot(nodes_x[node], nodes_y[node], 'o', color=color)

    # Plot the edges
    for edge in edges:
        ax.plot([nodes_x[edge[0]], nodes_x[edge[1]]], [nodes_y[edge[0]], nodes_y[edge[1]]], 'k-')

    plt.show()

for _ in range(10):test_check_all_edges_from_node()
