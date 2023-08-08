#include <math.h>
#include "measure_fitness.h"
#include <stdbool.h>
#include <stdlib.h>
#include "allocation_utils.h"
#include <string.h>

#define MAX_ITERS 100000
void gradient_update(double* nodes_x, double* nodes_y, double** adj_mat, int num_nodes, int** edges, int edge_count);

void gradient_update(double* nodes_x, double* nodes_y, double** adj_mat, int num_nodes, int** edges, int edge_count){
    double current_fit = fitness(nodes_x, nodes_y, num_nodes, false, adj_mat, edges, edge_count);
    double best_value = current_fit;

    double* best_x = allocateDoubleArray(num_nodes);
    memcpy(best_x, nodes_x, sizeof(double)*num_nodes);
    double* best_y = allocateDoubleArray(num_nodes);
    memcpy(best_y, nodes_y, sizeof(double)*num_nodes);

    double step = 0.01;
    for (int i = 0; i < MAX_ITERS; i++){

        for (int a = 0; a < num_nodes; a++){
            double* temp_x = allocateDoubleArray(num_nodes);
            memcpy(temp_x, nodes_x, sizeof(double)*num_nodes);

            double* temp_y = allocateDoubleArray(num_nodes);
            memcpy(temp_y, nodes_y, sizeof(double)*num_nodes);

            temp_x[a] += step;
            double temp_fit = fitness(temp_x, temp_y, num_nodes, false, adj_mat, edges, edge_count);
            if (temp_fit > best_value){
                best_value = temp_fit;
                memcpy(best_x, temp_x, sizeof(double)*num_nodes);
                memcpy(best_y, temp_y, sizeof(double)*num_nodes);
            }

            temp_x[a] -= 2*step; // *2 to overcorrect for opposite last time
            temp_fit = fitness(temp_x, temp_y, num_nodes, false, adj_mat, edges, edge_count);
            if (temp_fit > best_value){
                best_value = temp_fit;
                memcpy(best_x, temp_x, sizeof(double)*num_nodes);
                memcpy(best_y, temp_y, sizeof(double)*num_nodes);
            }

            temp_x[a] += step; // reset to base
            temp_y[a] += step;
            temp_fit = fitness(temp_x, temp_y, num_nodes, false, adj_mat, edges, edge_count);
            if (temp_fit > best_value){
                best_value = temp_fit;
                memcpy(best_x, temp_x, sizeof(double)*num_nodes);
                memcpy(best_y, temp_y, sizeof(double)*num_nodes);
            }

            temp_y[a] -= 2*step;
            temp_fit = fitness(temp_x, temp_y, num_nodes, false, adj_mat, edges, edge_count);
            if (temp_fit > best_value){
                best_value = temp_fit;
                memcpy(best_x, temp_x, sizeof(double)*num_nodes);
                memcpy(best_y, temp_y, sizeof(double)*num_nodes);
            }
            free(temp_x);
            free(temp_y);
        }
        if (best_value <= current_fit){
                break;
            } else {
                current_fit = best_value;
            }
    }
    memcpy(nodes_x, best_x, sizeof(double)*num_nodes);
    memcpy(nodes_y, best_y, sizeof(double)*num_nodes);
}