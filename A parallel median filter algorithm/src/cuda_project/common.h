#pragma once 

/*
	TEST PROCEDURE:

	To test the different filtersizes (the size of the "brush" which remove clutter)
	uncomment the test size that you want and build+run the program.
	From here you can test it any amount of times (using the executable in the debug folder). 
	To use a different filtersize just compile with the desired size and run again.
*/

//#define filtersize 1 
#define filtersize 2 
//#define filtersize 3

//Do not touch the filterarray size or size1, size2 and size3.
#define filterarraysize ((filtersize*2+1)*(filtersize*2+1))
#define size1 "size1.bmp"
#define size2 "size2.bmp"
#define size3 "size3.bmp"

struct Pixel {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned int value;
}typedef Pixel;

struct ImageData {
	Pixel* data;
	int width;
	int height;
}typedef ImageData;

/*
	readBMP takes a filename. It then gets the BMP file and returns ImageData from it.
	Input: Existing BMP file (in same folder).
	Output: Image data that can be processed by the seqfilter and cudafilter.
*/
ImageData readBMP(char* filename);

/*
	SaveBMP takes ImageData and a filename and then proceeds to convert the ImageData
	to a new BMP file which then is saved under the name "'filename'.bmp".
	Input: ImageData (output from one of the filters) and a filename.
	Output: If the file was able to save "true" else "false".
*/
bool SaveBMP(ImageData image, char * filename);

/*
	The seqfilter takes ImageData and process it by checking the neighbors of each
	pixel and setting each pixel to the most common pixel data. This should have
	the effect of flattening out or even out the image. The filtersize is a variable
	which you change by commenting out the version you want to test.
	Input: ImageData (to be processed).
	Output: ImageData (processed according to algorithm).
*/
ImageData seqfilter(ImageData input);

/*
	Does the same as seqfilter but using the CUDA cores of the GPU! 
*/
ImageData cudaFilter(ImageData imageInput);