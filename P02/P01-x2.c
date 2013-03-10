#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>


#define SIZE 50*1024*1024
int *array;
int length, count;


void count3s(int nthreads){
  int i;

#pragma omp parallel for num_threads(nthreads) reduction(+:count)
  for(i=0; i < SIZE; i++){
      if( array[i] == 3 ) {
          count++;
        }
    }
}

void fillArray() {
  array = (int *)malloc(SIZE*sizeof(int));
  length = SIZE;
  // srand(getpid());
  srand(0);
  int i;
  for(i=0; i < length; i++){
      array[i] = rand() % 4;
    }
}

int main( int argc, char *argv[]){

  if( argc != 2 ) {
      printf("USAGE: p02-x4 <num_threads>");
      exit;
    }


  count = 0;
  struct timeval s,t;

  int n = atoi(argv[1]);
  printf("Using %d threads\n", n );

  fillArray();

  gettimeofday(&s, NULL);
  count3s(n);
  gettimeofday(&t, NULL);

  printf("Count of 3s = %d\n", count);
  printf("Elapsed time (us) = %ld\n",
         (t.tv_sec - s.tv_sec)*1000000 + (t.tv_usec - s.tv_usec));
  return 0;
}


