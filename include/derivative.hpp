#ifndef DERIVATIVE_HPP
#define DERIVATIVE_HPP
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void derrivative_x_y(short int *smoothedim, int rows, int cols, short int **delta_x, short int **delta_y);
void magnitude_x_y(short int *delta_x, short int *delta_y, int rows, int cols, short int **magnitude);
void radian_direction(short int *delta_x, short int *delta_y, int rows, int cols, float **dir_radians, int xdirtag, int ydirtag);
double angle_radians(double x, double y);
#endif