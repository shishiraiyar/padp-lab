#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include<omp.h>

char *toLower(char *str) {
	for (int i=0; str[i] != 0; i++) {
		str[i] = tolower(str[i]);
	}
}

int countWordInstancesInFile(char *fileName, char *searchWord) {

	char lowerWord[100];
	strcpy(lowerWord, searchWord);
	toLower(lowerWord);

	FILE *file = fopen(fileName, "r");
	
	int count = 0;	
	char word[100];

	while (fscanf(file, "%99s", word) == 1) {
		toLower(word);
		if (strcmp(word, lowerWord) == 0)
			count++;
	}
	
	fclose(file);
	return count;
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Error. Usage: %s fileName word1 word2 word3 .....\n", argv[0]);
		exit(1);
	}

	int numWords = argc - 2;

	double startTime = omp_get_wtime();	
	
	#pragma omp parallel for			
	for (int i=0; i<numWords; i++) {
		int count = countWordInstancesInFile(argv[1], argv[i + 2]);
		printf("Count of  %s: %d\n", argv[i + 2], count);
	}

	double endTime = omp_get_wtime();
	printf("Time taken: %f\n", endTime - startTime);
	
	return 0;
}	