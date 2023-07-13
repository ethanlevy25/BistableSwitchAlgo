import random
import copy
import math
import numpy as np
import matplotlib.pyplot as plt
import measure_fitness_2state
import gradient2state
import csv
import utils
import pandas as pd
from datetime import datetime



population_size=20 #number of individuals in populatoin
generations=10 #number of generations to run

spring_min = 5
spring_max = 15

#init population
best=0
best_i=0
population_x=[]
population_y=[]
population_edges=[]
population_matrices=[]
population_springs = []
fitness_values=[]


alpha=.1#learning rate

#generate random population
for i in range(population_size):
    num_nodes=4

    nodes_x=[]
    nodes_y=[]
    edges=[]
    adjacency_matrix = [[0 for j in range(num_nodes)] for i in range(num_nodes)]
    springs = {}


    created = False
    
    while not created or measure_fitness_2state.fitness(nodes_x, nodes_y, False, adjacency_matrix, edges, springs) == 0:
        created = True
        nodes_x=[]
        nodes_y=[]
        edges=[]
        adjacency_matrix = [[0 for j in range(num_nodes)] for i in range(num_nodes)]
        
        for a in range(num_nodes): 
            nodes_x.append(random.uniform(0,1)+.5)
            nodes_y.append(random.uniform(0,1)+.5)


    #for each node in the graph, adds an edge from it to every other node as long as the edge wouldn't create an intersection
        for node1 in range(num_nodes):
            for node2 in range(num_nodes):
                #checks for: trying to make edge with itself, edge already created, or intersection created
                if node1 == node2 or adjacency_matrix[node1][node2] == 1 or utils.intersects_with_any(node1, node2, nodes_x, nodes_y, edges):
                    continue
                adjacency_matrix[node1][node2] = 1
                adjacency_matrix[node2][node1] = 1
                edges.append((node1, node2))
                springs[(node1,node2)] = random.uniform(spring_min, spring_max)

    population_springs.append(springs)
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
        fitness_values.append(measure_fitness_2state.fitness(population_x[i],population_y[i],False, population_matrices[i], population_edges[i], population_springs[i]))
    # print(fitness_values)
    #if we are not done, create new genration
    if g<(generations-1):
        total_fitness=0
        new_population_x=[]
        new_population_y=[]
        new_population_edges=[]
        new_population_matrices=[]
        new_population_springs = []

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
        # print("0 count: ", fitness_values.count(0))
        # print("fitness vals", fitness_values)
        
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
            
            adjacency_matrix = [[0 for j in range(num_nodes)] for i in range(num_nodes)]
            edges = [] 
            springs = {}

            # for edge in population_edges[sample1]:
            #     if edge[0] <sp or edge[1] < sp:
            #         edges.append(edge)
            #         springs[edge] = population_springs[sample1][edge]

            # for edge in population_edges[sample2]:
            #     if edge not in edges and (edge[0] >= sp or edge[1]>=sp):
            #         edges.append(edge)
            #         springs[edge] = population_springs[sample2][edge]

            for node1 in range(num_nodes):
                for node2 in range(num_nodes):
                    if node1 == node2 or adjacency_matrix[node1][node2] == 1 or utils.intersects_with_any(node1, node2, nodes_x, nodes_y, edges):
                        continue
                    adjacency_matrix[node1][node2] = 1
                    adjacency_matrix[node2][node1] = 1
                    edge = (node1, node2)
                    edges.append(edge)
                    if (node1, node2) in population_edges[sample1]:
                        springs[edge] = population_springs[sample1][edge]
                    elif (node1, node2) in population_edges[sample2]:
                        springs[edge] = population_springs[sample2][edge]
                    else:
                        springs[(node1, node2)] = random.uniform(spring_min, spring_max)

            new_population_x.append(copy.deepcopy(child_x))
            new_population_y.append(copy.deepcopy(child_y))
            new_population_edges.append(copy.deepcopy(edges))
            new_population_matrices.append(copy.deepcopy(adjacency_matrix))
            new_population_springs.append(springs)
            
       

        #gradient opt best 
        gradient2state.gradient_update(population_x[best_location],population_y[best_location], population_matrices[best_location], population_edges[best_location], population_springs[best_location])

        new_population_x.append(copy.deepcopy(population_x[best_location]))
        new_population_y.append(copy.deepcopy(population_y[best_location]))
        new_population_edges.append(copy.deepcopy(population_edges[best_location]))
        new_population_matrices.append(copy.deepcopy(population_matrices[best_location]))
        new_population_springs.append(population_springs[best_location])


        population_x=copy.deepcopy(new_population_x)
        population_y=copy.deepcopy(new_population_y)
        population_edges=copy.deepcopy(new_population_edges)
        population_matrices = copy.deepcopy(new_population_matrices)
        population_springs = copy.deepcopy(new_population_springs)

      

        #mutate all but best
        for k in range(population_size-1):
            for p in range(num_nodes):
                if random.uniform(0,1)>.75:
                    temp_nodes_x = copy.deepcopy(population_x[k])
                    temp_x = population_x[k][p] + random.uniform(-alpha,alpha)
                    temp_nodes_x[p] = temp_x
                    if utils.is_planar(temp_nodes_x, population_y[k], population_edges[k]):
                        population_x[k][p] = temp_x
                    
                if random.uniform(0,1)>.75:
                    temp_nodes_y = copy.deepcopy(population_y[k])
                    temp_y = population_y[k][p] + random.uniform(-alpha,alpha)
                    temp_nodes_y[p] = temp_y
                    if utils.is_planar(population_x[k], temp_nodes_y, population_edges[k]):
                        population_y[k][p] = temp_y
            if random.uniform(0,1)>0.5:
                gradient2state.gradient_update(population_x[k],population_y[k], population_matrices[k], population_edges[k], population_springs[k])


       

#display best individual   
max_v=0
max_i=0
for i in range(population_size):
    if fitness_values[i]>max_v:
        max_v=fitness_values[i]
        max_i=i
max_fitness = measure_fitness_2state.fitness(population_x[max_i],population_y[max_i],True, population_matrices[max_i], population_edges[max_i], population_springs[max_i])
print(max_fitness)

current_date_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

data = {
    "date_time": current_date_time,
    "nodes_x": [population_x[max_i]],
    "nodes_y": [population_y[max_i]],
    "edges": [population_edges[max_i]],
    "adjacency_matrix": [population_matrices[max_i]],
    "spring_coeff": [population_springs[max_i]],
    "fitness": max_fitness
}

df = pd.DataFrame(data)

# Write DataFrame to CSV
with open('results.csv', 'a') as f:
    df.to_csv(f, header=f.tell() == 0, index=False)
