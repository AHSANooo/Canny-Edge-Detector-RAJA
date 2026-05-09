#include "derivative.hpp"
#define VERBOSE 0

/*******************************************************************************
* Procedure: radian_direction
* Purpose: To compute a direction of the gradient image from component dx and
* dy images. Because not all derriviatives are computed in the same way, this
* code allows for dx or dy to have been calculated in different ways.
*
* FOR X:  xdirtag = -1  for  [-1 0  1]
*         xdirtag =  1  for  [ 1 0 -1]
*
* FOR Y:  ydirtag = -1  for  [-1 0  1]'
*         ydirtag =  1  for  [ 1 0 -1]'
*
* The resulting angle is in radians measured counterclockwise from the
* xdirection. The angle points "up the gradient".
*******************************************************************************/
void radian_direction(short int *delta_x, short int *delta_y, int rows,
    int cols, float **dir_radians, int xdirtag, int ydirtag)
{
   int r, c, pos;
   float *dirim=NULL;
   double dx, dy;

   /****************************************************************************
   * Allocate an image to store the direction of the gradient.
   ****************************************************************************/
   if((dirim = (float *) calloc(rows*cols, sizeof(float))) == NULL){
      fprintf(stderr, "Error allocating the gradient direction image.\n");
      exit(1);
   }
   *dir_radians = dirim;

   for(r=0,pos=0;r<rows;r++){
      for(c=0;c<cols;c++,pos++){
         dx = (double)delta_x[pos];
         dy = (double)delta_y[pos];

         if(xdirtag == 1) dx = -dx;
         if(ydirtag == -1) dy = -dy;

         dirim[pos] = (float)angle_radians(dx, dy);
      }
   }
}

/*******************************************************************************
* FUNCTION: angle_radians
* PURPOSE: This procedure computes the angle of a vector with components x and
* y. It returns this angle in radians with the answer being in the range
* 0 <= angle <2*PI.
*******************************************************************************/
double angle_radians(double x, double y)
{
   double xu, yu, ang;

   xu = fabs(x);
   yu = fabs(y);

   if((xu == 0) && (yu == 0)) return(0);

   ang = atan(yu/xu);

   if(x >= 0){
      if(y >= 0) return(ang);
      else return(2*M_PI - ang);
   }
   else{
      if(y >= 0) return(M_PI - ang);
      else return(M_PI + ang);
   }
}

/*******************************************************************************
* PROCEDURE: magnitude_x_y
* PURPOSE: Compute the magnitude of the gradient. This is the square root of
* the sum of the squared derivative values.
* NAME: Mike Heath
* DATE: 2/15/96
*******************************************************************************/
void magnitude_x_y(short int *delta_x, short int *delta_y, int rows, int cols,
        short int **magnitude)
{
   int r, c, pos, sq1, sq2;

   /****************************************************************************
   * Allocate an image to store the magnitude of the gradient.
   ****************************************************************************/
   if((*magnitude = (short *) calloc(rows*cols, sizeof(short))) == NULL){
      fprintf(stderr, "Error allocating the magnitude image.\n");
      exit(1);
   }

   for(r=0,pos=0;r<rows;r++){
      for(c=0;c<cols;c++,pos++){
         sq1 = (int)delta_x[pos] * (int)delta_x[pos];
         sq2 = (int)delta_y[pos] * (int)delta_y[pos];
         (*magnitude)[pos] = (short)(0.5 + sqrt((float)sq1 + (float)sq2));
      }
   }

}

/*******************************************************************************
* PROCEDURE: derrivative_x_y
* PURPOSE: Compute the first derivative of the image in both the x any y
* directions. The differential filters that are used are:
*
*                                          -1
*         dx =  -1 0 +1     and       dy =  0
*                                          +1
*
* NAME: Mike Heath
* DATE: 2/15/96
*******************************************************************************/
void derrivative_x_y(short int *smoothedim, int rows, int cols,
        short int **delta_x, short int **delta_y)
{
    printf("\n===================== Derivate_x_y =====================\n");
   int r, c, pos;
   clock_t ini_derivate,fin_derivate;
   ini_derivate = clock();

   /****************************************************************************
   * Allocate images to store the derivatives.
   ****************************************************************************/
   if(((*delta_x) = (short *) calloc(rows*cols, sizeof(short))) == NULL){
      fprintf(stderr, "Error allocating the delta_x image.\n");
      exit(1);
   }
   if(((*delta_y) = (short *) calloc(rows*cols, sizeof(short))) == NULL){
      fprintf(stderr, "Error allocating the delta_x image.\n");
      exit(1);
   }

   /****************************************************************************
   * Compute the x-derivative. Adjust the derivative at the borders to avoid
   * losing pixels.
   ****************************************************************************/
   clock_t ini_derivate_x, fin_derivate_x;
   ini_derivate_x = clock();
   if(VERBOSE) printf("   Computing the X-direction derivative.\n");
   for(r=0;r<rows;r++){
      pos = r * cols;
      (*delta_x)[pos] = smoothedim[pos+1] - smoothedim[pos];
      pos++;
      for(c=1;c<(cols-1);c++,pos++){
         (*delta_x)[pos] = smoothedim[pos+1] - smoothedim[pos-1];
      }
      (*delta_x)[pos] = smoothedim[pos] - smoothedim[pos-1];
   }
   fin_derivate_x = clock();
   double secs_derivate_x = (double)(fin_derivate_x - ini_derivate_x) / CLOCKS_PER_SEC;
   printf("Tiempo primer bloque serial: \t\t%.5g	segundos\n", secs_derivate_x);

   /****************************************************************************
   * Compute the y-derivative. Adjust the derivative at the borders to avoid
   * losing pixels.
   ****************************************************************************/
   clock_t ini_derivate_y, fin_derivate_y;
   ini_derivate_y = clock();
   if(VERBOSE) printf("   Computing the Y-direction derivative.\n");
   // for(c=0;c<cols;c++){
   //    pos = c;
   //    (*delta_y)[pos] = smoothedim[pos+cols] - smoothedim[pos];
   //    pos += cols;
   //    for(r=1;r<(rows-1);r++,pos+=cols){
   //       (*delta_y)[pos] = smoothedim[pos+cols] - smoothedim[pos-cols];
   //    }
   //    (*delta_y)[pos] = smoothedim[pos] - smoothedim[pos-cols];
   // }
   //

    for(r=0;r<cols;r++)
        (*delta_y)[r] = smoothedim[r+cols] - smoothedim[r];
    for(r=cols;r<(rows*cols)-cols;r++)
        (*delta_y)[r] = smoothedim[r+cols] - smoothedim[r-cols];
    for(r=(rows*cols)-cols;r<rows*cols;r++)
        (*delta_y)[r] = smoothedim[r] - smoothedim[r-cols];

   // for(int i=0;i<10;i++){printf("%d - ",smoothedim[i]);}

   fin_derivate_y = clock();
   double secs_derivate_y = (double)(fin_derivate_y - ini_derivate_y) / CLOCKS_PER_SEC;
   printf("Tiempo segundo bloque serial: \t\t%.5g	segundos\n", secs_derivate_y);
   fin_derivate = clock();
   double secs_derivate = (double)(fin_derivate - ini_derivate) / CLOCKS_PER_SEC;
   printf("Tiempo total funcion: \t\t\t%.5g	segundos\n", secs_derivate);
   printf("========================================================\n");

}

