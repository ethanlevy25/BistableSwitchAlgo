#include <stdbool.h>
#include <stdlib.h>

bool connected(bool** adj_mat, int size);
bool ccw(double* A, double* B, double* C);
bool intersect(double* A, double* B, double* C, double* D);
bool intersects_with_any(int node1, int node2, double* nodes_x, double* nodes_y, int** edges, int edge_count);
bool is_planar(double* nodes_x, double* nodes_y, int** edges, int edge_count);
void count_intersects(double* nodes_x, double* nodes_y, int** edges, int edge_count, bool* intersections);

bool connected(bool** adj_mat, int size){
    int* stack = (int*)malloc(size * sizeof(int));
    if (!stack) {
        exit(1);
    }   
    int stack_ptr = 0;
    
    bool* visited = (bool*)calloc(size, sizeof(bool));
    if (!visited) {
        exit(1);
    }   
    stack[stack_ptr++] = 0;

    int visited_count = 0;

    while (stack_ptr > 0){
        int node = stack[--stack_ptr];
        if (visited[node]){
            continue;
        }
        visited[node] = true;
        visited_count++;
        
        for (int i = 0; i < size; i++){
            if (adj_mat[node][i] && !visited[i]){
                stack[stack_ptr++] = i;
          }
        }

    }
    free(stack);
    free(visited);
    return size == visited_count;

}

bool ccw(double* a, double* b, double* c){
    return (c[1]-a[1])*(b[0]-a[0]) > (b[1]-a[1])*(c[0]-a[0]);
}

bool intersect(double* a, double* b, double* c, double* d){
    bool ccw_acd = ccw(a, c, d);
    bool ccw_bcd = ccw(b, c, d);

    bool ccw_abc = ccw(a, b, c);
    bool ccw_abd = ccw (a, b, d);

    return ccw_acd != ccw_bcd && ccw_abc != ccw_abd;

}

bool intersects_with_any(int node1, int node2, double* nodes_x, double* nodes_y, int** edges, int edge_count){
    double node1_val[2] = {nodes_x[node1], nodes_y[node1]};
    double node2_val[2] = {nodes_x[node2], nodes_y[node2]};
    
    for (int i = 0; i < edge_count; i++){
        int edgepoint1 = edges[i][0];
        int edgepoint2 = edges[i][1];        

        double edgepoint1_val[2] = {nodes_x[edgepoint1], nodes_y[edgepoint1]};
        double edgepoint2_val[2] = {nodes_x[edgepoint2], nodes_y[edgepoint2]};

        if (intersect(node1_val, node2_val, edgepoint1_val, edgepoint2_val)){
            return true;
        }
    }
    return false;

}

bool is_planar(double* nodes_x, double* nodes_y, int** edges, int edge_count){

    for (int i = 0; i < edge_count; i++){
        double a[2] = {nodes_x[edges[i][0]], nodes_y[edges[i][0]]};
        double b[2] = {nodes_x[edges[i][1]], nodes_y[edges[i][1]]};
        for (int ii = i+1; ii < edge_count; ii++){
            double c[2] = {nodes_x[edges[ii][0]], nodes_y[edges[ii][0]]};
            double d[2] = {nodes_x[edges[ii][1]], nodes_y[edges[ii][1]]};
            if (intersect(a, b, c, d)){
                return false;
            }
        }
    }
    return true;
}

void count_intersects(double* nodes_x, double* nodes_y, int** edges, int edge_count, bool* intersections){
    for (int i = 0; i < edge_count; i++){
        if (intersections[i]){
            continue;
        }
        double a[2] = {nodes_x[edges[i][0]], nodes_y[edges[i][0]]};
        double b[2] = {nodes_x[edges[i][1]], nodes_y[edges[i][1]]};
        for (int ii = 0; ii < edge_count; ii++){
            if (i == ii){
                continue;
            }
            double c[2] = {nodes_x[edges[ii][0]], nodes_y[edges[ii][0]]};
            double d[2] = {nodes_x[edges[ii][1]], nodes_y[edges[ii][1]]};
            if (intersect(a,b,c,d)){
                intersections[i] = true;
                intersections[ii] = true;
                break;
            }

        }

    } 
}