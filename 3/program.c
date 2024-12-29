#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>

int markMultiples(int *isComposite, int startIndex, int step, int endIndex) {
	int i;
	for (i = startIndex; i <= endIndex; i += step)
		isComposite[i] = 1;
	return i;
}

// cache unfriendly
int basicSieve(int upperLimit) {
	int sqrtLimit = (int) sqrt(upperLimit);
	int *isComposite = (int *) calloc(upperLimit + 1, sizeof(int));
	
	double startTime = omp_get_wtime();
	
	int primeCount = 0;
	for (int i = 2; i <= sqrtLimit; i++) {
		if (!isComposite[i]) {
			primeCount++;
			markMultiples(isComposite, i*i, i, upperLimit);
		}
	}
	for (int i = sqrtLimit + 1; i <= upperLimit; i++) {
		if (!isComposite[i])
			primeCount++;
	}
	double endTime = omp_get_wtime();
	printf("Basic Sieve time taken: %f\n", endTime - startTime);
	free(isComposite);
	return primeCount;
}

// cache friendly
int segmentedSieve(int upperLimit) {
	int sqrtLimit = (int)sqrt(upperLimit);
	int *isComposite = (int *)calloc(upperLimit + 1, sizeof(int));
	
	int *primes = (int *)malloc(sqrtLimit * sizeof(int));
	int *nextMultiple = (int *)malloc(sqrtLimit * sizeof(int));
	
	int primeCountUpToSqrt = 0;

	double startTime = omp_get_wtime();

	int totalPrimeCount = 0;
	for (int i = 2; i <= sqrtLimit; i++) {
		if (!isComposite[i]) {
			totalPrimeCount++;
			primes[primeCountUpToSqrt] = i;
			nextMultiple[primeCountUpToSqrt] = markMultiples(isComposite, i * i, i, sqrtLimit);
			primeCountUpToSqrt++;
		}
	}
	
	for (int left = sqrtLimit + 1; left <= upperLimit; left += sqrtLimit) {
		int right = (left + sqrtLimit - 1 < upperLimit) ? left + sqrtLimit - 1 : upperLimit;

		for (int j = 0; j < primeCountUpToSqrt; j++) {
			nextMultiple[j] = markMultiples(isComposite, nextMultiple[j], primes[j], right);
		}
		for (int i = left; i <= right; i++) {
			if (!isComposite[i])
				totalPrimeCount++;
		}
	}
	
	double endTime = omp_get_wtime();
	printf("Segmented sieve time: %f\n", endTime - startTime);
	free(isComposite);
	free(primes);
	free(nextMultiple);
	return totalPrimeCount;
}

// parallel
int parallelSegmentedSieve(int upperLimit) {
	int sqrtLimit = (int)sqrt(upperLimit);
	int *isComposite = (int *)calloc(upperLimit + 1, sizeof(int));
	
	int *primes = (int *)malloc(sqrtLimit * sizeof(int));
	int *nextMultiple = (int *)malloc(sqrtLimit * sizeof(int));
	
	int primeCountUpToSqrt = 0;

	double startTime = omp_get_wtime();

	int totalPrimeCount = 0;
	for (int i = 2; i <= sqrtLimit; i++) {
		if (!isComposite[i]) {
			totalPrimeCount++;
			primes[primeCountUpToSqrt] = i;
			nextMultiple[primeCountUpToSqrt] = markMultiples(isComposite, i * i, i, sqrtLimit);
			primeCountUpToSqrt++;
		}
	}
	
	for (int left = sqrtLimit + 1; left <= upperLimit; left += sqrtLimit) {
		int right = (left + sqrtLimit - 1 < upperLimit) ? left + sqrtLimit - 1 : upperLimit;
		
		#pragma omp parallel for
		for (int j = 0; j < primeCountUpToSqrt; j++) {
			nextMultiple[j] = markMultiples(isComposite, nextMultiple[j], primes[j], right);
		}

		#pragma omp parallel for reduction(+:totalPrimeCount)
		for (int i = left; i <= right; i++) {
			if (!isComposite[i])
				totalPrimeCount++;
		}
	}
	
	double endTime = omp_get_wtime();
	printf("Parallel segmented sieve time: %f\n", endTime - startTime);
	free(isComposite);
	free(primes);
	free(nextMultiple);
	return totalPrimeCount;
}


int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Error. Usage: %s upperLimit\n", argv[0]);
	}
	int upperLimit = atoi(argv[1]);
	printf("Basic sieve prime count: %d\n", basicSieve(upperLimit));

	printf("Segmented sieve prime count: %d\n", segmentedSieve(upperLimit));

	printf("Parallel segmented sieve prime count: %d\n", parallelSegmentedSieve(upperLimit));
}	
	

