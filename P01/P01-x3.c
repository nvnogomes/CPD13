#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE 50*1024*1024
int *array;
int length, count;

int *cs;
pthread_t *ids;
int length_per_thread;
pthread_mutex_t ex = PTHREAD_MUTEX_INITIALIZER;

void * count3s_thread(void *id){
	int i;
	int start =  length_per_thread*(int)id;
	int end = start + length_per_thread;
	cs[(int)id] = 0;
//	printf("i = %d, start = %d, end= %d\n", (int)id, start, end);	
	for(i=start; i < end; i++){
		if(array[i] == 3){
			cs[(int)id]++;
		}
	}
	pthread_mutex_lock(&ex);
	count+=cs[(int)id];
	pthread_mutex_unlock(&ex);

	return NULL;
}

void count3s(int nthreads){
	int i;
	
	for(i=0; i < nthreads; i++){
		pthread_create(&ids[i], NULL, count3s_thread, (void *)i);
	}

	for(i=0; i < nthreads; i++){
		pthread_join(ids[i], NULL);
	}
}

int main( int argc, char *argv[]){
	int i;

	struct timeval s,t;

	count = 0;
	int n = atoi(argv[1]);
	ids = (pthread_t *)malloc(n*sizeof(pthread_t));
	cs = (int *)malloc(n*sizeof(int));
	

	length_per_thread = SIZE / n;
	printf("Using %d threads; length_per_thread =  %d\n", n, length_per_thread);
	
	array= (int *)malloc(SIZE*sizeof(int));
	length = SIZE;
	// srand(getpid());
	srand(0);
	for(i=0; i < length; i++){
		array[i] = rand() % 4;
	}

	gettimeofday(&s, NULL);
	count3s(n);
	gettimeofday(&t, NULL);

	printf("Count of 3s = %d\n", count);
	printf("Elapsed time (us) = %ld\n", 
		(t.tv_sec - s.tv_sec)*1000000 + (t.tv_usec - s.tv_usec));
	return 0;
}


