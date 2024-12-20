#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define SEED 1234
#define NUM_ITERATIONS 1000000

#define ROOT 0
int main(int argc, char *argv[]){
	MPI_Init(&argc, &argv);
	
	int worldSize; // Number of workers
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	int workerRank; // unique id of worker
	MPI_Comm_rank(MPI_COMM_WORLD, &workerRank);

	double startTime;
	if (workerRank == ROOT) // only the first worker keeps track of time 
		startTime = MPI_Wtime();
	
	int circleHits = 0;

	srand(SEED + workerRank); // so that each worker gets a unique seed so they get different random numbers
		
	for (int i = 0; i < NUM_ITERATIONS / worldSize; i++) { // divide iterations equally among all workers
		double x = (double) rand() / RAND_MAX;
		double y = (double) rand() / RAND_MAX;
		
		if (x*x + y*y < 1)
			circleHits++;
	}
	int totalCircleHits;
	
	MPI_Reduce (&circleHits, &totalCircleHits, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);
	
	if (workerRank == ROOT) {
		double pi = (double) totalCircleHits / NUM_ITERATIONS * 4;
		double endTime = MPI_Wtime();
	
		printf("Number of Iterations: %d\n", NUM_ITERATIONS);
		printf("Number of Threads: %d\n", worldSize);
		printf("PI: %f\n", pi);
		printf("Time Taken: %f\n", endTime - startTime);
	}

	MPI_Finalize();
	return 0;
}
