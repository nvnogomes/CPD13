#!/bin/bash

# compiling all the source code
make clean
make all

declare -a Unix

# running the different version of

echo "Running sequential version"
for i in 1 2 3 4 5 6 7 8 9 10
do
	./bin/mandelbrot_seq
done 

