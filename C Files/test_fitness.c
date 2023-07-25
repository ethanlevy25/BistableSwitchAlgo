#include <stdio.h>
#include "measure_fitness.h"
#include <stdbool.h>
#include <stdlib.h>

int main(void){
    // int num_nodes = 4;
    // double nodes_x[4] = {0.8374389569585708, 0.9417516939568017, 1.3541002593374527, 0.41872381196303654};
    // double nodes_y[4] = {1.1735608578366041, 0.5627884362410314, 0.9677545522401831, 0.8077409656461261};
    // int edges[5][2] = {
    //     {0, 1},
    //     {0, 2},
    //     {0, 3},
    //     {1, 3},
    //     {2, 1}
    // };
    // bool adj_mat[4][4] = {
    //     {false, true, true, true},
    //     {true, false, true, true},
    //     {true, true, false, false},
    //     {true, true, false, false}
    // };
    // double springs[5] = {11.289537180416076, 8.234243053617385, 9.902374075620097, 8.26717011771618, 10.745324005617832};

    int num_nodes = 4;

    // Allocate memory for nodes
    double *nodes_x = (double *) malloc(num_nodes * sizeof(double));
    double *nodes_y = (double *) malloc(num_nodes * sizeof(double));

    if (!nodes_x || !nodes_y) {
        // Failed to allocate memory
        perror("Failed to allocate memory for nodes");
        return 1;
    }

    // Assign values to nodes
    nodes_x[0] = 0.8374389569585708;
    nodes_x[1] = 0.9417516939568017;
    nodes_x[2] = 1.3541002593374527;
    nodes_x[3] = 0.41872381196303654;

    nodes_y[0] = 1.1735608578366041;
    nodes_y[1] = 0.5627884362410314;
    nodes_y[2] = 0.9677545522401831;
    nodes_y[3] = 0.8077409656461261;

    // Allocate memory for edges as int**
    int **edges = (int **) malloc(5 * sizeof(int *));
    for (int i = 0; i < 5; i++) {
        edges[i] = (int *) malloc(2 * sizeof(int));
        if (!edges[i]) {
            // Failed to allocate memory
            perror("Failed to allocate memory for edges row");
            return 1;
        }
    }

    // Assign values to edges
    int edge_values[5][2] = {
        {0, 1},
        {0, 2},
        {0, 3},
        {1, 3},
        {2, 1}
    };

    for (int i = 0; i < 5; i++) {
        edges[i][0] = edge_values[i][0];
        edges[i][1] = edge_values[i][1];
    }

    // Allocate memory for adjacency matrix
    bool **adj_mat = (bool **) malloc(num_nodes * sizeof(bool *));
    for (int i = 0; i < num_nodes; i++) {
        adj_mat[i] = (bool *) malloc(num_nodes * sizeof(bool));
        if (!adj_mat[i]) {
            // Failed to allocate memory
            perror("Failed to allocate memory for adjacency matrix row");
            return 1;
        }
    }

    // Assign values to adjacency matrix
    bool adj_values[4][4] = {
        {false, true, true, true},
        {true, false, true, true},
        {true, true, false, false},
        {true, true, false, false}
    };

    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            adj_mat[i][j] = adj_values[i][j];
        }
    }

    // Allocate memory for springs
    double *springs = (double *) malloc(5 * sizeof(double));
    if (!springs) {
        // Failed to allocate memory
        perror("Failed to allocate memory for springs");
        return 1;
    }

    // Assign values to springs
    springs[0] = 11.289537180416076;
    springs[1] = 8.234243053617385;
    springs[2] = 9.902374075620097;
    springs[3] = 8.26717011771618;
    springs[4] = 10.745324005617832;

    double fit = fitness(nodes_x, nodes_y, 4, true, adj_mat, edges, 5, springs);
    printf("Fitness: %f", fit);


    free(nodes_x);
    free(nodes_y);
    for (int i = 0; i < 5; i++) {
        free(edges[i]);
    }
    free(edges);
    for (int i = 0; i < num_nodes; i++) {
        free(adj_mat[i]);
    }
    free(adj_mat);
    free(springs);

    
    return 0;

    
}