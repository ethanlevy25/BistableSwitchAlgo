
#ifndef MEASURE_FITNESS_H
#define MEASURE_FITNESS_H

#include <stdbool.h>
#include "utils.h"
#include <math.h>
#include <stdlib.h>

double fitness(double* pos_x, double* pos_y, int num_nodes, bool vis, bool** adj_mat, int** edges, int edge_count, double* spring_dict);

#endif // MEASURE_FITNESS_H
