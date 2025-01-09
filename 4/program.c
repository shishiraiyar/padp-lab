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
	if (!inImage) {
		printf("Input file not png (or broken png)\n");
		return 1;
	}
	int w = gdImageSX(inImage);
	int h = gdImageSY(inImage);

	gdImagePtr outImage = gdImageCreateTrueColor(w, h);
	
	if (!outImage){
		printf("Couldn't create output image of the same dimensions\n");
		return 1;
	}

	omp_set_num_threads(4);

	double startTime = omp_get_wtime();

	#pragma omp parallel for schedule(static, 50) // change to dynamic and guided
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int color = gdImageGetPixel(inImage, x, y);
			int avgColor = 0.33 * gdImageRed(inImage, color) + 0.33 * gdImageGreen(inImage, color) + 0.33 * gdImageBlue(inImage, color);
			int outputPixelColor;
			int tid = omp_get_thread_num();
	
			switch(tid) {
				case 0: outputPixelColor = gdImageColorAllocate(outImage, avgColor, 0, 0); break;
				case 1: outputPixelColor = gdImageColorAllocate(outImage, 0, avgColor, 0); break;
				case 2: outputPixelColor = gdImageColorAllocate(outImage, 0, 0, avgColor); break;
				case 3: outputPixelColor = gdImageColorAllocate(outImage, avgColor, avgColor, avgColor);
			}

			gdImageSetPixel(outImage, x, y, outputPixelColor);
	
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
