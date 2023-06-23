import matplotlib.pyplot as plt
import random

def ccw(A, B, C):
    '''
    Parameters:
    A, B, C: tuples representing the x and y coordinates of the points.

    Returns:
    A boolean representing whether the path from A to B to C is counter-clockwise. 
    True if the path is counter-clockwise (i.e., point C lies to the left of the line from A to B when moving from A to B),
    False if it's clockwise or collinear (i.e., point C lies to the right or on the same line).
    '''
    return (C[1]-A[1]) * (B[0]-A[0]) > (B[1]-A[1]) * (C[0]-A[0])

# Return true if line segments AB and CD intersect
def check_intersect(A, B, C, D):
    '''
    Parameters:
    A, B: tuples representing the x and y coordinates of the end points of line segment AB.
    C, D: tuples representing the x and y coordinates of the end points of line segment CD.

    Returns:
    A boolean representing whether line segment AB intersects with line segment CD.
    '''
    # Check if line segments are identical
    if (A == C and B == D) or (A == D and B == C):
        return True

    # Check if any points are equal
    if A == C or A == D or B == C or B == D:
        return False

    # Then do the original intersection check
    return ccw(A, C, D) != ccw(B, C, D) and ccw(A, B, C) != ccw(A, B, D)

def check_all_edges_from_node(node, nodes_x, nodes_y, edges):
    '''
    Returns true if no edges from node intersect with any other edges
    '''
    #iterate through every edge
    for i in range(len(edges)):
        #if modified node is a part of the edge
        edge = edges[i]
        if node in edge:
            for ii in range(len(edges)):
                if ii == i: continue
                if check_intersect((nodes_x[edge[0]], nodes_y[edge[0]]), (nodes_x[edge[1]], nodes_y[edge[1]]), (nodes_x[edges[ii][0]], nodes_y[edges[ii][0]]), (nodes_x[edges[ii][1]], nodes_y[edges[ii][1]])):
                    return False
    return True            





