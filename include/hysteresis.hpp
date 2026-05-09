#ifndef HYSTERESIS_HPP
#define HYSTERESIS_HPP
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void follow_edges(unsigned char *edgemapptr, short *edgemagptr, short lowval, int cols);
void apply_hysteresis(short int *mag, unsigned char *nms, int rows, int cols, float tlow, float thigh, unsigned char *edge);
#endif