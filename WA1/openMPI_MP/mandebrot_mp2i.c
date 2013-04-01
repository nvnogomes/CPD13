#include <mpi.h>
#include <stddef.h>

#include "mandel.c"

#define WORKTAG 1
#define DIETAG 2

typedef struct {
    double x;
    double y;
    double x_value;
    double y_value;
} point;

typedef struct {
    int index;
    int color;
} point_color;




/* Local functions */
static void master(void);
static void slave(void);
static point get_next_work_item(void);
static void process_results(void);
static point_color do_work(point);




int *buffer;
// cycle values
double cycle_x, cycle_y;

// steps in cycle
double delta_x, delta_y;


int width, height;
int xmax, xmin, ymax, ymin;


void
define_mpi_structs()
{
    /* create a type for entry struct */
    int struct_size = 4;
    int pblocklengths[] = {6,6,6,6};
    MPI_Datatype ptypes[] = {MPI_DOUBLE, MPI_DOUBLE,MPI_DOUBLE, MPI_DOUBLE};
    MPI_Datatype mpi_point_type;
    MPI_Aint offsets[struct_size];

    offsets[0] = offsetof(point, x);
    offsets[1] = offsetof(point, y);
    offsets[2] = offsetof(point, x_value);
    offsets[3] = offsetof(point, y_value);

    MPI_Type_create_struct(struct_size, pblocklengths, offsets, ptypes, &mpi_point_type);
    MPI_Type_commit(&mpi_point_type);


    /* create a type for struct car */
    struct_size = 2;
    int rblocklengths[] = {1,1};
    MPI_Datatype rtypes[] = {MPI_INT, MPI_INT};
    MPI_Datatype mpi_result_type;
    MPI_Aint roffsets[struct_size];

    offsets[0] = offsetof(point_color, index);
    offsets[1] = offsetof(point_color, color);

    MPI_Type_create_struct(struct_size, rblocklengths, roffsets, rtypes, &mpi_result_type);
    MPI_Type_commit(&mpi_result_type);
}



int
main(int argc, char **argv)
{
    int myrank;

    if( argc != 5 )
    {
        printf("USAGE: mandelbrot_mpi Xmin Ymin Xmax Ymax");
        return 1;
    }

    // initiate variables
    xmin = atoi( argv[1] );
    ymin = atoi( argv[2] );
    xmax = atoi( argv[3] );
    ymax = atoi( argv[4] );
    width = xmax - xmin;
    height = ymax - ymin;
    cycle_x = 0;
    cycle_y = 0;
    buffer = (int*)malloc(width * height);

    /* Initialize MPI */
    MPI_Init(&argc, &argv);


    /* Find out my identity in the default communicator */
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (myrank == 0) {
        master();
    } else {
        slave();
    }


    /* Shut down MPI */
    MPI_Finalize();
    return 0;
}


static void
master(void)
{
    int ntasks, rank;
    point work;
    point_color result;
    MPI_Status status;


    define_mpi_structs();

    /* Find out how many processes there are in the default
     communicator */
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);


    delta_x = (xmax - xmin)/width;
    delta_y = (ymax - ymin)/height;

    /* Seed the slaves; send one unit of work to each slave. */
    for (rank = 1; rank < ntasks; ++rank)
    {

        /* Find the next item of work to do */
        work = get_next_work_item();

        /* Send it to each rank */
        MPI_Send(&work,             /* message buffer */
                 1,                 /* one data item */
                 mpi_point_type,    /* data item is an integer */
                 rank,              /* destination process rank */
                 WORKTAG,           /* user chosen message tag */
                 MPI_COMM_WORLD);   /* default communicator */
    }

    /* Loop over getting new work requests until there is no more work
     to be done */
    work = get_next_work_item();
    while (work != NULL)
    {

        /* Receive results from a slave */
        MPI_Recv(&result,           /* message buffer */
                 1,                 /* one data item */
                 mpi_result_type,   /* of type double real */
                 MPI_ANY_SOURCE,    /* receive from any sender */
                 MPI_ANY_TAG,       /* any type of message */
                 MPI_COMM_WORLD,    /* default communicator */
                 &status);          /* info about the received message */

        /* Send the slave a new work unit */
        MPI_Send(&work,             /* message buffer */
                 1,                 /* one data item */
                 mpi_point_type,           /* data item is an integer */
                 status.MPI_SOURCE, /* to who we just received from */
                 WORKTAG,           /* user chosen message tag */
                 MPI_COMM_WORLD);   /* default communicator */

        /* Get the next unit of work to be done */
        work = get_next_work_item();
    }

    /* Receive all the results from the slaves. */
    for (rank = 1; rank < ntasks; ++rank)
    {
        MPI_Recv(&result,
                 1,
                 mpi_result_type,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);
    }

    /* Tell all the slaves to exit by sending an DIETAG. */
    for (rank = 1; rank < ntasks; ++rank)
    {
        MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
    }

    process_results();
}


static void
slave(void)
{
    point work;
    point_color result;
    MPI_Status status;

    while (1)
    {
        /* Receive a message from the master */
        MPI_Recv(&work, 1, mpi_point_type, 0, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);

        /* Check the tag of the received message. */
        if (status.MPI_TAG == DIETAG)
        {
            return;
        }

        /* Do the work */
        result = do_work(work);

        /* Send the result back */
        MPI_Send(&result, 1, mpi_result_type, 0, 0, MPI_COMM_WORLD);
    }
}


static point
get_next_work_item(void)
{

    cycle_x++;
    if( cycle_x >= width )
    {
        cycle_x = 0;
        cycle_y++;
        if( cycle_y++ >= height )
        {
            return NULL;
        }
    }

    double y_value = ymin + delta_y * cycle_y;
    double x_value = xmin + delta_x * cycle_x;

    point work_unit;
    work_unit.x = cycle_x;
    work_unit.y = cycle_y;
    work_unit.x_value = x_value;
    work_unit.y_value = y_value;

    return work_unit;
}


static void
process_results(void)
{
    char filename[] = "figure";
    output_pgm( filename,buffer, width, height, 255);
}


static point_color
do_work(point p)
{
    point_color pc;
    pc.index = p.y*width + p.x;
    pc.color = compute_point(p.x_value, p.y_value);

    return pc;
}

