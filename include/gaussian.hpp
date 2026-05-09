#ifndef GAUSSIAN_HPP
#define GAUSSIAN_HPP
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void gaussian_smooth(unsigned char *image, int rows, int cols, float sigma, short int **smoothedim);
void make_gaussian_kernel(float sigma, float **kernel, int *windowsize);
#endif