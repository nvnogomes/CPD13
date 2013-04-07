/**
 * Parallel implementation of Mandelbrot using OpenMP
 */

#include <omp.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include "../lib/mandel.h"
#include "../lib/definitions.h"
#include "../lib/walltime.h"



int main(int argc, char *argv[])
{
	if( argc != 2 ) {
	  printf("USAGE: mandelbrot_dmp <threads>");
	  exit(1);
	}
	int numThreads = atoi(argv[1]);
	if( numThreads == 0 )
	{
	  printf("Mandelbrot_smp: Number of threads not accepted");
	  exit(1);
	}



  /* for timing */
  double startTime, elapsedTime;
  double clockZero = 0.0;

  /* buffer size and chunk calculation */
  int buffer_size = WIDTH*HEIGHT;
  int *buffer = malloc(buffer_size*sizeof(int));

  debug_info("Computing image...\n");
  startTime = walltime( &clockZero );

  double delta_x, delta_y;
  int x, y;

  delta_x = (X_MAX - X_MIN)/WIDTH;
  delta_y = (Y_MAX - Y_MIN)/HEIGHT;

  debug_info("Starting Mandelbrot dynamic openMP (Threads: %i; ChunkSize: %i)...\n", numThreads, 1);

  omp_set_num_threads(numThreads);
#pragma omp parallel shared(buffer) private(x,y)
  {
#pragma omp for schedule(dynamic)
  for (y=0;  y < (int)HEIGHT; y++)
  {
      double y_value = Y_MIN + delta_y * y;
      for (x=0; x < (int)WIDTH; x++)
      {
          double x_value = X_MIN + delta_x * x;
          int index = y*WIDTH + x;
          int color = compute_point(x_value, y_value);
#pragma omp critical
          buffer[index] = color;
      }
  }
  } // end pragma parallel

  elapsedTime = walltime( &startTime );

  debug_info("Walltime (s): ");
  printf("%.2f\n", elapsedTime);
  debug_info("Building image...\n");
  output_pgm("mandel_dmp",buffer,WIDTH,HEIGHT,255);

  debug_info("Finished!\n");
  return 0;


}
