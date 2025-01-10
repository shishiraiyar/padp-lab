#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>

double max(double a, double b) {
    return (a > b) ? a : b;
}

void printMatrix(double *matrix, int matrixSize) {
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++)
            printf("%f\t", matrix[i * matrixSize + j]);
        printf("\n");
    }
    printf("\n");
}

void initializeMatrixRandom(double *matrix, int matrixSize) {
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++)
            matrix[i * matrixSize + j] = (double) rand()/RAND_MAX * 100;
    }
}

int main (int argc, char *argv[]) {

    int n = 5;
    double tolerance = 0.1;
    int  maxIterations = 200;

    double *A = (double *) malloc(n * n * sizeof(double));
    double *Anew = (double *) malloc(n * n * sizeof(double));
    
    initializeMatrixRandom(A, n);
    printMatrix(A, n);

    double error = tolerance + 1; // Just to get into the while loop initially
    int iterationCount = 0;

    double startTime = omp_get_wtime();

    while (error > tolerance && iterationCount < maxIterations) {
        error = 0;
        
        for (int i = 1; i<n - 1; i++) {
            for (int j = 1; j<n - 1; j++) {
                Anew[i*n + j] = 0.25 * (A[(i-1)*n + j] + A[(i+1)*n + j] + 
                                        A[i*n + (j-1)] + A[i*n + (j+1)]);
                error = fmax(error, fabs(Anew[i*n + j] - A[i*n + j]));
            }
        }
        
        for (int i = 1; i<n - 1; i++) {
            for (int j = 1; j<n - 1; j++) {
                A[i*n + j] = Anew[i*n + j];
            }
        }
        // printMatrix(A, n);
    }

    double endTime = omp_get_wtime();

    printf("Time taken: %f\n", endTime - startTime);
}