CC=gcc-4.2
CFLAGS= -g -fopenmp
LIBS= -lgomp 

PROGRAMS= taylor taylor_mp
all: $(PROGRAMS)

taylor: taylor.c walltime.o
	$(CC) -g -o $@ $<  walltime.o

taylor_mp: taylor_mp.c walltime.o
	$(CC) $(CFLAGS) -o $@ $< walltime.o $(LIBS) -lm


walltime.o: walltime.c
	$(CC) -g -c $<

clean:
	rm -rf $(PROGRAMS) walltime.o *.dSYM *~

