#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>

#define MSG_SIZE 32
#define NUM_WORKERS 4
#define ROOT 0
#define TAG 0

char messages[4][MSG_SIZE] = {"", "HELLO", "CSE", "RVCE"};
int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv); 

	int rank, numWorkers;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numWorkers);

	if (numWorkers != NUM_WORKERS) {
		printf("This needs %d workers to run correctly\n", NUM_WORKERS);
		exit(1);
	}
	
	char msg[MSG_SIZE];

	if (rank != ROOT) {
		strcpy(msg, messages[rank]);
		MPI_Send(&msg, MSG_SIZE, MPI_CHAR, ROOT, TAG, MPI_COMM_WORLD);
	}
	else {
		for (int i=1; i<numWorkers; i++) {
			MPI_Recv(&msg, MSG_SIZE, MPI_CHAR, i, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Process %d received %s from process %d\n", rank, msg, i);
		}
	}
	MPI_Finalize();
}
	

	
