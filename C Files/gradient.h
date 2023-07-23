#ifndef GRADIENT_H
#define GRADIENT_H

#include <stdbool.h>
#include <utils.h>
#include <math.h>
#include <stdlib.h>

#define MAX_ITERS 100000

void gradient_update(double* nodes_x, double* nodes_y, bool** adj_mat, int num_nodes, int** edges, int edge_count, double* springs);

#endif // GRADIENT_H
