import random
import copy
import math
import numpy as np
import matplotlib.pyplot as plt
import measure_fitness_3state
import gradient3state


nodes_x=[]
nodes_y=[]
num_nodes=5



for a in range(num_nodes): 
    nodes_x.append(random.uniform(0,1)+.5)
    nodes_y.append(random.uniform(0,1)+.5)

gradient3state.gradient_update(nodes_x,nodes_y)

measure_fitness_3state.fitness(nodes_x,nodes_y,True)