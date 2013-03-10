
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>


double acc;
pthread_mutex_t mutex;


void *f(void *arg) {

    int repetitions = (int)arg;
    // init randomize with thread id
    srand(pthread_self());

    int i,local;
    for( i=0; i < repetitions; i++ ) {
		local++;
    }

    pthread_mutex_lock(&mutex);
    acc += local;
    pthread_mutex_unlock(&mutex);
}



int main (int argc, char *argv[]) {

    if(argc!=3){
        printf("Usage:%s #threads #tests\n", argv[0]);
        exit(1);
    }

    int numOfThreads = atoi(argv[1]);
    int numOfReps = atoi(argv[2]);
    int threadReps = (int)(numOfReps / numOfThreads);

	pthread_t threads[numOfThreads];
    pthread_mutex_init(&mutex, NULL);

    printf("Threads: %i\nReps: %i\n", numOfThreads, threadReps);

    int i;
	for (i = 0; i < numOfThreads; i++) {
		pthread_create (&threads[i],NULL, f,(void *)numOfReps);
	}

    for(i=0;i<numOfThreads;i++) {
        pthread_join( threads[i], NULL);
    }


    printf( "Result: %f\n", acc);

    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}
