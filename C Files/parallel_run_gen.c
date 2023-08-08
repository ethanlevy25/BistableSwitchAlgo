#include <pthread.h>
#include <stdlib.h>
#include "utils.h"
#include "gradient.h"
#include "measure_fitness.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "allocation_utils.h"
#include "mt19937ar.h"

typedef struct {
    int start;
    int end;
    double **population_x;
    double **population_y;
    int num_nodes;
    double ***population_matrices;
    int ***population_edges;
    int *population_edgecounts;
    double *fitness_values;
    double alpha;
} ThreadArgs;

#define NUM_THREADS 6

void* calculate_fitness(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*) args;

    for (int i = threadArgs->start; i < threadArgs->end; i++) {
        threadArgs->fitness_values[i] = fitness(
            threadArgs->population_x[i], 
            threadArgs->population_y[i], 
            threadArgs->num_nodes, 
            false, 
            threadArgs->population_matrices[i], 
            threadArgs->population_edges[i], 
            threadArgs->population_edgecounts[i]
        );
    }

    return NULL;
}

void* update_population(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*) args;

    for (int i = threadArgs->start; i < threadArgs->end; i++) {
        for (int n = 0; n < threadArgs->num_nodes; n++) {
            if (genrand_real1() < 0.25) {
                threadArgs->population_x[i][n] += randomDouble(-1 * threadArgs->alpha, threadArgs->alpha);
            }
            if (genrand_real1() < 0.25) {
                threadArgs->population_y[i][n] += randomDouble(-1 * threadArgs->alpha, threadArgs->alpha);
            }
        }
        if (genrand_real1() < 0.5) {
            gradient_update(
                threadArgs->population_x[i], 
                threadArgs->population_y[i], 
                threadArgs->population_matrices[i], 
                threadArgs->num_nodes, 
                threadArgs->population_edges[i], 
                threadArgs->population_edgecounts[i]
            );
        }
    }

    return NULL;
}


