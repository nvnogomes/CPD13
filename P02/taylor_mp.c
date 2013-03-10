/*
 * taylor.c
 *
 * This program calculates the value of e*pi by first calculating e
 * and pi by their taylor expansions and then multiplying them
 * together.
 */

#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "walltime.h"
#define num_steps 20000000

double clockZero = 0.0;
double start;

double calc_e( int num_iter){

  double e, factorial, stop;
  int i;

  printf("e started\n");
  e = 1.0;
  factorial = 1.0; /* rather than recalculating the factorial from
			scratch each iteration we keep it in this varialbe
			and multiply it by i each iteration. */
  for (i = 1; i<num_iter; i++) {
    factorial *= i;
    e += 1.0/factorial;
  }
  stop = walltime(&start);
  printf("e done in %f s\n", stop);
  return e;
}


double calc_pi( int num_iter){
       /* calculate pi expansion */

  double pi4, stop;
  int i;
  printf("pi started\n");

  pi4 = 0.0;
  for (i = 0; i < num_iter; i++) {
	/* we want 1/1 - 1/3 + 1/5 - 1/7 etc.
	   therefore we count by fours (0, 4, 8, 12...) and take
	   1/(0+1) =  1/1
	     - 1/(0+3) = -1/3
	   1/(4+1) =  1/5
	     - 1/(4+3) = -1/7 and so on */
    pi4 += 1.0/(i*4.0 + 1.0);
    pi4 -= 1.0/(i*4.0 + 3.0);
  }
  stop = walltime(&start);
  printf("pi done in %f s\n", stop);
  return 4.0*pi4;
}



int main(int argc, char *argv[])
{
 
  double product, e, pi;
 

  omp_set_num_threads(2);
  /* start the timer */
  start = walltime(&clockZero);

  /* Now there is no first and second, we calculate e and pi */
#pragma omp parallel sections default(none) shared(product,e,pi)
{
#pragma omp section
  e = calc_e(40*num_steps);
#pragma omp section 
  pi = calc_pi(10*num_steps);
  } /* omp sections */
  
  /* at this point the threads should rejoin */


  printf("e = %f, pi = %f\n", e, pi);
  product = e * pi;
  printf("Reached result %f\n", product);

  return 0;
}
