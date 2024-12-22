// Assumes square matrix for simplicity
// Takes random values as input

#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main (int argc, char *argv[]) {
	if (argc != 3) {
		printf("Error. Usage: %s squareMatrixSize numberOfThreads\n", argv[0]);
	exit(1);
	}
	
	int size = atoi(argv[1]);

	int *A = malloc(size * size * sizeof(int));
	int *B = malloc(size * size * sizeof(int));
	int *C = malloc(size * size * sizeof(int));

	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++){
			A[i * size + j] = rand() % 10;
			B[i * size + j] = rand() % 10;
			C[i * size + j] = 0;
		}
	}

	int threadCount = atoi(argv[2]);
	omp_set_num_threads(threadCount);

	double startTime = omp_get_wtime();
	
	#pragma omp parallel for 
	for (int i=0; i<size; i++){
		for (int j=0; j<size; j++){
			for (int k=0; k<size; k++){
				C[i * size + k] += A[i * size + j] + B[j * size + k];
			}
		}
	}
	double endTime = omp_get_wtime();

	printf("Matrix size: %d\nNumber of threads: %d\nTime taken: %f\n", size, threadCount, endTime - startTime);

	free(A);
	free(B);
	free(C);
 
	return 0;
}
