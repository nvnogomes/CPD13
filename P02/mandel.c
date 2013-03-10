#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int max_iterations = 255;


/* This function (compute_point) is the algorithmic core of this program,
   computing what could be called the Mandelbrot function of the complex
   number (cr, ci).
*/

int compute_point(double ci, double cr) {
  int iterations = 0;
  double zi = 0;
  double zr = 0;
  
  while ((zr*zr + zi*zi < 4) && (iterations < max_iterations)) {
    double nr, ni; 

    /* Z <-- Z^2 + C */
   
    nr = zr*zr - zi*zi + cr;
    ni = 2*zr*zi + ci;
    
    zi = ni;
    zr = nr;

    iterations ++;
  } 
  return iterations;  
}

/* The "compute" function computes the Mandelbrot function over every
   point on a grid that is "nx" points wide by "ny" points tall, where
   (xmin,ymin) and (xmax,ymax) give two corners of the region the 
   complex plane.
*/

void compute(int *buffer, int nx, int ny, double xmin, double xmax, 
	     double ymin, double ymax) {
  double delta_x, delta_y;
  int x, y;

  delta_x = (xmax - xmin)/nx;
  delta_y = (ymax - ymin)/ny;

  for (y=0;  y<ny; y++) {
    double y_value = ymin + delta_y * y;
    for (x=0; x<nx; x++) {
      double x_value = xmin + delta_x * x;
      buffer[y*nx + x] = compute_point(x_value, y_value);
    }
  }
}

/* Output the data contained in the buffer to a Portable Greymap format
   image file.  The parameter "max" should be an upper bound for the
   data values in the buffer. 
*/

void output_pgm(char *filename,int *buffer, int nx, int ny, int max) {
  int i;
  FILE *file;
  file = fopen(filename,"w");
  fprintf(file,"P2\n");
  fprintf(file,"%d %d\n",nx,ny);
  fprintf(file,"%d\n",max);
  for (i=0; i<nx*ny; i++) {
    if (!(i%nx)) fprintf(file,"\n");
    fprintf(file,"%d ",buffer[i]);
  }
  fclose(file);
}
