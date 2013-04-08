#include <mpi.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

#include "../lib/mandel.h"
#include "../lib/definitions.h"


#define WORKTAG 1
#define DIETAG 2



/**
 * Structure used by the
 * workers to return the
 * color and index.
 */
typedef struct {
    int index;
    int color;
} point_color;



/* Local functions */
static void define_mpi_structs(void);
static void master(void);
static void worker(void);
static void build_image();


/* buffer */
int *buffer;

/* thread number */
int numThreads;

/* cycle values */
int cycle_x, cycle_y;

/* steps in cycle */
double delta_x, delta_y;

/* MPI Structures */
MPI_Datatype mpi_result_type, mpi_rowtype;


/**
 * @brief define_mpi_structs
 * Defines structures that are
 * used in the communication
 * between master and workers.
 */
static void
define_mpi_structs(void)
{
    /* create a type for struct point_color */
    int r_blocklengths[] = {1,1};
    MPI_Datatype r_types[] = {MPI_INT, MPI_INT};

    MPI_Aint r_offsets[2];
    r_offsets[0] = offsetof(point_color, index);
    r_offsets[1] = offsetof(point_color, color);

    MPI_Type_create_struct(2,
                           r_blocklengths,
                           r_offsets,
                           r_types,
                           &mpi_result_type);
    int result = MPI_Type_commit(&mpi_result_type);

    MPI_Datatype mpi_rowtype;
    MPI_Type_contiguous((int)WIDTH, mpi_result_type, &mpi_rowtype);
    result += MPI_Type_commit(&mpi_rowtype);

    if( result > 0 ) {
        printf("define_mpi_structs: Type result not commited\n");
        exit(1);
    }
}


/**
 * @brief main
 * Main function.
 *
 * @param argc
 * @param argv
 * @return exit_code
 */
int
main(int argc, char **argv)
{
    int myrank;

    if( argc != 2 ) {
      printf("USAGE: mandelbrot_mp2i <threads>");
      exit(1);
    }

    int numThreads = atoi(argv[1]);
    if( numThreads == 0 )
    {
      printf("Mandelbrot_mp2i: Number of threads not accepted");
      exit(1);
    }


    /* Initialize MPI */
    MPI_Init(&argc, &argv);

    define_mpi_structs();

    /* Find out my identity in the default communicator */
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (myrank == 0)
    {
        debug_info("Starting Mandelbrot with openMPI...\n");
        debug_info("Master initiating...\n");
        double start, finish;

        start = MPI_Wtime();
        master();
        finish = MPI_Wtime();
        printf("\n\n Time elapsed: %e seconds", finish-start);

    }
    else {
        worker();
    }

    /* free custom data types */
    free(buffer);
    MPI_Type_free(&mpi_rowtype);
    MPI_Type_free(&mpi_result_type);

    /* Shut down MPI */
    MPI_Finalize();

    debug_info("Finished!\n");
    return 0;
}


/**
 * @brief master
 * This function represents the master.
 * This function will distribute the
 * work throughout the workers and
 * assembly the data returned by
 * them. At the end, the result image
 * is constructed.
 */
void
aggregate_results(point_color *result)
{
    int i;
    omp_set_num_threads(numThreads);
    #pragma omp parallel shared(buffer, result) private(i)
    {
    #pragma omp for schedule(static)
        for(i = 0; i < (int)WIDTH; i++)
        {

            point_color result_pair = result[i];
            printf("%d %d %d ", i, result_pair.index, result_pair.color);
            buffer[(int)result_pair.index] = result_pair.color;
        }
    } // end pragma
}


