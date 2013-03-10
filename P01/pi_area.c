
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>


/*

*/
double ni = 0;
int numOfThreads;
int numOfReps;
double threadReps;
pthread_mutex_t mutex;




/**
 * @brief best
 * @param N
 * @return
 */
void *best(void *i) {

    double id = atof(i);
    double region = 1.0/ numOfThreads;
    double start = id * region;
    double stop = start + region;
    double step = 1.0 / threadReps;

    double subArea = 0;
    double x;
    for (x = start; x < stop; ++x) {
        subArea += sqrt(1-x*x);
    }
    subArea *= step;

    pthread_mutex_lock(&mutex);
    ni += subArea;
    pthread_mutex_unlock(&mutex);
}


int main (int argc, char *argv[]) {

    if(argc!=3){
        printf("Usage:%s #threads #tests #type\n", argv[0]);
        exit(1);
    }

    clock_t begin, end;
    pthread_mutex_init(&mutex, NULL);
    numOfThreads = atoi(argv[1]);
    numOfReps = atoi(argv[2]);
    int testType = atoi(argv[3]);
    pthread_t threads[numOfThreads];

    threadReps = (int)(numOfReps / numOfThreads);


    printf("Threads: %i\nReps: %i\n", numOfThreads, threadReps);

    begin = clock();
    int i;
	for (i = 0; i < numOfThreads; i++) {
		pthread_create (&threads[i],NULL, best,(void *)i);
	}


    for(i=0;i<numOfThreads;i++) {
        pthread_join( threads[i], NULL);
    }
    end = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

    double pi = 4*(ni/numOfReps);
    printf("TimeSpent: %d\n", timeSpent);
    printf( "Result: %f\n", pi);

    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}
