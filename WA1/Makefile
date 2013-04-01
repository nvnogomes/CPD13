# GNU Compiler Compiler
CC = gcc

# mpi compiler wrap
MPICC = mpicc

# flags for showing warnings and be utf8 aware
CFLAGS  = -g -Wall

# OpenMP
OMPFLAG = -fopenmp

# additional libraries
LIBS = -lgomp 



.IGNORE:	# ignore problems (as far as possible)
.SILENT:	# don't echo commands executed


PROGRAMS = seq mp mpi mp2i cl
CLIBS = mandel walltime defs


all: $(CLIBS) seq mpi
library: $(CLIBS)


# CLIBS
mandel: lib/mandel.c lib/mandel.h
	$(CC) $(CFLAGS) -c lib/mandel.c -o bin/mandel.o

walltime: lib/walltime.c lib/walltime.h
	$(CC) $(CFLAGS) -c lib/walltime.c -o bin/walltime.o

defs: lib/definitions.c lib/definitions.h
	$(CC) $(CFLAGS) -c lib/definitions.c -o bin/definitions.o


# PROGRAMS
seq: mandel defs
	$(CC) $(CFLAGS) -o bin/mandelbrot_seq seq/mandelbrot_seq.c bin/mandel.o bin/definitions.o

mp: walltime mandel defs
	$(CC) $(CFLAGS) $(LIBS) -o bin/mandelbrot_omp openMP/mandelbrot_omp.c bin/definitions.o bin/mandel.o bin/walltime.o

mpi: mandel defs
	$(MPICC) $(CFLAGS) -o bin/mandelbrot_mpi openMPI/mandelbrot_mpi.c bin/mandel.o bin/definitions.o

mp2i: mandel defs
	$(MPICC) $(CFLAGS) $(LIBS) -o bin/mandelbrot_mp2i openMPI_MP/mandelbrot_mp2i.c bin/mandel.o bin/walltime.o bin/definitions.o

cl:
	$(CC) $(CFLAGS)

clean:
	rm -f bin/*
