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


int main(void){
    int population_size = 100;
    int generations = 100;
    int num_nodes = 8;

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
    
    bool*** population_matrices = (bool***) malloc(population_size*sizeof(bool**));
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
    double** population_springs = (double**) malloc(population_size*sizeof(double*));
    if (!population_springs){
        exit(0);
    }

    
    
    for (int individual = 0; individual < population_size; individual++){
        double* nodes_x;
        double* nodes_y;
        int** edges;
        bool** adj_mat;
        double* springs;
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
        adj_mat = (bool**) malloc(sizeof(bool*)*num_nodes);
        if (!adj_mat){
            exit(1);
        }
        for (int i = 0; i < num_nodes; i++){
            adj_mat[i] = (bool*) calloc(num_nodes, sizeof(bool));
            if (!adj_mat[i]){
                exit(1);
            }
        }
        springs = (double*) malloc(sizeof(double));
        if (!springs){
            exit(1);
        }

        for (int i = 0; i < num_nodes; i++){
            nodes_x[i] = randomDouble(0.5, 1.5);
            nodes_y[i] = randomDouble(0.5, 1.5); 
        }

        for (int node1 = 0; node1 < num_nodes; node1++){
            for (int node2 = node1+1; node2 < num_nodes; node2++){
                if (!intersects_with_any(node1, node2, nodes_x, nodes_y, edges, num_edges)){
                    adj_mat[node1][node2] = true;
                    adj_mat[node2][node1] = true;
                    edges[num_edges][0] = node1;
                    edges[num_edges][1] = node2;
                    springs[num_edges] = randomDouble(spring_min, spring_max);
                    num_edges++;
                    edges = realloc(edges, (num_edges+1)*sizeof(int*)); 
                    springs = realloc(springs, (num_edges+1)*sizeof(double));
                    if (!edges || !springs){
                        exit(1);
                    }
                    edges[num_edges] = (int*) malloc (sizeof(int)*2);
                    if (!edges[num_edges]){
                        exit(1);
                    }
                }
            }
        }
        this_fitness = fitness(nodes_x, nodes_y, num_nodes, false, adj_mat, edges, num_edges, springs);
        printf("%f", this_fitness);
        } while (this_fitness!= 0);
        
        
        population_x[individual] = nodes_x;
        population_y[individual] = nodes_y;
        population_matrices[individual] = adj_mat;
        population_edges[individual] = edges;
        population_springs[individual] = springs;
        population_edgecounts[individual] = num_edges;

    }
    for (int g = 0; g < generations; g++){
        printf("Generation %d\n", g);
        for (int i = 0; i < population_size; i++){
            fitness_values[i] = fitness(population_x[i], population_y[i], num_nodes, false, population_matrices[i], population_edges[i], population_edgecounts[i], population_springs[i]);
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
            
            bool*** new_population_matrices = (bool***) malloc(population_size*sizeof(bool**));
            if (!new_population_matrices){
                exit(1);
            }
            int* new_population_edgecounts = (int*) malloc(sizeof(int)*population_size);
            if (!population_edgecounts){
                exit(1);
            }

            int*** new_population_edges = (int***) malloc(population_size*sizeof(int**));
            if (!population_edges){
                exit(1);
            }
            double** new_population_springs = (double**) malloc(population_size*sizeof(double*));
            if (!population_springs){
                exit(0);
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
                bool** adj_mat = (bool**) malloc(sizeof(bool*)*num_nodes);
                if (!adj_mat){
                    exit(1);
                }
                for (int i = 0; i < num_nodes; i++){
                    adj_mat[i] = (bool*) calloc(num_nodes, sizeof(bool));
                    if (!adj_mat[i]){
                        exit(1);
                    }
                }
                double* springs = (double*) malloc(sizeof(double));
                if (!springs){
                    exit(1);
                }

                for (int node1 = 0; node1 < num_nodes; node1++){
                    for (int node2 = node1+1; node2 < num_nodes; node2++){
                        bool in1 = (population_matrices[sample1][node1][node2]);
                        bool in2 = (population_matrices[sample2][node1][node2]);
                        bool added = false;
                        double spring = -1;
                        if (in1 && in2){
                            double spring1, spring2;
                            for (int i = 0; i < population_edgecounts[sample1]; i++){
                                if (node1 == population_edges[sample1][i][0]  && node2 == population_edges[sample1][i][1]){
                                    spring1 = population_springs[sample1][i];
                                    break;
                                }
                            }
                            for (int i = 0; i < population_edgecounts[sample2]; i++){
                                if (node2 == population_edges[sample2][i][0]  && node2 == population_edges[sample2][i][1]){
                                    spring2 = population_springs[sample2][i];
                                    break;
                                }
                            }
                            spring = (spring1 + spring2)/2.0;
                            
                        } else if (in1 && genrand_real1() < edge_probability){
                            for (int i = 0; i < population_edgecounts[sample1]; i++){
                                if (node1 == population_edges[sample1][i][0]  && node2 == population_edges[sample1][i][1]){
                                    spring = population_springs[sample1][i];
                                    break;
                                }
                            }
                        } else if (in2 && genrand_real1()< edge_probability){
                            for (int i = 0; i < population_edgecounts[sample2]; i++){
                                if (node2 == population_edges[sample2][i][0]  && node2 == population_edges[sample2][i][1]){
                                    spring = population_springs[sample2][i];
                                    break;
                                }
                            }
                        } 
                        
                        if (spring >= 0){
                            edges[num_edges][0] = node1;
                            edges[num_edges][1] = node2;
                            adj_mat[node1][node2] = true;
                            adj_mat[node2][node1] = true;
                            springs[num_edges] = spring;
                            num_edges++;
                            edges = realloc(edges, (num_edges+1)*sizeof(int*)); //check if should be = edges
                            springs = realloc(springs, (num_edges+1)*sizeof(double));
                            if (!edges || !springs){
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
                    new_population_springs[k] = springs;
                    new_population_edgecounts[k] = num_edges;
                    


                }
                
                new_population_x[population_size-1] = population_x[best_location];
                new_population_y[population_size-1] = population_y[best_location];
                new_population_matrices[population_size-1] = population_matrices[best_location];
                new_population_edges[population_size-1] = population_edges[best_location];
                new_population_springs[population_size-1] = population_springs[best_location];
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
                        free(population_springs[i]);
                    }
                    
                    population_x[i] = new_population_x[i];
                    population_y[i] = new_population_y[i];
                    population_matrices[i] = new_population_matrices[i];
                    population_edges[i] = new_population_edges[i];
                    population_springs[i] = new_population_springs[i];
                    population_edgecounts[i] = new_population_edgecounts[i];
                }

                for (int i = 0; i < population_size-1; i++){
                    for (int n = 0; n < num_nodes; n++){
                        if (genrand_real1()< 0.25 ){
                            population_x[i][n]+=randomDouble(-1*alpha, alpha);
                        }
                        if (genrand_real1() < 0.25){
                            population_y[i][n]+=randomDouble(-1*alpha, alpha);
                        }
                    }
                    if (genrand_real1() < 0.5){
                        gradient_update(population_x[i], population_y[i], population_matrices[i], num_nodes, population_edges[i], population_edgecounts[i], population_springs[i]);
                    }
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
            fprintf(fp, "%f\n", population_springs[max_i][i]);
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
        free(population_springs[i]);
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
    free(population_springs);

    return(0);

    



    
}

