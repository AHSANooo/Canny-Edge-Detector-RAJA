#ifndef NMS_HPP
#define NMS_HPP
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void non_max_supp(short *mag, short *gradx, short *grady, int nrows, int ncols, unsigned char *result);
#endif