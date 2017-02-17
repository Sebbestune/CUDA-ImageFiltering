#include <fstream> // for file I/O
#include "common.h"
using namespace std;

ImageData readBMP(char* filename)
{
	ImageData returnImage;
	FILE* f = fopen(filename, "rb");
	if (f == NULL) {
		printf("Can't open file (%s)",filename);
		return returnImage;
	}
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

											   // extract image height and width from header
	int width = returnImage.width = *(int*)&info[18];
	int height = returnImage.height = *(int*)&info[22];

	int size = 3 * width * height;
	unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
	
	Pixel* matrix = new Pixel[height*width];



	fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int bufpos = x * 3 + (y*width * 3);
			matrix[x+y*width].R = data[bufpos + 2];
			matrix[x+y*width].G = data[bufpos + 1];
			matrix[x+y*width].B = data[bufpos + 0];
			matrix[x+y*width].value = (matrix[x+y*width].R + matrix[x+y*width].G + matrix[x+y*width].B) / 3;
		}
	}
	delete[] data;
	returnImage.data = matrix;
	return returnImage;
}

bool SaveBMP(ImageData image, char * filename)
{
	//Read the old header file
	FILE* f = fopen(filename, "rb");
	if (f == NULL) {
		printf("Can't open file (%s)", filename);
		return false;
	}
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
	fclose(f);

	//Create a file to save the result
	f = fopen("Output.bmp", "wb");

	fwrite(info, sizeof(unsigned char), 54, f);
	int size = 3 * image.width * image.height;
	unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
	Pixel *matrix = image.data;
	for (int y = 0; y < image.height; y++)
	{
		for (int x = 0; x < image.width; x++)
		{
			int bufpos = x * 3 + (y*image.width * 3);
			data[bufpos + 2] = matrix[x+y*image.width].R;
			data[bufpos + 1] = matrix[x+y*image.width].G;
			data[bufpos + 0] = matrix[x+y*image.width].B;
		}
	}

	fwrite(data, sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);
	delete[] data;
	return true;

}