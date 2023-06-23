from utils import check_intersect
import matplotlib.pyplot as plt
import random  

def test_check_intersect():
    # Set 1
    A = (random.uniform(0,1)+.5, random.uniform(0,1)+.5)
    B = (random.uniform(0,1)+.5, random.uniform(0,1)+.5)
    
    # Set 2
    C = (random.uniform(0,1)+.5, random.uniform(0,1)+.5)
    D = (random.uniform(0,1)+.5, random.uniform(0,1)+.5)
    
    # Check and print intersection
    print(check_intersect(A, B, C, D))
    
    # Create the plot
    plt.figure()

    # Add Set 1 line
    plt.plot([A[0], B[0]], [A[1], B[1]], label='AB')
    
    # Add Set 2 line
    plt.plot([C[0], D[0]], [C[1], D[1]], label='CD')
    
    # Add markers for points
    plt.scatter(*zip(A,B,C,D), color='red')
    
    # Add legend
    plt.legend()
    
    # Display the plot
    plt.show()

for _ in range(20): test_check_intersect()

