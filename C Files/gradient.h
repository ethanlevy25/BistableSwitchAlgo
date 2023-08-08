#ifndef GRADIENT_H
#define GRADIENT_H

#include <stdbool.h>
#include "utils.h"
#include <math.h>
#include <stdlib.h>
#include "allocation_utils.h"

#define MAX_ITERS 100000

void gradient_update(double* nodes_x, double* nodes_y, double** adj_mat, int num_nodes, int** edges, int edge_count);

#endif // GRADIENT_H
