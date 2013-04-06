#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../lib/mandel.h"
#include "../lib/definitions.h"
#include "../lib/walltime.h"

const double max_iterations = 255;

double delta_x;
double delta_y;
int *buffer;


int
calculate_image()
{
    delta_x = (X_MAX - X_MIN)/WIDTH;
    delta_y = (Y_MAX - Y_MIN)/HEIGHT;
    int x,y;

    for (y=0;  y < (int)HEIGHT; y++)
    {
        double cr = Y_MIN + delta_y * y;
        for (x=0; x< (int)WIDTH; x++)
        {
            double ci = X_MIN + delta_x * x;
            int index = y*WIDTH + x;

            int iterations = 0;
            double zi = 0;
            double zr = 0;

            while ((zr*zr + zi*zi < 4) && (iterations < max_iterations))
            {
                double nr, ni;

                /* Z <-- Z^2 + C */
                nr = zr*zr - zi*zi + cr;
                ni = 2*zr*zi + ci;

                zi = ni;
                zr = nr;

                iterations ++;
            }

            buffer[index] = iterations;
        }
    }

    return 0;
}


int
main(int argc, char *argv[])
{
    if( argc != 1 ) {
        printf("USAGE: mandelbrot_cl");
        exit(1);
    }

    debug_info("Starting Mandelbrot openCL...\n");

    /* for timing */
    double startTime, elapsedTime;
    double clockZero = 0.0;

    int buffer_size = WIDTH*HEIGHT;
    buffer = malloc(buffer_size*sizeof(int));

    calculate_image();

    debug_info("Computing image...\n");
    startTime = walltime( &clockZero );


    elapsedTime = walltime( &startTime );
    debug_info("WallTime: %.2fs\n", elapsedTime);
    debug_info("Building image...\n");
    output_pgm("mandel_cl",buffer,WIDTH,HEIGHT,255);

    debug_info("Finished!\n");
    return 0;
}
