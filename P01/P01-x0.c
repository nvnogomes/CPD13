#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define SIZE 50*1024*1024
int *array;
int length, count;

int count3s(){
	int i;
	count = 0;
	
	for(i=0; i < length; i++){
		if(array[i] == 3){
			count++;
		}
	}
	return count;
}

int main( int argc, char *argv[]){
	int i, r;

	struct timeval s,t;
	array= (int *)malloc(SIZE*sizeof(int));
	length = SIZE;
//	srand(getpid());
	srand(0);
	for(i=0; i < length; i++){
		array[i] = rand() % 4;
	}
	gettimeofday(&s, NULL);
	r = count3s();
	gettimeofday(&t, NULL);

	printf("Count of 3s = %d\n", r);
	printf("Elapsed time (us) = %ld\n", 
		(t.tv_sec - s.tv_sec)*1000000 + (t.tv_usec - s.tv_usec));
	return 0;
}


