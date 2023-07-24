#include <stdlib.h>
#include <utils.h>
#include <gradient.h>
#include <measure_fitness.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <allocation_utils.h>


typedef int edge[2];

int main(void){
    int population_size = 20;
    int generations = 10;
    int num_nodes = 4;

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

    edge** population_edges = (int***) malloc(population_size*sizeof(edge*));
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
        edge* edges;
        bool** adj_mat;
        double* springs;
        int num_edges;
        do{
        num_edges = 0;
        nodes_x = allocateDoubleArray(num_nodes);
        nodes_y = allocateDoubleArray(num_nodes);
        edges = (edge*) malloc(sizeof(edge));
        if (!edges){
            exit(1);
        }
        adj_mat = (bool**) malloc(sizeof(bool*)*num_nodes);
        if (!adj_mat){
            exit(1);
        }
        for (int i = 0; i < num_nodes; i++){
            adj_mat[i] = (bool*) calloc(sizeof(bool)*num_nodes);
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
                    edges = realloc(edges, (num_edges)*sizeof(edge)); //check if should be = edges
                    springs = realloc(springs, (num_edges)*sizeof(double));
                    if (!edges || !springs){
                        exit(1);
                    }
                }
            }
        }
        } while (fitness(nodes_x, nodes_y, num_nodes, false, adj_mat, edges, num_edges, springs) != 0);
        
        population_x[individual] = nodes_x;
        population_y[individual] = nodes_y;
        population_matrices[individual] = adj_mat;
        population_edges[individual] = edges;
        population_springs[individual] = springs;
        population_edgecounts[individual] = num_edges;

    }
    for (int g = 0; g < generations; g++){
        for (int i = 0; i < population_size; i++){
            fitness_values[i] = fitness(population_x[i], population_y[i], num_nodes, false, population_matrices[i], population_edges[i], population_edgecounts[i], population_springs[i]);
        
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

            edge** new_population_edges = (int***) malloc(population_size*sizeof(edge*));
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

            if (best_location == population_size-1){
                if (alpha < 2){
                    alpha+=0.1;
                }
            } else{
                alpha = 0.1;
            }

            //PRINT BEST FOUND

            for (int k = 0; k < population_size-1; k++){
                double sample_point1;// = random(0,total_fitness);
                double sample_point2;// = random(0, total_fitness);

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
                int split; // = random(0,num_nodes-1)

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
                edge* edges = (edge*) malloc(sizeof(edge));
                if (!edges){
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
                            
                        } else if (in1 && rand() < RAND_MAX * edge_probability){
                            for (int i = 0; i < population_edgecounts[sample1]; i++){
                                if (node1 == population_edges[sample1][i][0]  && node2 == population_edges[sample1][i][1]){
                                    spring = population_springs[sample1][i];
                                    break;
                                }
                            }
                        } else if (in2 && rand() < RAND_MAX * edge_probability){
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
                            edges = realloc(edges, (num_edges)*sizeof(edge)); //check if should be = edges
                            springs = realloc(springs, (num_edges)*sizeof(double));
                            if (!edges || !springs){
                                exit(1);
                            }
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

                

            }
        }    
            


    }

    



    
}

