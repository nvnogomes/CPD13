/*
 * taylor.c
 *
 * This program calculates the value of e*pi by first calculating e
 * and pi by their taylor expansions and then multiplying them
 * together.
 */

#include <stdio.h>
#include <time.h>

#include "walltime.h"

#define num_steps 20000000

double clockZero = 0.0;

int main(int argc, char *argv[])
{
  double start, stop; /* times of beginning and end of procedure */
  double e, pi, factorial, product;
  int i;

  /* start the timer */
  start = walltime(&clockZero);

  /* First we calculate e from its taylor expansion */
  printf("e started\n");
  e = 1;
  factorial = 1; /* rather than recalculating the factorial from
		    scratch each iteration we keep it in this varialbe
		    and multiply it by i each iteration. */
  for (i = 1; i<num_steps*40; i++) {
    factorial *= i;
    e += 1.0/factorial;
  }
  printf("e done\n");
  stop = walltime(&start);
  printf("\nwall time for e: %f s \n", stop);

  /* Then we calculate pi from its taylor expansion */
  start = walltime(&clockZero);
  printf("pi started\n");

  pi = 0;
  for (i = 0; i < num_steps*10; i++) {
    /* we want 1/1 - 1/3 + 1/5 - 1/7 etc.
       therefore we count by fours (0, 4, 8, 12...) and take
         1/(0+1) =  1/1
       - 1/(0+3) = -1/3
         1/(4+1) =  1/5
       - 1/(4+3) = -1/7 and so on */
    pi += 1.0/(i*4.0 + 1.0);
    pi -= 1.0/(i*4.0 + 3.0);
  }
  pi = pi * 4.0;
  printf("pi done\n");
  stop = walltime(&start);
  printf("\nwall time for pi: %f s \n", stop);



  printf("e = %f, pi = %f\n", e, pi);

  product = e * pi;
  printf("Reached result %f \n", product);

  return 0;
}
