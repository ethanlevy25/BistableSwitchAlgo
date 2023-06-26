


def connected(adj_mat):
    def dfs(adj_mat, start):
        counter = 0
        visited = set()
        stack = [start]
        while stack:
            node = stack.pop()
            if node in visited:
                continue
            visited.add(node)
            counter+=1
            for i in range(len(adj_mat)):
                if adj_mat[node][i] == 1:
                    stack.append(i)
        return counter
    return dfs(adj_mat, 0) == len(adj_mat)

def ccw(A,B,C):
    return (C[1]-A[1])*(B[0]-A[0]) > (B[1]-A[1])*(C[0]-A[0])

def intersect(A,B,C,D):
    return ccw(A,C,D) != ccw(B,C,D) and ccw(A,B,C) != ccw(A,B,D)

def intersects_with_any(node1, node2, nodes_x, nodes_y, edges):
    for i in range(len(edges)):
        if intersect((nodes_x[node1], nodes_y[node1]), (nodes_x[node2], nodes_y[node2]), (nodes_x[edges[i][0]], nodes_y[edges[i][0]]), (nodes_x[edges[i][1]], nodes_y[edges[i][1]])):
            return True
    return False

def planar_check(node, nodes_x, nodes_y, edges):
    for i in range(len(edges)):
        if node in edges[i] and intersects_with_any(edges[i][0], edges[i][1], nodes_x, nodes_y, edges):
            return False
    return True

def is_planar(nodes_x, nodes_y, edges):
    for i in range(len(edges)):
        for ii in range(i, len(edges)):
            a = (nodes_x[edges[i][0]], nodes_y[edges[i][0]])
            b = (nodes_x[edges[i][1]], nodes_y[edges[i][1]])
            c = (nodes_x[edges[ii][0]], nodes_y[edges[ii][0]])
            d = (nodes_x[edges[ii][1]], nodes_y[edges[ii][1]])
            if intersect(a,b,c,d): return False
    return True        

