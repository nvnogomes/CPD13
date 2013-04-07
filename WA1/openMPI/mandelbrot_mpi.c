#include <mpi.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "../lib/mandel.h"
#include "../lib/definitions.h"


#define WORKTAG 1
#define DIETAG 2

/**
 * Structure used to give
 * the necessary data
 * to the workers.
 */
typedef struct {
    int x;
    int y;
    double x_value;
    double y_value;
    int end;
} point_data;

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
static point_data get_next_work_item(void);
static void build_image();
static point_color do_work(point_data);

/* buffer */
int *buffer;


/* cycle values */
int cycle_x, cycle_y;

/* steps in cycle */
double delta_x, delta_y;

/* MPI Structures */
MPI_Datatype mpi_result_type, mpi_pointdata_type;


/**
 * @brief define_mpi_structs
 * Defines structures that are
 * used in the communication
 * between master and workers.
 */
static void
define_mpi_structs(void)
{
    /* create a type for entry struct */
    int pd_blocklengths[] = {1,1,6,6,1};
    MPI_Datatype pd_types[] = {MPI_INT, MPI_INT,MPI_DOUBLE, MPI_DOUBLE,
                               MPI_INT};

    MPI_Aint pd_offsets[5];
    pd_offsets[0] = offsetof(point_data, x);
    pd_offsets[1] = offsetof(point_data, y);
    pd_offsets[2] = offsetof(point_data, x_value);
    pd_offsets[3] = offsetof(point_data, y_value);
    pd_offsets[4] = offsetof(point_data, end);

    MPI_Type_create_struct(5,
                           pd_blocklengths,
                           pd_offsets,
                           pd_types,
                           &mpi_pointdata_type);
    int result = MPI_Type_commit(&mpi_pointdata_type);
    if( result > 0 ) {
        printf("define_mpi_structs: type pointdata not commited\n");
        exit(1);
    }


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
    result = MPI_Type_commit(&mpi_result_type);
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

    if( argc != 1 )
    {
        printf("USAGE: mandelbrot_mpi\n");
        return 1;
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
        debug_info("Walltime (s): ");
        printf("%e\n", finish-start);

    }
    else {
        worker();
        debug_info("Worker %i: dismissed.", myrank);
    }

    /* free custom data types */
    free(buffer);
    MPI_Type_free(&mpi_pointdata_type);
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
static void
master(void)
{
    int ntasks, rank;
    point_color result;
    point_data work;
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
    /* Seed the slaves; send one unit of work to each slave. */
    for (rank = 1; rank < ntasks; ++rank)
    {

        /* Find the next item of work to do */
        work = get_next_work_item();

        //debug_info("NW (%f,%f) %i\n", work.x_value, work.y_value, work.end);
        if( work.end == 1 ){
            break;
        }

        /* Send it to each rank */
        MPI_Send(&work,             /* message buffer */
                 1,                 /* one data item */
                 mpi_pointdata_type,    /* data item is an integer */
                 rank,              /* destination process rank */
                 WORKTAG,           /* user chosen message tag */
                 MPI_COMM_WORLD);   /* default communicator */
    }
	debug_info("Refeeding...\n");

    /* Loop over until there is no more work to be done */
    for(;;)
    {
        /* Get the next unit of work to be done */
        work = get_next_work_item();
        if( work.end == 1 ){
            break;
        }

        //debug_info("NW (%f,%f) %i\n", work.x_value, work.y_value, work.end);

        /* Receive results from a slave */
        MPI_Recv(&result,           /* message buffer */
                 1,                 /* one data item */
                 mpi_result_type,   /* of type double real */
                 MPI_ANY_SOURCE,    /* receive from any sender */
                 MPI_ANY_TAG,       /* any type of message */
                 MPI_COMM_WORLD,    /* default communicator */
                 &status);          /* info about the received message */

        //debug_info("Result i%i c%i\n", result.index, result.color);
        buffer[(int)result.index] = result.color;

        /* Send the slave a new work unit */
        MPI_Send(&work,             /* message buffer */
                 1,                 /* one data item */
                 mpi_pointdata_type,           /* data item is an integer */
                 status.MPI_SOURCE, /* to who we just received from */
                 WORKTAG,           /* user chosen message tag */
                 MPI_COMM_WORLD);   /* default communicator */

    }

    debug_info("Waiting results... \n");
    /* Receive all the results from the workers. */
    for (rank = 1; rank < ntasks; ++rank)
    {
        MPI_Recv(&result,
                 1,
                 mpi_result_type,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);

        buffer[result.index] = result.color;
    }

    debug_info("Laying off all the workers...\n");
    /* Tell all the workers to exit by sending an DIETAG. */
    for (rank = 1; rank < ntasks; ++rank)
    {
        MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
    }


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
    point_data work;
    point_color result;
    MPI_Status status;
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    debug_info( "Worker %i: initiating...\n", myrank );
    while(1)
    {
        /* Receive a message from the master */
        MPI_Recv(&work, 1, mpi_pointdata_type, 0, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);

        /* Check the tag of the received message. */
        if ( work.end == 1 || status.MPI_TAG == DIETAG ) {
            debug_info("Worker %i: dismissed.", myrank);
            return;
        }

        /* Do the work */
        result = do_work(work);

        /* Send the result back */
        MPI_Send(&result, 1, mpi_result_type, 0, 0, MPI_COMM_WORLD);
    }
}


/**
 * @brief get_next_work_item
 * This function calculates the next point
 * for which will be calculated its color
 *
 * @return point_data
 */
static point_data
get_next_work_item(void)
{
    point_data next_work_unit;


    if( cycle_x >= WIDTH )
    {
        cycle_x = 0;
        cycle_y++;

        // end cycle condition
        if( cycle_y >= HEIGHT )
        {
            next_work_unit.end = 1;
            return next_work_unit;
        }
    }

    double y_value = Y_MIN + delta_y * cycle_y;
    double x_value = X_MIN + delta_x * cycle_x;

    next_work_unit.end = 0;
    next_work_unit.x = cycle_x;
    next_work_unit.y = cycle_y;
    next_work_unit.x_value = x_value;
    next_work_unit.y_value = y_value;
    cycle_x++;

    return next_work_unit;
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
    output_pgm("mandel_mpi",buffer, WIDTH, HEIGHT, 255);
}


/**
 * @brief do_work
 * Calculates the color of the given point
 * This function requires the function
 * declared in the mandel file, which has
 * the actual calculation code.
 *
 * @param p : point to caculate color
 * @return point_color : point index and color
 */
static point_color
do_work(point_data p)
{
    point_color pc;
    pc.index = p.y*WIDTH + p.x;
    pc.color = compute_point(p.x_value, p.y_value);

    return pc;
}

