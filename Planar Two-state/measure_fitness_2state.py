import random
import copy
import math
import numpy as np
import matplotlib.pyplot as plt
import utils

#compute fitness of individual
def fitness(pose_x,pose_y,vis, adj_mat, edges):
    num_nodes=len(pose_x)
    nodes_x=copy.deepcopy(pose_x)
    nodes_y=copy.deepcopy(pose_y)    
    spring_array =  [[0 for i in range(num_nodes)] for j in range(num_nodes)]
    vel_x=[]
    vel_y=[]
    acc_x=[]
    acc_y=[]
    force_x=[]
    force_y=[]

    #nodes which are attached to the actuating spring
    spring_points = edges[0]

    #flag used if individual is too big, set fitness to 0
    too_big_flag=0
    intersection_flag=0
    
    first_time_in_state=1
    for a in range(num_nodes): 
        #nodes_x.append(random.uniform(0,1)+.5)
        #nodes_y.append(random.uniform(0,1)+.5)
        vel_x.append(0)
        vel_y.append(0)
        acc_x.append(0)
        acc_y.append(0)
        force_x.append(0)
        force_y.append(0)

    #put a spring between all nodes
    for a in range(num_nodes):
        for b in range(num_nodes): 
            if a<b:
                spring_array[a][b]=0  
                if a!=b and adj_mat[a][b]==1:            
                    if random.uniform(0,1)>0.0:#between all nodes
                        spring_array[a][b]=math.sqrt((nodes_x[a]-nodes_x[b])**2+(nodes_y[a]-nodes_y[b])**2)
            else:
                spring_array[a][b]=spring_array[b][a]

    #data used to measer distance between all nodes to compute change
    distance_state0=[[0 for i in range(num_nodes)] for j in range(num_nodes)]
    distance_state1=[[0 for i in range(num_nodes)] for j in range(num_nodes)]    

    t         = 0      # current time of the simulation
    dt        = 0.01    # timestep
    Nt        = 601    # number of timesteps
    spring_coeff = 10  # Hooke's law spring coefficient
    gravity   = -0.0   # strength of gravity
    plotRealTime = vis # switch on for plotting as the simulation goes along
    box_size=2
    damping=.2



    if plotRealTime:
        fig = plt.figure(figsize=(5,5), dpi=80)
        ax = fig.add_subplot(111)

    state=0
    stability_counter=0
    kill=0
    end_loop=0

    # Simulation Main Loop
    for i in range(Nt):
       

        #compute node accelerations
        #f=ma,  all nodes have unit mass, so accel = f
        if (Nt-2)<i:#stop if simulation running too long
            kill=1
           # print("---KILL!-----",i, Nt)
            end_loop=1

        #find f using hooks law
        for n in range(num_nodes):
            force_x[n]=0
            force_y[n]=0
            for k in range(num_nodes):
                if n!=k:
                    if spring_array[n][k]!=0:
                        current_distance=math.sqrt((nodes_x[n]-nodes_x[k])**2+(nodes_y[n]-nodes_y[k])**2)
                        #print("current dist",current_distance)
                        rest_length=spring_array[n][k]
                        #print("rest",rest_length,"dist",current_distance)
                        force_mag=spring_coeff*(rest_length-current_distance)#negitive force pulls nodes together
                        f_x_component_norm=(nodes_x[n]-nodes_x[k])/current_distance
                        f_y_component_norm=(nodes_y[n]-nodes_y[k])/current_distance
                        force_x[n] = force_x[n]+f_x_component_norm*force_mag
                        force_y[n] = force_y[n]+f_y_component_norm*force_mag
         
   
    
        #state 0 active, state 1 passive, state 2 active, state 3 passive.

        #record starting distances
        if state==0 and first_time_in_state==1:
            first_time_in_state=0
            for a in range(num_nodes):
                for b in range(num_nodes):
                    distance_state0[a][b]=math.sqrt((nodes_x[a]-nodes_x[b])**2+(nodes_y[a]-nodes_y[b])**2)
                    if distance_state0[a][b]>0.95:
                        too_big_flag=1
            if not utils.is_planar(nodes_x, nodes_y, edges): return 0            
            if plotRealTime :
                # print(nodes_x)
                # print(nodes_y)
                #print out image
                plt.subplot(411)
                x_values=[]
                y_values=[]

                x_values_link=[]
                y_values_link=[]
                for x in range(num_nodes):
                    x_values.append(nodes_x[x])
                    y_values.append(nodes_y[x])
                            
                for a in range(num_nodes):
                    for b in range(a,num_nodes): 
                        if a!=b:
                            if spring_array[a][b]!=0:
                                
                            
                                x_spring=[]
                                y_spring=[]
                                x_spring.append(nodes_x[a])
                                x_spring.append(nodes_x[b])
                                y_spring.append(nodes_y[a])
                                y_spring.append(nodes_y[b])
                                plt.plot(x_spring,y_spring, 'bo', linestyle="--")
                
                x_values=[]
                y_values=[]

                x_values_link=[]
                y_values_link=[]
                for x in spring_points:
                    x_values.append(nodes_x[x])
                    y_values.append(nodes_y[x])
            
                plt.plot(x_values,y_values,'r')

        #record distances
        if state==2 and first_time_in_state==1:
            first_time_in_state=0
            for a in range(num_nodes):
                for b in range(num_nodes):
                    distance_state1[a][b]=math.sqrt((nodes_x[a]-nodes_x[b])**2+(nodes_y[a]-nodes_y[b])**2)
                    if distance_state1[a][b]>0.95:
                        too_big_flag=1
            if not utils.is_planar(nodes_x, nodes_y, edges): return 0      

            if plotRealTime :           #print out image

                # print(nodes_x)
                # print(nodes_y)
                plt.subplot(412)
                #print out image
            
                x_values=[]
                y_values=[]

                x_values_link=[]
                y_values_link=[]
                for x in range(num_nodes):
                    x_values.append(nodes_x[x])
                    y_values.append(nodes_y[x])
                            
                for a in range(num_nodes):
                    for b in range(a,num_nodes): 
                        if a!=b:
                            if spring_array[a][b]!=0:
                                
                            
                                x_spring=[]
                                y_spring=[]
                                x_spring.append(nodes_x[a])
                                x_spring.append(nodes_x[b])
                                y_spring.append(nodes_y[a])
                                y_spring.append(nodes_y[b])
                                plt.plot(x_spring,y_spring, 'bo', linestyle="--")
                
                x_values=[]
                y_values=[]

                x_values_link=[]
                y_values_link=[]
                for x in spring_points:
                    x_values.append(nodes_x[x])
                    y_values.append(nodes_y[x])
            
                plt.plot(x_values,y_values,'r')
                plt.savefig("test.png")


            #print("good ending-------------------")
            end_loop=1
     
        
        #actuate one spring
        if state==0:#expanding
            a,b = spring_points    
            current_distance=math.sqrt((nodes_x[a]-nodes_x[b])**2+(nodes_y[a]-nodes_y[b])**2)
            force_mag=-spring_coeff*(.1-current_distance)#negitive force pulls nodes together   
            f_x_component_norm=(nodes_x[a]-nodes_x[b])/current_distance
            f_y_component_norm=(nodes_y[a]-nodes_y[b])/current_distance 
            force_x[a] = force_x[a]-f_x_component_norm*force_mag
            force_y[a] = force_y[b]-f_y_component_norm*force_mag
            force_x[b] = force_x[b]+f_x_component_norm*force_mag
            force_y[b] = force_y[b]+f_y_component_norm*force_mag            
       


       
        

        #update node velocity
        for n in range(num_nodes):
            vel_x[n] = vel_x[n]*damping + force_x[n]
            if True: 
                vel_y[n] = vel_y[n]*damping + force_y[n]+ gravity
            else:            
                vel_y[n] = vel_y[n]*damping + force_y[n]
           
        max_force=0
        for n in range(num_nodes):
            if force_x[n]>max_force:
                max_force=force_x[n]
            if force_y[n]>max_force:
                max_force=force_y[n]  

        if max_force<.001:
            stability_counter=stability_counter+1
        else:
            stability_counter=0
        if stability_counter>10:
            stability_counter=0
            state=state+1
            first_time_in_state=1
   

        # #update node postion        
        for n in range(num_nodes):
            nodes_x[n]=nodes_x[n]+vel_x[n]*dt
            nodes_y[n]=nodes_y[n]+vel_y[n]*dt

        if not utils.is_planar(nodes_x, nodes_y, edges): return 0    





        # update time
        t += dt
      #  print(t)

        # plot in real time
        if plotRealTime :
            x_values=[]
            y_values=[]

            x_values_link=[]
            y_values_link=[]
            for x in range(num_nodes):
                x_values.append(nodes_x[x])
                y_values.append(nodes_y[x])
            
        
               

            plt.subplot(414)
            plt.subplot(414).cla()
                    
            for a in range(num_nodes):
                for b in range(a,num_nodes): 
                    if a!=b:
                        if spring_array[a][b]!=0:
                            
                        
                            x_spring=[]
                            y_spring=[]
                            x_spring.append(nodes_x[a])
                            x_spring.append(nodes_x[b])
                            y_spring.append(nodes_y[a])
                            y_spring.append(nodes_y[b])
                            plt.plot(x_spring,y_spring, 'bo', linestyle="--")
            
            x_values=[]
            y_values=[]

            x_values_link=[]
            y_values_link=[]
            for x in spring_points:
                x_values.append(nodes_x[x])
                y_values.append(nodes_y[x])
        
            plt.plot(x_values,y_values,'r')

            #ax.set(xlim=(0, box_size), ylim=(0, box_size))
            #ax.set_aspect('equal', 'box')
            plt.pause(0.001)
        if end_loop==1:
            break

   
    #compute fitness
    dist_diff01=0

    for a in range(num_nodes):
        for b in range(num_nodes):
            dist_diff01=dist_diff01+abs(distance_state0[a][b]-distance_state1[a][b])
            


    #print("distance",dist_diff01,dist_diff02,dist_diff12)


    #too_big_flag=0
    if too_big_flag==0 and kill==0 and intersection_flag==0:
        return dist_diff01
    else:
        return 0