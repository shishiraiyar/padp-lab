#!/usr/bin/bash
thread_counts=(1 2 4 8)
num_iterations=(100 1000 10000 100000)

mpicc program.c

for n_iter in ${num_iterations[@]}; do
	for tc in ${thread_counts[@]}; do
		mpirun -n $tc ./a.out $n_iter
		echo
	done
done
