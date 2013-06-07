#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define OFFSET 12

unsigned char *readImage( char *fileName){
	FILE *f;
	int l, h, p;
	unsigned char *pt;

	f = fopen(fileName, "rb");
	if(f == NULL){
		printf("%s does not exist\n", fileName);
		return NULL;
	}

	fscanf(f,"%d %d %d\n", &l, &h, &p);
	printf("L=%d H= %d P= %d\n", l, h, p);
	fseek( f, OFFSET, SEEK_SET);
	
    pt = (unsigned char*) malloc(l*h*p);
	fread(pt, 1, l*h*p, f);
	return pt;
}




