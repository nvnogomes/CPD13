# GNU Compiler Compiler
CC = gcc

# flags for showing warnings and be utf8 aware
CFLAGS  = -g -Wall


# openMPI compiler wrap
MPICC = mpicc


# OpenMP flags
OMPFLAG = -fopenmp -lm
LIBS = -lgomp 


# openCL flags and library
CL_LIBRARY=-L /opt/AMDAPP/lib/x86_64
CL_INCLUDE=-I /opt/AMDAPP/include
CL_CFLAG=-lOpenCL


.IGNORE:	# ignore problems (as far as possible)
.SILENT:	# don't echo commands executed


PROGRAMS = seq mp mpi mp2i
CLIBS = mandel walltime defs


all: $(CLIBS) $(PROGRAMS)
library: $(CLIBS)
openCL: $(CLIBS) cl


# CLIBS
mandel: lib/mandel.c lib/mandel.h
	$(CC) $(CFLAGS) -c lib/mandel.c -o bin/mandel.o

walltime: lib/walltime.c lib/walltime.h
	$(CC) $(CFLAGS) -c lib/walltime.c -o bin/walltime.o

defs: lib/definitions.c lib/definitions.h
	$(CC) $(CFLAGS) -c lib/definitions.c -o bin/definitions.o


# PROGRAMS
seq: mandel defs walltime
	$(CC) $(CFLAGS) -o bin/mandelbrot_seq seq/mandelbrot_seq.c bin/mandel.o bin/definitions.o bin/walltime.o

mp: dmp smp

smp: walltime mandel defs
	$(CC) $(CFLAGS) $(OMPFLAG) -o bin/mandelbrot_smp openMP/mandelbrot_smp.c bin/definitions.o bin/mandel.o bin/walltime.o

dmp: walltime mandel defs
	$(CC) $(CFLAGS) $(OMPFLAG) -o bin/mandelbrot_dmp openMP/mandelbrot_dmp.c bin/definitions.o bin/mandel.o bin/walltime.o

mpi: mandel defs
	$(MPICC) $(CFLAGS) -o bin/mandelbrot_mpi openMPI/mandelbrot_mpi.c bin/mandel.o bin/definitions.o

mp2i: mandel defs walltime
	$(MPICC) $(CFLAGS) $(OMPFLAG) -o bin/mandelbrot_mp2i openMP2I/mandelbrot_mp2i.c bin/mandel.o bin/walltime.o bin/definitions.o

cl: mandel defs walltime
	$(CC) $(CL_INCLUDE) -o bin/mandelbrot_cl openCL/mandelbrot_cl.c bin/mandel.o bin/definitions.o bin/walltime.o $(CL_FLAG) $(CL_LIBRARY) 


clean:
	rm -f bin/*
