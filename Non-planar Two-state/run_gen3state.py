import random
import copy
import math
import numpy as np
import matplotlib.pyplot as plt
import measure_fitness_3state
import gradient3state
import utils



population_size=20 #number of individuals in populatoin
generations=20 #number of generations to run

#init population
best=0
best_i=0
population_x=[]
population_y=[]
population_edges=[]
population_matrices=[]
fitness_values=[]

alpha=.1#learning rate

#generate random population
for i in range(population_size):
    nodes_x=[]
    nodes_y=[]
    
    num_nodes=4
    edges=[]
    adjacency_matrix = [[0 for j in range(num_nodes)] for i in range(num_nodes)]



    for a in range(num_nodes): 
        nodes_x.append(random.uniform(0,1)+.5)
        nodes_y.append(random.uniform(0,1)+.5)

    while not utils.connected(adjacency_matrix):
        node1, node2 = random.randint(0, num_nodes-1), random.randint(0, num_nodes-1)
        if node1 == node2 or adjacency_matrix[node1][node2] == 1 or utils.intersects_with_any(node1, node2, nodes_x, nodes_y, edges):
            continue
        adjacency_matrix[node1][node2] = 1
        adjacency_matrix[node2][node1] = 1
        edges.append((node1, node2))



    # plt.figure(i)  # Create a new figure for each graph
    # for edge in edges:
    #     plt.plot([nodes_x[edge[0]], nodes_x[edge[1]]], [nodes_y[edge[0]], nodes_y[edge[1]]], 'k-')  # Plot edges
    # plt.scatter(nodes_x, nodes_y)  # Plot nodes
    # plt.title(f'Graph {i+1}')
    # plt.show()




    population_x.append(nodes_x)
    population_y.append(nodes_y)
    population_edges.append(edges)
    population_matrices.append(adjacency_matrix)

#evolve
for g in range(generations):
    print("generation",g)
   
    #measure population
    fitness_values=[]
    for i in range(population_size): 
        fitness_values.append(measure_fitness_3state.fitness(population_x[i],population_y[i],False, population_matrices[i], population_edges[i]))

    #if we are not done, create new genration
    if g<(generations-1):
        total_fitness=0
        new_population_x=[]
        new_population_y=[]
        new_population_edges=[]
        new_population_matrices=[]

        best_found=0
        best_location=0
        for k in range(population_size):
            total_fitness=total_fitness+fitness_values[k]
            if fitness_values[k]>=best_found:
                best_found=fitness_values[k]
                best_location=k

        #if no imporovment from last generation, increase learning rate, else set learning rate to base rate
        if best_location==population_size-1:
            alpha=alpha+.1
        else:
            alpha = .1
        if alpha>2:
            alpha=2
        
        # print(fitness_values)
        print("best found is ",best_found,"location",best_location,"total fitness=",total_fitness,"alpha",alpha)
        
        #generate next generation
        for k in range(population_size-1):

            #find two parents with P ~ fitness
            sample_point1=random.uniform(0,total_fitness)
            sample_point2=random.uniform(0,total_fitness)
            
            sample1=0
            sample2=0
            current_sample_point=0
            for s in range(population_size):
                current_sample_point=current_sample_point+fitness_values[s]

                if current_sample_point<sample_point1:
                    sample1=s+1
                if current_sample_point<sample_point2:
                    sample2=s+1
            
            #do crossover
            child_x=[]
            child_y=[]
            split=random.randint(0,num_nodes-1)
            for sp in range(num_nodes):
                if sp<split:
                    child_x.append(population_x[sample1][sp])
                    child_y.append(population_y[sample1][sp])
                else:
                    child_x.append(population_x[sample2][sp])
                    child_y.append(population_y[sample2][sp])

            possible_edges = []
            for edge in population_edges[sample1]:
                if edge[0] < split or edge[1] < split:
                    possible_edges.append(edge)
            for edge in population_edges[sample2]:
                if edge[0] >= split or edge[1] >= split:
                    possible_edges.append(edge)

            adjacency_matrix = [[0 for j in range(num_nodes)] for i in range(num_nodes)]
            edges =[]
            for edge in possible_edges:
                if edge in edges or utils.intersects_with_any(edge[0], edge[1], child_x, child_y, possible_edges):
                    continue
                adjacency_matrix[edge[0]][edge[1]] = 1
                adjacency_matrix[edge[1]][edge[0]] = 1
                edges.append(edge)

            while not utils.connected(adjacency_matrix):
                node1, node2 = random.randint(0, num_nodes-1), random.randint(0, num_nodes-1)
                if node1 == node2 or adjacency_matrix[node1][node2] == 1 or utils.intersects_with_any(node1, node2, nodes_x, nodes_y, edges):
                    continue
                adjacency_matrix[node1][node2] = 1
                adjacency_matrix[node2][node1] = 1
                edges.append((node1, node2))        
                
                




            new_population_x.append(copy.deepcopy(child_x))
            new_population_y.append(copy.deepcopy(child_y))
            new_population_edges.append(copy.deepcopy(edges))
            new_population_matrices.append(copy.deepcopy(adjacency_matrix))
       

        #gradient opt best 
        gradient3state.gradient_update(population_x[best_location],population_y[best_location], population_matrices[best_location], population_edges[best_location])

        new_population_x.append(copy.deepcopy(population_x[best_location]))
        new_population_y.append(copy.deepcopy(population_y[best_location]))
        new_population_edges.append(copy.deepcopy(population_edges[best_location]))
        new_population_matrices.append(copy.deepcopy(population_matrices[best_location]))


        population_x=copy.deepcopy(new_population_x)
        population_y=copy.deepcopy(new_population_y)
        population_edges=copy.deepcopy(new_population_edges)
        population_matrices = copy.deepcopy(new_population_matrices)

      

        #mutate all but best
        for k in range(population_size-1):
            for p in range(num_nodes):
                if random.uniform(0,1)>.75:
                    temp_nodes_x = copy.deepcopy(population_x[k])
                    temp_x = population_x[k][p] + random.uniform(-alpha,alpha)
                    temp_nodes_x[p] = temp_x
                    if utils.planar_check(p, temp_nodes_x, population_y[k], population_edges[k]):
                        population_x[k][p] = temp_x
                    
                if random.uniform(0,1)>.75:
                    temp_nodes_y = copy.deepcopy(population_y[k])
                    temp_y = population_y[k][p] + random.uniform(-alpha,alpha)
                    temp_nodes_y[p] = temp_y
                    if utils.planar_check(p, population_x[k], temp_nodes_y, population_edges[k]):
                        population_y[k][p] = temp_y
            if random.uniform(0,1)>.5:
                gradient3state.gradient_update(population_x[k],population_y[k], population_matrices[k], population_edges[k])


       

#display best indidual   
max_v=0
max_i=0
for i in range(population_size):
    if fitness_values[i]>max_v:
        max_v=fitness_values[i]
        max_i=i

print(utils.is_planar(population_x[max_i], population_y[max_i], population_edges[max_i]))
print(population_x[max_i])
print(population_y[max_i])
print(population_edges[max_i])
print(measure_fitness_3state.fitness(population_x[max_i],population_y[max_i],True, population_matrices[max_i], population_edges[max_i]))
