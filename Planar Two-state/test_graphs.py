import random
from utils import check_intersect
import matplotlib.pyplot as plt

def test_graph_creation(population_size):
    for i in range(population_size):
        nodes_x=[]
        nodes_y=[]
        edges = set()
        num_nodes=4

        for a in range(num_nodes): 
            nodes_x.append(random.uniform(0,1)+.5)
            nodes_y.append(random.uniform(0,1)+.5)
            for i in range(a):
                add = True
                x,y = nodes_x[i], nodes_y[i]
                for edge in edges:
                    if check_intersect((nodes_x[a], nodes_y[a]), (x, y), (nodes_x[edge[0]], nodes_y[edge[0]]), (nodes_x[edge[1]], nodes_y[edge[1]])):
                        add = False
                        break
                if add:
                    edges.add((a,i))
        
        # Start Plotting
        plt.figure()
        for edge in edges:
            plt.plot([nodes_x[edge[0]], nodes_x[edge[1]]], [nodes_y[edge[0]], nodes_y[edge[1]]])

        # plot nodes
        plt.scatter(nodes_x, nodes_y, color='red')

        # Add title and labels
        plt.title('Graph {}'.format(i+1))
        plt.xlabel('x')
        plt.ylabel('y')

        # Show the plot
        plt.show()

# Call the function
test_graph_creation(5)