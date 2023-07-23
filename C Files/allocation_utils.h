
#ifndef ALLOCATION_UTILS_H
#define ALLOCATION_UTILS_H

#include <stdlib.h>

double* allocateDoubleArray(int num_elements);
double** allocateDouble2DArray(int num_elements);
double* callocDoubleArray(int num_elements);
double** callocDouble2DArray(int num_elements);

#endif // ALLOCATION_UTILS_H
