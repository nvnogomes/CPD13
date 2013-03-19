#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

#define TAG_REQUEST 1
#define TAG_REPLY 2

int MonteCarlo(int count){
	int i, total;
	float x, y;
	total = 0;
	srand(getpid());

	for(i=0; i < count; i++){
		x= (float)rand()/(float)RAND_MAX;
		y= (float)rand()/(float)RAND_MAX;
//		printf("x = %f, y = %f\n", x, y);
		if((x*x+y*y) <= 1.0) total++;
	}
	return total;
}


int master(int nslaves, int iterations){
	int i, res, tot;
	MPI_Status s;

	for(i=1; i <= nslaves; i++)
        	MPI_Send( &iterations, 1, MPI_INT, i, TAG_REQUEST, MPI_COMM_WORLD);
	tot = 0;
	for(i=1; i <= nslaves; i++){
        	MPI_Recv( &res, 1, MPI_INT, MPI_ANY_SOURCE, TAG_REPLY, 
			MPI_COMM_WORLD, &s );
		printf("Received reply %d from slave %d\n", res, (int)s.MPI_SOURCE);
   		tot += res;
	}
	return tot;
}

void slave(void){
	int res, iter;
	MPI_Recv( &iter, 1, MPI_INT, 0, TAG_REQUEST, MPI_COMM_WORLD, NULL);
//	printf("iter = %d\n", iter);
	res = MonteCarlo( iter );
//	printf("res = %d\n", res);
        MPI_Send( &res, 1, MPI_INT, 0, TAG_REPLY, MPI_COMM_WORLD);
}

int main( int argc, char *argv[]){

  int rank, size, len, total, iterations;
  char name[MPI_MAX_PROCESSOR_NAME];
  
  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */
  MPI_Get_processor_name( name, &len);
  

  if(argc!= 2){
	printf("Usage: %s <iterations per slave>\n", argv[0]);
	MPI_Finalize();
	return 1;
  }

  if( rank == 0){
	iterations = atoi(argv[1]);
#ifdef DEBUG
	printf( "Hello from master %d of %d, running in %s\n", 
		rank, size, name );
	MPI_Barrier(MPI_COMM_WORLD);
#endif
	total = master(size -1, iterations);
	printf("pi = %f\n", 4.0*(float)total/(float)((size-1) *iterations));
  } else{
#ifdef DEBUG
	printf( "Hello, from slave %d  (of %d), running in %s\n", 
		rank, size, name );
	MPI_Barrier(MPI_COMM_WORLD);
#endif
	slave();	
  }
  MPI_Finalize();
  return 0;
}

