#include<stdio.h>
#include<stdlib.h>

#include<omp.h>

#define SEED 1234

// area of quarter of a circle with radius 1 / area of square with side length 1

int main(int argc, char *argv[]){
	if (argc != 3) {
		printf("Pass number of iterations and number of threads\n");
		exit(1);
	}
	int numIterations = atoi(argv[1]);
	int numThreads = atoi(argv[2]);

	srand(SEED);
	int circleHits = 0;
	double x, y;	
	
	omp_set_num_threads(numThreads);	
	double startTime = omp_get_wtime();
	#pragma omp parallel for private(x, y) reduction(+ : circleHits)
	for (int i = 0; i < numIterations; i++){
		x = (double) rand()/RAND_MAX;
		y = (double) rand()/RAND_MAX;
		if ((x*x + y*y) < 1)
			circleHits++;
	}
	double pi = ((double)circleHits / numIterations) * 4;
	
	double endTime = omp_get_wtime();
	printf("PI: %f\nTime Taken: %f", pi, endTime - startTime);
		 
}	
