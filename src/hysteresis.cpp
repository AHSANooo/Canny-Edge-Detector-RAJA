#include "hysteresis.hpp"
#define VERBOSE 0
#define NOEDGE 255
#define POSSIBLE_EDGE 128
#define EDGE 0

/*******************************************************************************
* PROCEDURE: follow_edges
* PURPOSE: This procedure edges is a recursive routine that traces edgs along
* all paths whose magnitude values remain above some specifyable lower
* threshhold.
* NAME: Mike Heath
* DATE: 2/15/96
*******************************************************************************/
void follow_edges(unsigned char *edgemapptr, short *edgemagptr, short lowval,
   int cols)
{
   short *tempmagptr;
   unsigned char *tempmapptr;
   int i;
   float thethresh;
   int x[8] = {1,1,0,-1,-1,-1,0,1},
       y[8] = {0,1,1,1,0,-1,-1,-1};

   for(i=0;i<8;i++){
      tempmapptr = edgemapptr - y[i]*cols + x[i];
      tempmagptr = edgemagptr - y[i]*cols + x[i];

      if((*tempmapptr == POSSIBLE_EDGE) && (*tempmagptr > lowval)){
         *tempmapptr = (unsigned char) EDGE;
         follow_edges(tempmapptr,tempmagptr, lowval, cols);
      }
   }
}

/*******************************************************************************
* PROCEDURE: apply_hysteresis
* PURPOSE: This routine finds edges that are above some high threshhold or
* are connected to a high pixel by a path of pixels greater than a low
* threshold.
* NAME: Mike Heath
* DATE: 2/15/96
*******************************************************************************/
void apply_hysteresis(short int *mag, unsigned char *nms, int rows, int cols,
	float tlow, float thigh, unsigned char *edge)
{
    printf("\n==================== Appy_Hysteresis ===================\n" );
    clock_t ini_hyst,fin_hyst;
    ini_hyst = clock();
   int r, c, pos, numedges, lowcount, highcount, lowthreshold, highthreshold,
       i, hist[32768], rr, cc;
   short int maximum_mag, sumpix;

   /****************************************************************************
   * Initialize the edge map to possible edges everywhere the non-maximal
   * suppression suggested there could be an edge except for the border. At
   * the border we say there can not be an edge because it makes the
   * follow_edges algorithm more efficient to not worry about tracking an
   * edge off the side of the image.
   ****************************************************************************/
   clock_t ini_hyst_first_for,fin_hyst_first_for;
   ini_hyst_first_for = clock();

   for(r=0,pos=0;r<rows;r++){
      for(c=0;c<cols;c++,pos++){
	 if(nms[pos] == POSSIBLE_EDGE) edge[pos] = POSSIBLE_EDGE;
	 else edge[pos] = NOEDGE;
      }
   }

   fin_hyst_first_for = clock();
   double secs_hyst_first_for = (double)(fin_hyst_first_for - ini_hyst_first_for) / CLOCKS_PER_SEC;
   printf("Tiempo primer bloque serial: \t\t%.5g	segundos\n", secs_hyst_first_for);


   for(r=0,pos=0;r<rows;r++,pos+=cols){
      edge[pos] = NOEDGE;
      edge[pos+cols-1] = NOEDGE;
   }
   pos = (rows-1) * cols;
   for(c=0;c<cols;c++,pos++){
      edge[c] = NOEDGE;
      edge[pos] = NOEDGE;
   }

   /****************************************************************************
   * Compute the histogram of the magnitude image. Then use the histogram to
   * compute hysteresis thresholds.
   ****************************************************************************/
   for(r=0;r<32768;r++) hist[r] = 0;

   clock_t ini_hyst_fourth_for,fin_hyst_fourth_for;
   ini_hyst_fourth_for = clock();

   for(r=0,pos=0;r<rows;r++){
      for(c=0;c<cols;c++,pos++){
	 if(edge[pos] == POSSIBLE_EDGE) hist[mag[pos]]++;
      }
   }
   fin_hyst_fourth_for = clock();
   double secs_hyst_fourth_for = (double)(fin_hyst_fourth_for - ini_hyst_fourth_for) / CLOCKS_PER_SEC;
   printf("Tiempo segundo bloque serial: \t\t%.5g	segundos\n", secs_hyst_fourth_for);
   /****************************************************************************
   * Compute the number of pixels that passed the nonmaximal suppression.
   ****************************************************************************/
   for(r=1,numedges=0;r<32768;r++){
      if(hist[r] != 0) maximum_mag = r;
      numedges += hist[r];
   }

   highcount = (int)(numedges * thigh + 0.5);

   /****************************************************************************
   * Compute the high threshold value as the (100 * thigh) percentage point
   * in the magnitude of the gradient histogram of all the pixels that passes
   * non-maximal suppression. Then calculate the low threshold as a fraction
   * of the computed high threshold value. John Canny said in his paper
   * "A Computational Approach to Edge Detection" that "The ratio of the
   * high to low threshold in the implementation is in the range two or three
   * to one." That means that in terms of this implementation, we should
   * choose tlow ~= 0.5 or 0.33333.
   ****************************************************************************/
   r = 1;
   numedges = hist[1];
   while((r<(maximum_mag-1)) && (numedges < highcount)){
      r++;
      numedges += hist[r];
   }
   highthreshold = r;
   lowthreshold = (int)(highthreshold * tlow + 0.5);

   if(VERBOSE){
      printf("The input low and high fractions of %f and %f computed to\n",
	 tlow, thigh);
      printf("magnitude of the gradient threshold values of: %d %d\n",
	 lowthreshold, highthreshold);
   }

   /****************************************************************************
   * This loop looks for pixels above the highthreshold to locate edges and
   * then calls follow_edges to continue the edge.
   ****************************************************************************/
   clock_t ini_hyst_fiveth_for,fin_hyst_fiveth_for;
   ini_hyst_fiveth_for = clock();

   for(r=0,pos=0;r<rows;r++){
      for(c=0;c<cols;c++,pos++){
	 if((edge[pos] == POSSIBLE_EDGE) && (mag[pos] >= highthreshold)){
            edge[pos] = EDGE;
            follow_edges((edge+pos), (mag+pos), lowthreshold, cols);
	 }
      }
   }

   fin_hyst_fiveth_for = clock();
   double secs_hyst_fiveth_for = (double)(fin_hyst_fiveth_for - ini_hyst_fiveth_for) / CLOCKS_PER_SEC;
   printf("Tiempo tercer bloque serial: \t\t%.5g	segundos\n", secs_hyst_fiveth_for);
   /****************************************************************************
   * Set all the remaining possible edges to non-edges.
   ****************************************************************************/
   clock_t ini_hyst_sixth_for,fin_hyst_sixth_for;
   ini_hyst_sixth_for = clock();
   for(r=0,pos=0;r<rows;r++){
      for(c=0;c<cols;c++,pos++) if(edge[pos] != EDGE) edge[pos] = NOEDGE;
   }
   fin_hyst_sixth_for = clock();
   double secs_hyst_sixth_for = (double)(fin_hyst_sixth_for - ini_hyst_sixth_for) / CLOCKS_PER_SEC;
   printf("Tiempo cuarto bloque serial: \t\t%.5g	segundos\n", secs_hyst_sixth_for);

   fin_hyst = clock();
   double secs_hyst = (double)(fin_hyst - ini_hyst) / CLOCKS_PER_SEC;
   printf("Tiempo total funcion: \t\t\t%.5g	segundos\n", secs_hyst);
   printf("========================================================\n");
}

