# GNU Compiler Compiler
CC = gcc

# mpi compiler wrap
MPICC = mpic++

# flags for showing warnings and be utf8 aware
CFLAGS  = -g -Wall

# OpenMP
OMPFLAG = -fopenmp

# additional libraries
LIBS= -lgomp 


.IGNORE:	# ignore problems (as far as possible)
.SILENT:	# don't echo commands executed


PROGRAMS= mandelbrot_omp mandelbrot_mpi
all: $(PROGRAMS)


mandelbrot_omp:
	$(CC) $(CFLAGS) $(LIBS) -o mandelbrot_omp mandelbrot_omp.c walltime.c

mandelbrot_mpi:
	$(MPICC) $(CFLAGS) -o mandelbrot_mpi mandelbrot_mpi.c walltime.c

clean:
	rm -rf $(PROGRAMS) walltime.o *.dSYM *~
