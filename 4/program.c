#include<stdio.h>
#include<stdlib.h>

#include<omp.h>
#include<gd.h>

int main(int argc, char *argv[]){
	if (argc != 3) {
		printf("Error. Usage: %s inputFilename.png outputFilename.png\n", argv[0]);
		exit(1);
	}

	FILE *inputFile = fopen(argv[1], "rb");
	FILE *outputFile = fopen(argv[2], "wb");

	if (!inputFile || !outputFile) {
		printf("Error in opening files\n");
		return 1;
	}

	gdImagePtr inImage = gdImageCreateFromPng(inputFile);
	if (!inImage)
		printf("Input file not png (or broken png)\n");
	int w = gdImageSX(inImage);
	int h = gdImageSY(inImage);

	gdImagePtr outImage = gdImageCreateTrueColor(w, h);
	
	if (!outImage){
		printf("Couldn't create output image of the same dimensions\n");
		return 1;
	}
	
	double startTime = omp_get_wtime();

	#pragma omp parallel for collapse(2)
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int color = gdImageGetPixel(inImage, x, y);
			int avgColor = 0.33 * gdImageRed(inImage, color) + 0.33 * gdImageGreen(inImage, color) + 0.33 * gdImageBlue(inImage, color);
			gdImageSetPixel(outImage, x, y, gdImageColorAllocate(outImage, avgColor, avgColor, avgColor));
	
		}
	}

	gdImagePng(outImage, outputFile);
	gdImageDestroy(inImage);
	gdImageDestroy(outImage);
	
	fclose(inputFile);
	fclose(outputFile);
	
	double endTime = omp_get_wtime();
	printf("Time taken: %f\n", endTime - startTime);

	return 0;	
}
