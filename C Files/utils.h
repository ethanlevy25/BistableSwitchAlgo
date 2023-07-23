
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

bool connected(bool** adj_mat, int size);
bool ccw(double* A, double* B, double* C);
bool intersect(double* A, double* B, double* C, double* D);
bool intersects_with_any(int node1, int node2, double* nodes_x, double* nodes_y, int** edges, int edge_count);
bool is_planar(double* nodes_x, double* nodes_y, int** edges, int edge_count);
void count_intersects(double* nodes_x, double* nodes_y, int** edges, int edge_count, bool* intersections);

#endif // UTILS_H