static void
master(void)
{
    int ntasks, rank;
    point_color result[(int)WIDTH];
    MPI_Status status;


    /* Find out how many processes there are in the default communicator */
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    // initiate variables
    delta_x = (X_MAX - X_MIN) / WIDTH;
    delta_y = (Y_MAX - Y_MIN ) / HEIGHT;

    cycle_x = 0;
    cycle_y = 0;
    int buffer_size = (int)WIDTH * HEIGHT;
    buffer = (int*)malloc(buffer_size*sizeof(int));

    debug_info("Distributing work... \n");
    int stripe = 0;

    /* Seed the slaves; send one unit of work to each slave. */
    for (; stripe < ntasks; stripe++)
    {
        // should not happen
        if( stripe >= ntasks ) { break; }

        /* Send it to each rank */
        MPI_Send(&stripe,             /* message buffer */
                 1,                 /* one data item */
                 MPI_INT,    /* data item is an integer */
                 stripe,              /* destination process rank */
                 WORKTAG,           /* user chosen message tag */
                 MPI_COMM_WORLD);   /* default communicator */
    }


    debug_info("Refeeding (%d)...\n",stripe);
    /* Loop over until there is no more work to be done */
    for(; stripe < HEIGHT; stripe++)
    {

        /* Receive results from a slave */
        MPI_Recv(&result,           /* message buffer */
                 (int)WIDTH,                 /* WIDTH data item */
                 mpi_result_type,   /* of type */
                 MPI_ANY_SOURCE,    /* receive from any sender */
                 MPI_ANY_TAG,       /* any type of message */
                 MPI_COMM_WORLD,    /* default communicator */
                 &status);          /* info about the received message */

        debug_info("Result received\n");
        aggregate_results(result);

        /* Send the slave a new work unit */
        MPI_Send(&stripe,             /* message buffer */
                 1,                 /* one data item */
                 MPI_INT,           /* data item is an integer */
                 status.MPI_SOURCE, /* to who we just received from */
                 WORKTAG,           /* user chosen message tag */
                 MPI_COMM_WORLD);   /* default communicator */

    }

    debug_info("Waiting results... \n");
    /* Receive all the results from the workers. */
    for (rank = 1; rank < ntasks; ++rank)
    {
        MPI_Recv(&result,
                 WIDTH,
                 mpi_result_type,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);

        aggregate_results(result);
    }

    debug_info("Laying off all the workers...\n");
    /* Tell all the workers to exit by sending an DIETAG. */
//    for (rank = 1; rank < ntasks; ++rank)
//    {
//        MPI_Send(0, 0, mpi_result_type, rank, DIETAG, MPI_COMM_WORLD);
//    }


    debug_info("Building mandelbrot image file...\n");
    build_image();
    debug_info("Image built!\n");
}


/**
 * @brief worker
 * This functions represents the worker.
 * It will receive point through MPI struct
 * pointdata and calculate its color.
 * After the calculation, the color is
 * sent through MPI struct result.
 * This cycle will continue until it
 * receives the DIETAG from the master.
 */
static void
worker(void)
{
    int work_y;
    point_color result[(int)WIDTH];
    MPI_Status status;
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


    debug_info( "Worker %i: initiating...\n", myrank );
    while(1)
    {
        /* Receive a message from the master */
        MPI_Recv(&work_y, 1, MPI_INT, 0, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);

        /* Check the tag of the received message. */
        if ( work_y == -1 || status.MPI_TAG == DIETAG ) {
            debug_info("Worker %i: dismissed.", myrank);
            return;
        }

        /* Do the work */
        int x_index;
        double y_value = Y_MIN + delta_y * work_y;


        omp_set_num_threads(numThreads);
#pragma omp parallel shared(result,delta_y, delta_x, work_y) private(x_index)
{
#pragma omp for schedule(dynamic)
        for(x_index = 0; x_index < (int)WIDTH ; x_index++)
        {
            double x_value = X_MIN + delta_x * x_index;
            result[x_index].index = work_y*WIDTH + x_index;
            result[x_index].color = compute_point(x_value, y_value);
            debug_info("%f %d %d > ", x_index, result[x_index].index, result[x_index].color);
        }
} // end pragma

        /* Send the result back */
        MPI_Send(&result,
                 (int)WIDTH,
                 mpi_result_type,
                 0,
                 0,
                 MPI_COMM_WORLD);
    }
}


/**
 * @brief process_results
 * Outputs the Mandelbrot image to a file
 * This function requires the function
 * declared in the mandel file, which has
 * the actual implementation.
 */
static void
build_image()
{
    output_pgm("mandel_mp2i",buffer, WIDTH, HEIGHT, 255);
}
