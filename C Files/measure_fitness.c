#include <stdbool.h>
#include <utils.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <allocation_utils.h>
double fitness(double* pos_x, double* pos_y, int num_nodes, bool vis, bool** adj_mat, int** edges, int edge_count, double* spring_dict);

double fitness(double* pos_x, double* pos_y, int num_nodes, bool vis, bool** adj_mat, int** edges, int edge_count, double* spring_dict){
    
    //Copies passed pos_x and pos_y values to nodes_x and nodes_y
    double* nodes_x = allocateDoubleArray(num_nodes);
    memcpy(nodes_x, pos_x, sizeof(double)*num_nodes);

    double* nodes_y = allocateDoubleArray(num_nodes);
    memcpy(nodes_y, pos_y, sizeof(double)*num_nodes);
    

    //creates an empty spring array to hold lengths of edges
    double** spring_array = allocateDouble2DArray(num_nodes);
    
    //creates arrays of 0s for velocities, accelerations, and forces in both x and y for each node
    double* vel_x = callocDoubleArray(num_nodes);
    double* vel_y = callocDoubleArray(num_nodes);
    double* acc_x = callocDoubleArray(num_nodes);
    double* acc_y = callocDoubleArray(num_nodes);
    double* force_x = callocDoubleArray(num_nodes);
    double* force_y = callocDoubleArray(num_nodes);

    //Initializes spring_grid
    double** spring_grid = callocDouble2DArray(num_nodes);
    for (int i = 0; i < edge_count; i++){
        int a = edges[i][0];
        int b = edges[i][1];
        spring_grid[a][b] = spring_dict[i];
        spring_grid[b][a] = spring_dict[i];
    }

    //saves the endpoints of edge 0 as the spring points (to be actuated)
    int spring_points[2];
    memcpy(spring_points, edges[0], sizeof(int)*2);

    //sets too big and too close flags to 0
    bool too_big_flag = false;
    bool too_close_flag = false;

    //sets the standard for the distance between nodes considered to be too close or too big
    double too_big_val = 0.95;
    double too_close_val = 0.0001;
    double too_close_penalty = 0.25;
    

    //establishes the penalty to multiply fitness by if any edges intersect
    double intersection_penalty = 0.1;

    //creates bool array to track if an edge has intersected
    bool* intersections = (bool*) calloc(edge_count, sizeof(bool));
    if (!intersections){
        exit(1);
    }

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
    double** distance_state0 = callocDouble2DArray(num_nodes);
    double** distance_state1 = callocDouble2DArray(num_nodes);

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
            force_x[n] = 0;
            force_y[n] = 0;
            for (int k = 0; k < num_nodes; k++){
                if (n!=k && adj_mat[n][k]){
                    double dx = nodes_x[n] - nodes_x[k];
                    double dy = nodes_y[n] - nodes_y[k];
                    double cur_dist = sqrt(dx*dx + dy*dy);

                    double rest_length = spring_array[n][k];
                    double spring_coeff = spring_grid[n][k];

                    double force_mag = spring_coeff * (rest_length - cur_dist);

                    double f_x_component_norm = dx / cur_dist;
                    double f_y_component_norm = dy / cur_dist;

                    force_x[n] += f_x_component_norm*force_mag;
                    force_y[n] += f_y_component_norm*force_mag;

                }
            }
        }
        // record distance_state0
        if (state == 0 && first_time_in_state){
            first_time_in_state = false;
            for (int a = 0; a < num_nodes; a++){
                for (int b = a+1; b < num_nodes; b++){
                    double dx = nodes_x[a] - nodes_x[b];
                    double dy = nodes_y[a] - nodes_y[b];
                    double dist = sqrt(dx*dx + dy*dy);
                    distance_state0[a][b] = dist;
                    distance_state0[b][a] = dist;
                    if (dist > too_big_val){
                        too_big_flag = true;
                    } else if (dist < too_close_flag){
                        too_close_flag = 1;
                    }

                }
            }
        }

        // record distance_state1
        if (state == 2 && first_time_in_state){
            first_time_in_state = false;
            for (int a = 0; a < num_nodes; a++){
                for (int b = a+1; b < num_nodes; b++){
                    double dx = nodes_x[a] - nodes_x[b];
                    double dy = nodes_y[a] - nodes_y[b];
                    double dist = sqrt(dx*dx + dy*dy);
                    distance_state1[a][b] = dist;
                    distance_state1[b][a] = dist;
                    if (dist > too_big_val){
                        too_big_flag = true;
                    } else if (dist < too_close_val){
                        too_close_flag = true;
                    }

                }
            }
        }

        //actuate one spring if expanding
        if (state == 0){
            int a = spring_points[0];
            int b = spring_points[1];

            double dx = nodes_x[a] - nodes_x[b];
            double dy = nodes_y[a] - nodes_y[b];
            double cur_dist = qrt(dx*dx + dy*dy);

            double force_mag = -1 * spring_grid[a][b] * (0.1 - cur_dist);
            double f_x_component_norm = dx / cur_dist;
            double f_y_component_norm = dy / cur_dist;
            force_x[a] -= f_x_component_norm*force_mag;
            force_y[a] -= f_y_component_norm*force_mag;

            force_x[b] += f_x_component_norm*force_mag;
            force_y[b] += f_y_component_norm*force_mag;
        }

        double max_force = 0;
        //update node velocities, update max_force, and update node positions
        for (int n = 0; n < num_nodes; n++){
            // velocity updates
            vel_x[n] = vel_x[n] * damping + force_x[n];
            vel_y[n] = vel_y[n] * damping + force_y[n] + gravity;

            // max force upodates
            if (force_x[n] > max_force){
                max_force = force_x[n];
            }
            if (force_y[n] > max_force){
                max_force = force_y[n];
            }

            //position updates
            nodes_x[n] += vel_x[n] * dt;
            nodes_y[n] += vel_y[n] * dt;

        }

        if (max_force < 0.001){
            stability_counter++;
            if (stability_counter > 10){
                stability_counter = 0;
                state++;
                first_time_in_state = true;
            }
        } else {
            stability_counter = 0;
        }

        count_intersects(nodes_x, nodes_y, edges, edge_count, intersections);

        if (end_loop){
            break;
        }
    }

    double dist_diff = 0;

    //Should change to b = a + 1 ????
    for (int a = 0; a < num_nodes; a++){
            for (int b = 0; b < num_nodes; b++){
                dist_diff += fabs(distance_state0[a][b] - distance_state1[a][b]);
            }
    }
    
    int intersection_count = 0;
    for (int i = 0; i < edge_count; i++){
        if (intersections[i]){
            intersection_count++;
        }
    }

    if (too_close_flag){
        dist_diff *= too_close_penalty;
    }

    // Free 1D arrays
    free(nodes_x);
    free(nodes_y);
    free(vel_x);
    free(vel_y);
    free(acc_x);
    free(acc_y);
    free(force_x);
    free(force_y);
    free(intersections);

    // Free 2D arrays
    for (int i = 0; i < num_nodes; i++) {
        free(spring_array[i]);
        free(spring_grid[i]);
        free(distance_state0[i]);
        free(distance_state1[i]);
    }
    free(spring_array);
    free(spring_grid);
    free(distance_state0);
    free(distance_state1);
    
    if (!too_big_flag && !kill){
        if (intersection_count > 0){
            double percent_planar = ((double) edge_count - (double) intersection_count) / (double) edge_count;
            if (percent_planar == 0){
                percent_planar = 1/(double) edge_count * 0.5;
                return intersection_penalty * dist_diff * percent_planar;
            }
        }
        return dist_diff;
    }
    return 0;
    


    



}