#include <stdlib.h>

double* allocateDoubleArray(int num_elements);
double** allocateDouble2DArray(int num_elements);
double* callocDoubleArray(int num_elements);
double** callocDouble2DArray(int num_elements);
double*** allocateDouble3DArray(int num_arrays, int num_elements);

double* allocateDoubleArray(int num_elements) {
    double* ptr = (double*) malloc(sizeof(double)*num_elements);
    if (!ptr){
        exit(1);
    }
    return ptr;
}

double** allocateDouble2DArray(int num_elements) {
    double** array2D = (double**) malloc(sizeof(double*)*num_elements);
    if (!array2D){
        exit(1);
    }
    for (int i = 0; i < num_elements; i++) {
        array2D[i] = allocateDoubleArray(num_elements);
    }
    return array2D;
}

double* callocDoubleArray(int num_elements) {
    double* ptr = (double*) calloc(num_elements, sizeof(double));
    if (!ptr) {
        exit(1);
    }
    return ptr;
}

double** callocDouble2DArray(int num_elements){
    double** array2D = (double**) malloc(sizeof(double*) * num_elements);
    if (!array2D){
        exit(1);
    }
    for (int i = 0; i < num_elements; i++) {
        array2D[i] = callocDoubleArray(num_elements);
    }
    return array2D;
}

double*** allocateDouble3DArray(int num_arrays, int num_elements){
    double*** array3D = (double***) malloc(sizeof(double**)*num_arrays);
    if (!array3D){
        exit(1);
    }
    for (int i = 0; i<num_arrays; i++){
        array3D[i] = allocateDouble2DArray(num_elements);
    }
    return array3D;
}