#include "common.h"
#include <cmath>

ImageData seqfilter(ImageData input) {
	ImageData output;
	
	int heigh = output.height = input.height;
	int width = output.width = input.width;

	Pixel temp[filterarraysize-1];
	Pixel Biggest;
	Pixel tmp;
	Pixel* matrix = new Pixel[input.height*input.width];	//Allocating a new matrix for the output
	for (int y = 0; y < heigh; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int j=0;
			for (int i = 0; i < filterarraysize; i++)// the pixels around (x,y)
			{
				int target_x = x - filtersize + (i % (filtersize*2+1));
				int target_y = y - filtersize + (i / (filtersize*2+1));
				if (target_x < 0 || target_x >= width || target_y < 0 || target_y >= heigh || i==4)
					continue;
				//Place the biggest pixel furthest in the array
				Biggest = input.data[target_x + target_y*width];
				for (int iter = 0; iter < j; iter++)
				{
					//If a bigger value then the current is found, switch place and continues search
					if (Biggest.value < temp[iter].value) {
						tmp = temp[iter];
						temp[iter] = Biggest;
						Biggest = tmp;
					}
				}
				temp[j] = Biggest;

				j++;

			}
			//Now that temp[] has all neighbors, take the pixel in the middle (the midian)
			//If the amount of neighbors is even (n%2=0), take the avarge of the two middle pixels.
			if (j % 2 == 0)//Even
			{
				matrix[x+y*input.width].R = (temp[j / 2].R + temp[(j / 2 + 1)].R) / 2;
				matrix[x+y*input.width].G = (temp[j / 2].G + temp[(j / 2 + 1)].G) / 2;
				matrix[x+y*input.width].B = (temp[j / 2].B + temp[(j / 2 + 1)].B) / 2;
				matrix[x+y*input.width].value = (matrix[x+y*input.width].R + matrix[x+y*input.width].G + matrix[x+y*input.width].B) / 3;
			}
			else//Odd
				matrix[x+y*input.width] = temp[j / 2 + 1];
		}
	}

	output.data = matrix;
	return output;
}