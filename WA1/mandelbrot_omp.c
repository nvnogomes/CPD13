/**
 * Parallel implementation of Mandelbrot using OpenMP
 *
 *
 */

#include <omp.h>
#include <stdio.h>
#include <time.h>

#define L 1280
#define H 800



int main(int argc, char *argv[])
{
  if( argc != 2 ) {
      printf("USAGE: mandelbrot <threads>");
  }

}