int main(void){
    int population_size = 100;
    int generations = 100;
    int num_nodes = 10;

    double spring_min = 5;
    double spring_max = 15;

    double best = 0;
    int best_i = 0;

    double alpha = 0.1; //Learning rate
    double edge_probability = 0.75;

    double** population_x = (double**) malloc(sizeof(double*)*population_size);
    double** population_y = (double**) malloc(sizeof(double*)*population_size);
    if (!population_x || !population_y){
        exit(1);
    }
    for (int i = 0; i < population_size; i++){
        population_x[i] = (double*) malloc(sizeof(double)*num_nodes);
        population_y[i] = (double*) malloc(sizeof(double)*num_nodes);
        if (!population_x[i] || !population_y[i]){
            exit(1);
        }
    }
    
    double*** population_matrices = (double***) malloc(population_size*sizeof(double**));
    if (!population_matrices){
        exit(1);
    }
    double* fitness_values = allocateDoubleArray(population_size);
    int* population_edgecounts = (int*) malloc(sizeof(int)*population_size);
    if (!population_edgecounts){
        exit(1);
    }

    int*** population_edges = (int***) malloc(population_size*sizeof(int**));
    if (!population_edges){
        exit(1);
    }


    
    
    for (int individual = 0; individual < population_size; individual++){
        double* nodes_x;
        double* nodes_y;
        int** edges;
        double** adj_mat;
        int num_edges;
        double this_fitness;
        do{
        num_edges = 0;
        nodes_x = allocateDoubleArray(num_nodes);
        nodes_y = allocateDoubleArray(num_nodes);
        edges = (int**) malloc(sizeof(int*));
        if (!edges){
            exit(1);
        }
        edges[0] = (int*) malloc(sizeof(int)*2);
        if (!edges[0]){
            exit(0);
        }
        adj_mat = (double**) malloc(sizeof(double*)*num_nodes);
        if (!adj_mat){
            exit(1);
        }
        for (int i = 0; i < num_nodes; i++){
            adj_mat[i] = (double*) calloc(num_nodes, sizeof(double));
            if (!adj_mat[i]){
                exit(1);
            }
        }

        for (int i = 0; i < num_nodes; i++){
            nodes_x[i] = randomDouble(0.5, 1.5);
            nodes_y[i] = randomDouble(0.5, 1.5); 
        }

        for (int node1 = 0; node1 < num_nodes; node1++){
            for (int node2 = node1+1; node2 < num_nodes; node2++){
                if (!intersects_with_any(node1, node2, nodes_x, nodes_y, edges, num_edges)){
                    double spring = randomDouble(spring_min, spring_max);
                    adj_mat[node1][node2] = spring;
                    adj_mat[node2][node1] = spring;
                    edges[num_edges][0] = node1;
                    edges[num_edges][1] = node2;
                    num_edges++;
                    edges = realloc(edges, (num_edges+1)*sizeof(int*)); 
                    if (!edges){
                        exit(1);
                    }
                    edges[num_edges] = (int*) malloc (sizeof(int)*2);
                    if (!edges[num_edges]){
                        exit(1);
                    }
                }
            }
        }
        this_fitness = fitness(nodes_x, nodes_y, num_nodes, false, adj_mat, edges, num_edges);
        // printf("%f", this_fitness);
        } while (this_fitness == 0.0);
        
        
        population_x[individual] = nodes_x;
        population_y[individual] = nodes_y;
        population_matrices[individual] = adj_mat;
        population_edges[individual] = edges;
        population_edgecounts[individual] = num_edges;

    }
    for (int g = 0; g < generations; g++){
        printf("Generation %d\n", g);
        
    pthread_t threads[NUM_THREADS];
    ThreadArgs threadArgs[NUM_THREADS];
    int chunk_size = population_size / NUM_THREADS;

    for (int t = 0; t < NUM_THREADS; t++) {
        threadArgs[t].start = t * chunk_size;
        threadArgs[t].end = (t == NUM_THREADS - 1) ? population_size : (t + 1) * chunk_size;
        threadArgs[t].population_x = population_x;
        threadArgs[t].population_y = population_y;
        threadArgs[t].num_nodes = num_nodes;
        threadArgs[t].population_matrices = population_matrices;
        threadArgs[t].population_edges = population_edges;
        threadArgs[t].population_edgecounts = population_edgecounts;
        threadArgs[t].fitness_values = fitness_values;
        threadArgs[t].alpha = alpha;
        
        pthread_create(&threads[t], NULL, calculate_fitness, &threadArgs[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

        if (g < generations-1){
            double** new_population_x = (double**) malloc(sizeof(double*)*population_size);
            double** new_population_y = (double**) malloc(sizeof(double*)*population_size);
            if (!new_population_x || !new_population_y){
                exit(1);
            }
            for (int i = 0; i < population_size; i++){
                new_population_x[i] = (double*) malloc(sizeof(double)*num_nodes);
                new_population_y[i] = (double*) malloc(sizeof(double)*num_nodes);
                if (!population_x[i] || !population_y[i]){
                    exit(1);
                }
            }
            
            double*** new_population_matrices = (double***) malloc(population_size*sizeof(double**));
            if (!new_population_matrices){
                exit(1);
            }
            int* new_population_edgecounts = (int*) malloc(sizeof(int)*population_size);
            if (!new_population_edgecounts){
                exit(1);
            }

            int*** new_population_edges = (int***) malloc(population_size*sizeof(int**));
            if (!new_population_edges){
                exit(1);
            }
            double total_fitness = 0;
            int best_location = 0;
            double best_found = 0;
            for (int i = 0; i < population_size; i++){
                total_fitness+=fitness_values[i];
                if (fitness_values[i]>=best_found){
                    best_found = fitness_values[i];
                    best_location = i;
                }
            }
            printf("best found is %f location %d total fitness=%f alpha %f\n", best_found, best_location, total_fitness, alpha);
            if (best_location == population_size-1){
                if (alpha < 2){
                    alpha+=0.1;
                }
            } else{
                alpha = 0.1;
            }

            


            for (int k = 0; k < population_size-1; k++){
                double sample_point1 = randomDouble(0, total_fitness);
                double sample_point2 = randomDouble(0, total_fitness);

                int sample1 = 0;
                int sample2 = 0;
                double current_sample_point = 0;
                for (int s = 0; s < population_size; s++){
                    current_sample_point+=fitness_values[s];
                    if (current_sample_point < sample_point1){
                        sample1 = s+1;
                    }
                    if (current_sample_point < sample_point2){
                        sample2 = s+1;
                    }
                }

                double* child_x = allocateDoubleArray(num_nodes);
                double* child_y = allocateDoubleArray(num_nodes);
                int split = getRandInt(0, num_nodes-2);

                for (int sp = 0; sp < num_nodes; sp++){
                    if (sp < split){
                        child_x[sp] = population_x[sample1][sp];
                        child_y[sp] = population_y[sample1][sp];
                    } else {
                        child_x[sp] = population_x[sample2][sp];
                        child_y[sp] = population_y[sample2][sp];
                    }
                }
                int num_edges = 0;
                int** edges = (int**) malloc(sizeof(int*));
                if (!edges){
                    exit(1);
                }
                edges[0] = (int*) malloc(sizeof(int)*2);
                if (!edges[0]){
                    exit(1);
                }
                double** adj_mat = (double**) malloc(sizeof(double*)*num_nodes);
                if (!adj_mat){
                    exit(1);
                }
                for (int i = 0; i < num_nodes; i++){
                    adj_mat[i] = (double*) calloc(num_nodes, sizeof(double));
                    if (!adj_mat[i]){
                        exit(1);
                    }
                }

                for (int node1 = 0; node1 < num_nodes; node1++){
                    for (int node2 = node1+1; node2 < num_nodes; node2++){
                        double spring_val = -1.0;
                        if (node1 < split && node2 < split){
                            spring_val = population_matrices[sample1][node1][node2];
                        } else if (node1 >= split && node2 >= split){
                            spring_val = population_matrices[sample2][node1][node2];
                        } else {
                            if (genrand_real1() < (double) split / (double) num_nodes){
                                spring_val = population_matrices[sample1][node1][node2];
                            } else {
                                spring_val = population_matrices[sample2][node1][node2];
                            }
                        }
                        if (spring_val != 0){
                            adj_mat[node2][node1] = spring_val;
                            adj_mat[node1][node2] = spring_val;
                            edges[num_edges][0] = node1;
                            edges[num_edges][1] = node2;
                            num_edges++;
                            edges = realloc(edges, (num_edges+1)*sizeof(int*)); //check if should be = edges
                            if (!edges){
                                exit(1);
                            }
                            edges[num_edges] = (int*) malloc(sizeof(int)*2);
                        }
                        
                    }
                }


                new_population_x[k] = child_x;
                new_population_y[k] = child_y;
                new_population_matrices[k] = adj_mat;
                new_population_edges[k] = edges;
                new_population_edgecounts[k] = num_edges;
                    


                }
                
                new_population_x[population_size-1] = population_x[best_location];
                new_population_y[population_size-1] = population_y[best_location];
                new_population_matrices[population_size-1] = population_matrices[best_location];
                new_population_edges[population_size-1] = population_edges[best_location];
                new_population_edgecounts[population_size-1] = population_edgecounts[best_location];
                for (int i = 0; i < population_size; i++){
                    if (i != best_location){
                        free(population_x[i]);
                        free(population_y[i]);
                        for (int j = 0; j < num_nodes; j++){
                            free(population_matrices[i][j]);
                        }
                        free(population_matrices[i]);
                        for (int j = 0; j < population_edgecounts[i]; j++){
                            free(population_edges[i][j]);
                        }
                        free(population_edges[i]);
                    }
                    
                    population_x[i] = new_population_x[i];
                    population_y[i] = new_population_y[i];
                    population_matrices[i] = new_population_matrices[i];
                    population_edges[i] = new_population_edges[i];
                    population_edgecounts[i] = new_population_edgecounts[i];
                }

                
    for (int t = 0; t < NUM_THREADS; t++) {
        threadArgs[t].start = t * chunk_size;
        threadArgs[t].end = (t == NUM_THREADS - 1) ? population_size - 1 : (t + 1) * chunk_size;
        
        pthread_create(&threads[t], NULL, update_population, &threadArgs[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }


                

            
        }

            


    }


    double max_fitness = 0;
    int max_i = 0;
    for (int i = 0; i < population_size; i++){
        if (fitness_values[i]>max_fitness){
            max_fitness = fitness_values[i];
            max_i = i;
        }
    }
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", timeinfo);
    char filename[100];
    sprintf(filename, "results_%s.csv", buffer);

    FILE *fp = fopen(filename, "w");

    // Fetch the length of the edge array for the best individual
    int best_edge_count = population_edgecounts[max_i];

    // Iterate over the best individual's data
    for (int i = 0; i < num_nodes || i < best_edge_count; i++) {
        // Write x and y values if within bounds
        if (i < num_nodes) {
            fprintf(fp, "%f,%f,", population_x[max_i][i], population_y[max_i][i]);
        } else {
            fprintf(fp, ",,");
        }

        // Write edge values if within bounds
        if (i < best_edge_count) {
            fprintf(fp, "%d-%d,", population_edges[max_i][i][0], population_edges[max_i][i][1]);
            fprintf(fp, "%f,", population_matrices[max_i][population_edges[max_i][i][0]][population_edges[max_i][i][1]]);
        } else {
            fprintf(fp, ",\n");
        }
    }

    fclose(fp); // Close the file

    printf("Max fitness: %f\n", max_fitness);

        // Freeing 1D arrays
    free(fitness_values);
    // Freeing 2D arrays
    for (int i = 0; i < population_size; i++) {
        free(population_x[i]);
        free(population_y[i]);
        // Free the 2D arrays inside the 3D array
        for (int j = 0; j < num_nodes; j++) {
            free(population_matrices[i][j]);
        }
        
        // Free the edge arrays
        for (int j = 0; j < population_edgecounts[i]; j++) {
            free(population_edges[i][j]);
        }
        free(population_matrices[i]);
        free(population_edges[i]);
    }
    
    // Free the outer arrays
    free(population_edgecounts);
    free(population_x);
    free(population_y);
    free(population_matrices);
    free(population_edges);

    return(0);

    



    
}

