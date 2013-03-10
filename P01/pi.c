
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>


double piSum = 0;
pthread_mutex_t mutex;


void *sim(void *N) {

    int repetitions = (int)N;
    double acc = 0;

    int i;
    for( i=0; i < repetitions; i++ ) {
        double x = rand_r(*i) % 100;
        double y = rand_r(*i) % 100;
        printf("%d %d %i\n", x,y, (rand()%2));
        if( x*x + y*y <= 1) {
            acc++;
        }
    }

    //pthread_mutex_lock(&mutex);
    piSum += acc;
    //pthread_mutex_unlock(&mutex);
}


int main (int argc, char *argv[]) {

    if(argc!=3){
        printf("Usage:%s #threads #tests\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);
    int numOfThreads = atoi(argv[1]);
    int numOfReps = atoi(argv[2]);
    pthread_t threads[numOfThreads];

    int threadReps = (int)(numOfReps / numOfThreads);

    printf("Threads: %i, numOfReps: %i, threadReps: %i\n", numOfThreads, numOfReps, threadReps);

    int i;
	for (i = 0; i < numOfThreads; i++) {
		pthread_create (&threads[i],NULL, sim,(void *)threadReps);
	}

    for(i=0;i<numOfThreads;i++) {
        pthread_join( threads[i], NULL);
    }

	printf("piSum: %i, numOfReps: %i\n", piSum, numOfReps);
    double pi = 4*(piSum/numOfReps);

    printf( "Result: %f\n", pi);

    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}
