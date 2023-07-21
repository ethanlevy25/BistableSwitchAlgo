#include <stdbool.h>
#include <utils.h>
#include <math.h>

double fitness(double* pos_x, double* pos_y, int num_nodes, bool vis, bool** adj_mat, int** edges, int edge_count, double* spring_dict){
    
    //Copies passed pos_x and pos_y values to nodes_x and nodes_y
    double nodes_x[num_nodes];
    memcpy(nodes_x, pos_x, sizeof(pos_x));

    double nodes_y[num_nodes];
    memcpy(nodes_y, pos_y, sizeof(pos_x));

    //creates an empty spring array to hold lengths of edges
    double spring_array[num_nodes][num_nodes];
    
    //creates arrays of 0s for velocities, accelerations, and forces in both x and y for each node
    double vel_x[num_nodes];
    memset(vel_x, 0, sizeof(vel_x));

    double vel_y[num_nodes];
    memset(vel_y, 0, sizeof(vel_y));

    double acc_x[num_nodes];
    memset(acc_x, 0, sizeof(acc_x));

    double acc_y[num_nodes];
    memset(acc_y, 0, sizeof(acc_y));

    double force_x[num_nodes];
    memset(force_x, 0, sizeof(force_x));

    double force_y[num_nodes];
    memset(force_y, 0, sizeof(force_y));

    double spring_grid[num_nodes][num_nodes];
    for (int i = 0; i < edge_count; i++){
        int a = edges[i][0];
        int b = edges[i][1];
        spring_grid[a][b] = spring_dict[i];
        spring_grid[b][a] = spring_dict[i];
    }

    //saves the endpoints of edge 0 as the spring points (to be actuated)
    int spring_points[2] = edges[0];

    //sets too big and too close flags to 0
    bool too_big_flag = false;
    bool too_close_flag = false;

    //sets the standard for the distance between nodes considered to be too close
    double too_close_val = 0.0001;

    //establishes the penalty to multiply fitness by if any edges intersect
    double intersection_penalty = 0.1;

    //creates bool array to track if an edge has intersected
    bool intersections[edge_count];
    memset(intersections, 0, sizeof(intersections));

    bool first_time_in_state = true;

    for (int a = 0; a < num_nodes; a++){
        for (int b = 0; b < num_nodes; b++){
            if (a < b){
                if (adj_mat[a][b]){
                    double dx = nodes_x[a] - nodes_x[b];
                    double dy = nodes_y[a] - nodes_y[b];
                    spring_array[a][b] = sqrt(dx*dx + dy*dy);
                } else { 
                    spring_array[a][b] = 0;
                }
            } else {
                spring_array[a][b] = spring_array[b][a];
            }

        }
    }

    double distance_state0[num_nodes][num_nodes];
    double distance_state1[num_nodes][num_nodes];

    double dt = 0.01; // timestep size
    double Nt = 601; // number of timesteps
    double gravity = -0.0; // strength of gravity
    double damping = 0.2; // ???

    int state = 0;
    bool kill = false;
    bool end_loop = false;
    int stability_counter = 0;

    for (int time_steps = 0; time_steps < Nt; time_steps++){
        if ((Nt-2) < time_steps){
            kill = true;
            end_loop = true;
        }

        //find f using hooke's law
        for (int n = 0; n < num_nodes; n++){
            force_x[n] = 0
        }

    }

    



}