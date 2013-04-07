#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

#include "../lib/mandel.h"
#include "../lib/definitions.h"
#include "../lib/walltime.h"


int
calculate_image(int *buf)
{
    double delta_x = (X_MAX - X_MIN)/WIDTH;
    double delta_y = (Y_MAX - Y_MIN)/HEIGHT;
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

            while ((zr*zr + zi*zi < 4) && (iterations < 255))
            {
                double nr, ni;

                /* Z <-- Z^2 + C */
                nr = zr*zr - zi*zi + cr;
                ni = 2*zr*zi + ci;

                zi = ni;
                zr = nr;

                iterations ++;
            }

            buf[index] = iterations;
        }
    }

    return 0;
}

const char calculate_image_cl[] = "                                 \
    __kernel calculate_image_cl                                     \
        (   __global    const   char*    in                         \
        ,   __global            char*    out                        \
        )                                                           \
    {                                                               \
        float delta_x = (1 - -1)/2048;                              \
        float delta_y = (1 - -1)/2048;                              \
        int x,y;                                                    \
                                                                    \
        y = get_global_id(0);                                       \
        x = get_global_id(1);                                       \
                                                                    \
            float cr = -1 + delta_y * y;                            \
            float ci = -1 + delta_x * x;                            \
            int index = y*2048 + x;                                 \
                                                                    \
            int iterations = 0;                                     \
            float zi = 0;                                           \
            float zr = 0;                                           \
                                                                    \
            while ((zr*zr + zi*zi < 4) && (iterations < 255))       \
            {                                                       \
                float nr, ni;                                       \
                                                                    \
                nr = zr*zr - zi*zi + cr;                            \
                ni = 2*zr*zi + ci;                                  \
                                                                    \
                zi = ni;                                            \
                zr = nr;                                            \
                                                                    \
                iterations ++;                                      \
            }                                                       \
            buf[index] = iterations;                                \
    }                                                               \
";


int
main(int argc)
{
    if( argc != 1 ) {
        printf("USAGE: mandelbrot_cl");
        exit(1);
    }
    debug_info("Starting Mandelbrot openCL...\n");

    cl_int error;
    cl_platform_id platform;
    cl_device_id device;
    cl_uint platforms, devices;

    int buffer_size = WIDTH*HEIGHT;
    int *buf_in = malloc(buffer_size*sizeof(int));
    int *buf_out = malloc(buffer_size*sizeof(int));

    // Fetch the Platform and Device IDs; we only want one.
    error=clGetPlatformIDs(1, &platform, &platforms);
    error=clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, &devices);
    cl_context_properties properties[]={
        CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
        0};
    // Note that nVidia's OpenCL requires the platform property
    cl_context context=clCreateContext(properties, 1, &device, NULL, NULL, &error);
    cl_command_queue cq = clCreateCommandQueue(context, device, 0, &error);


    size_t srcsize, worksize=buffer_size;
    const char *src=calculate_image_cl;
    srcsize=strlen(calculate_image_cl);

    const char *srcptr[]={src};
    // Submit the source code of the calculate_image kernel to OpenCL
    cl_program prog=clCreateProgramWithSource(context,
        1, srcptr, &srcsize, &error);
    // and compile it (after this we could extract the compiled version)
    error=clBuildProgram(prog, 0, NULL, "", NULL, NULL);

    // Allocate memory for the kernel to work with
    cl_mem mem1, mem2;
    mem1=clCreateBuffer(context, CL_MEM_READ_ONLY, worksize, NULL, &error);
    mem2=clCreateBuffer(context, CL_MEM_WRITE_ONLY, worksize, NULL, &error);

    // get a handle and map parameters for the kernel
    cl_kernel k_calc_image=clCreateKernel(prog, "calc_i", &error);
    clSetKernelArg(k_calc_image, 0, sizeof(mem1), &mem1);
    clSetKernelArg(k_calc_image, 1, sizeof(mem2), &mem2);


    /* for timing */
    double startTime, elapsedTime;
    double clockZero = 0.0;





    // Send input data to OpenCL (async, don't alter the buffer!)
    error=clEnqueueWriteBuffer(cq, mem1, CL_FALSE, 0, worksize, buf_in, 0, NULL, NULL);

    debug_info("Computing image...\n");
    startTime = walltime( &clockZero );
    // Perform the operation
    error=clEnqueueNDRangeKernel(cq, k_calc_image, 1, NULL, &worksize, &worksize, 0, NULL, NULL);
    elapsedTime = walltime( &startTime );

    // Read the result back into buf2
    error=clEnqueueReadBuffer(cq, mem2, CL_FALSE, 0, worksize, buf_out, 0, NULL, NULL);
    // Await completion of all the above
    error=clFinish(cq);


    debug_info("WallTime: ");
    printf("%.2fs\n", elapsedTime);

    debug_info("Building image...\n");
    output_pgm("mandel_cl",buf_out,WIDTH,HEIGHT,255);

    debug_info("Finished!\n");
    return 0;
}
