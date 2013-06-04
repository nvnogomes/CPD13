# GNU Compiler Compiler
CC = g++

# flags for showing warnings and be utf8 aware
CFLAGS  = -g -Wall


# FastFlow library
INCFF = -I fastflow-2.0.1

# POSIX Threads
PTHREAD = -lpthread


.IGNORE:	# ignore problems (as far as possible)
.SILENT:	# don't echo commands executed


all: shadow

# PROGRAMS
shadow: 
	$(CC) $(CFLAGS) $(PTHREAD) -o bin/skeleton skeleton.cpp

clean:
	rm -f bin/skeleton