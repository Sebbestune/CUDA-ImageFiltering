#include "common.h"
#include <time.h>
#include <stdio.h>
#include <Windows.h>

int main() {
	char* filename = size2;
	ImageData imageInput, imageOutput;
	imageInput = readBMP(filename);

	clock_t time;
	double seqTime, cudaTime;

	time = clock();
	imageOutput = seqfilter(imageInput);
	seqTime = double(clock() - time) / CLOCKS_PER_SEC;

	time = clock();
	imageOutput = cudaFilter(imageInput);
	cudaTime = double(clock() - time) / CLOCKS_PER_SEC;

	SaveBMP(imageOutput, filename);

	printf("Time: %lf (Seq)\n", seqTime);
	printf("Time: %lf (CUDA)\n", cudaTime);

	system("pause");

	delete[] imageInput.data;
	delete[] imageOutput.data;

	return 0;
}
