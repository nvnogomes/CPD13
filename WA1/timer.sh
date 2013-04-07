#!/bin/bash

# compiling all the source code
make clean
make all


# running the different version of
echo "Taking times on a dual-core processor"
echo -e "Each version will be run 10 times\n"

echo "Timing sequential version"
acc=0
for i in `seq 1 10`;
do
	cexec=$(./bin/mandelbrot_seq);
	acc=$(echo "$acc + $cexec" | bc);
done
avg=$(echo "$acc/10.0" | bc -l)
echo -e "Average time:" $avg"s\n"


echo "Timing openMP version (static, 1 core)"
acc=0
for i in `seq 1 10`;
do
	cexec=$(./bin/mandelbrot_smp 1);
	acc=$(echo "$acc + $cexec" | bc);
done
avg=$(echo "$acc/10.0" | bc -l)
echo -e "Average time:" $avg"s\n"


echo "Timing openMP version (static, 2 core)"
acc=0
for i in `seq 1 10`;
do
	cexec=$(./bin/mandelbrot_smp 2);
	acc=$(echo "$acc + $cexec" | bc);
done
avg=$(echo "$acc/10.0" | bc -l)
echo -e "Average time:" $avg"s\n"


echo "Timing openMP version (static, 4 core)"
acc=0
for i in `seq 1 10`;
do
    cexec=$(./bin/mandelbrot_smp 4);
    acc=$(echo "$acc + $cexec" | bc);
done
avg=$(echo "$acc/10.0" | bc -l)
echo -e "Average time:" $avg"s\n"

echo "Timing openMP version (dynamic, 1 core)"
acc=0
for i in `seq 1 10`;
do
	cexec=$(./bin/mandelbrot_dmp 1);
	acc=$(echo "$acc + $cexec" | bc);
done
avg=$(echo "$acc/10.0" | bc -l)
echo -e "Average time:" $avg"s\n"


echo "Timing openMP version (dynamic, 2 core)"
acc=0
for i in `seq 1 10`;
do
	cexec=$(./bin/mandelbrot_dmp 2);
	acc=$(echo "$acc + $cexec" | bc);
done
avg=$(echo "$acc/10.0" | bc -l)
echo -e "Average time:" $avg"s\n"


echo "Timing openMP version (dynamic, 4 core)"
acc=0
for i in `seq 1 10`;
do
    cexec=$(./bin/mandelbrot_dmp 4);
    acc=$(echo "$acc + $cexec" | bc);
done
avg=$(echo "$acc/10.0" | bc -l)
echo -e "Average time:" $avg"s\n"
