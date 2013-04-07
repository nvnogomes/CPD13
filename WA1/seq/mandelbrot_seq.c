
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lib/mandel.h"
#include "../lib/definitions.h"
#include "../lib/walltime.h"

int
main(int argc, char **argv)
{
    debug_info("Starting Mandelbrot sequential...\n");

    /* for timing */
    double startTime, elapsedTime;
    double clockZero = 0.0;

    int *buffer = malloc(WIDTH*HEIGHT*sizeof(int));


    debug_info("Computing image...\n");
    startTime = walltime( &clockZero );
    compute(buffer,WIDTH,HEIGHT,X_MIN,X_MAX,Y_MIN,Y_MAX);
    elapsedTime = walltime( &startTime );

    debug_info("Walltime (s): ");
    printf("%.2f\n", elapsedTime);
    debug_info("Building image...\n");
    output_pgm("mandel_seq",buffer,WIDTH,HEIGHT,255);

    debug_info("Finished!\n");
    return 0;
}
