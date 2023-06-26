import random
import copy
import math
import numpy as np
import matplotlib.pyplot as plt
import measure_fitness_2state
import utils




#use gradient search to optimize individual
def gradient_update(nodes_x,nodes_y, adj_mat, edges):
    num_nodes=len(nodes_x)
        
    current_fit=measure_fitness_2state.fitness(nodes_x,nodes_y,False, adj_mat, edges)

    best_value=current_fit
    best_x=copy.deepcopy(nodes_x)
    best_y=copy.deepcopy(nodes_y)

    iter=0
    while True:
        #print("iter",iter,"value",best_value)
        iter=iter+1
        for a in range(num_nodes):
            step=.01
        
            #+x step 
            temp_x=copy.deepcopy(best_x)
            temp_y=copy.deepcopy(best_y)
            temp_x[a]=temp_x[a]+step
            temp_fit=measure_fitness_2state.fitness(temp_x,temp_y,False, adj_mat, edges)
            if temp_fit>best_value:
                best_value=temp_fit
                best_x=copy.deepcopy(temp_x)
                best_y=copy.deepcopy(temp_y)

            #-x step 
            temp_x=copy.deepcopy(best_x)
            temp_y=copy.deepcopy(best_y)
            temp_x[a]=temp_x[a]-step
            temp_fit=measure_fitness_2state.fitness(temp_x,temp_y,False, adj_mat, edges)
            if temp_fit>best_value:
                best_value=temp_fit
                best_x=copy.deepcopy(temp_x)
                best_y=copy.deepcopy(temp_y)

            #+y step 
            temp_x=copy.deepcopy(best_x)
            temp_y=copy.deepcopy(best_y)
            temp_y[a]=temp_y[a]+step
            temp_fit=measure_fitness_2state.fitness(temp_x,temp_y,False, adj_mat, edges)
            if temp_fit>best_value:
                best_value=temp_fit
                best_x=copy.deepcopy(temp_x)
                best_y=copy.deepcopy(temp_y)

            #-y step 
            temp_x=copy.deepcopy(best_x)
            temp_y=copy.deepcopy(best_y)
            temp_y[a]=temp_y[a]-step
            temp_fit=measure_fitness_2state.fitness(temp_x,temp_y,False, adj_mat, edges)
            if temp_fit>best_value:
                best_value=temp_fit
                best_x=copy.deepcopy(temp_x)
                best_y=copy.deepcopy(temp_y)
            


        if best_value<=current_fit:
            break
        current_fit=best_value
    
    for a in range(num_nodes):
        nodes_x[a]=best_x[a]
        nodes_y[a]=best_y[a]
    return best_x,best_y
 