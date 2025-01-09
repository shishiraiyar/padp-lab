#!/usr/bin/bash
thread_counts=(1 2 4 8)
matrix_sizes=(100 200 300 400)

cc program.c -fopenmp

for m_size in ${matrix_sizes[@]}; do
	for tc in ${thread_counts[@]}; do
		./a.out $m_size $tc
		echo
	done
done
