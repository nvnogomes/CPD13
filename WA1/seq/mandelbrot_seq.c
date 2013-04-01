
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lib/mandel.h"
#include "../lib/definitions.h"



int
main(int argc, char **argv)
{
    int *buffer = malloc(WIDTH*HEIGHT*sizeof(int));

    debug_info("Starting Mandelbrot sequential...\n");
    compute(buffer,WIDTH,HEIGHT,X_MIN,X_MIN,Y_MIN,Y_MAX);

    debug_info("Mandelbrot Ended\n");

    output_pgm("mandel",buffer,WIDTH,HEIGHT,255);
    return 0;
}
