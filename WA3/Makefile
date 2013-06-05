# GNU Compiler Compiler
CC = g++

# flags for showing warnings and be utf8 aware
CFLAGS  = -g -Wall


# FastFlow library
INCFF = -I fastflow-2.0.1/

# POSIX Threads
PTHREAD = -lpthread


.IGNORE:	# ignore problems (as far as possible)
.SILENT:	# don't echo commands executed


all: histogram segmentation 

# PROGRAMS
histogram: 
	$(CC) $(CFLAGS) $(INCFF) $(PTHREAD) -o bin/histogram histogram.cpp

segmentation: 
	$(CC) $(CFLAGS) $(INCFF) $(PTHREAD) -o bin/segmentation bi-segmentation.cpp


clean:
	rm -f bin/*
