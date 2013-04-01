#ifndef LIB_MANDEL_H
#define LIB_MANDEL_H

int
compute_point(double ci, double cr);


void
compute(int *buffer, int nx, int ny,
        double xmin, double xmax, double ymin, double ymax);

void
output_pgm(char *filename,int *buffer, int nx, int ny, int max);


#endif
