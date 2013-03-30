# GNU Compiler Compiler
CC = gcc

# mpi compiler wrap
MPICC = mpic++

# flags for showing warnings and be utf8 aware
CFLAGS  = -g -Wall

# OpenMP
OMPFLAG = -fopenmp

# additional libraries
LIBS = -lgomp 



.IGNORE:	# ignore problems (as far as possible)
.SILENT:	# don't echo commands executed


PROGRAMS = mp mpi mp2i cl
LIBS = mandel walltime


all: $(LIBS) $(PROGRAMS)
lib: $(LIBS)


mandel:
	$(CC) $(CFLAGS) -c lib/mandel.c -o lib/mandel.o
walltime:
	$(CC) $(CFLAGS) -c lib/walltime.c -o lib/walltime.o



mp: lib/walltime.o lib/mandel.o
	$(CC) $(CFLAGS) $(LIBS) -o bin/mandelbrot_omp openMP/mandelbrot_omp.c

mpi: lib/walltime.o lib/mandel.o
	$(MPICC) $(CFLAGS) -o bin/mandelbrot_mpi openMPI/mandelbrot_mpi.c

mp2i:
	$(MPICC) $(CFLAGS) $(LIBS) bin/mandelbrot_mp2i openMPI_MP/mandelbrot_mp2.c

cl:
	$(CC) $(CFLAGS)

clean:
	rm -f bin/* lib/*.o
