#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include "common.h"

__global__ void filterKernel(Pixel *input, Pixel *output, int width, int height)
{
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	int x = blockIdx.x * blockDim.x + threadIdx.x;

	int total = width * height;
	Pixel temp[filterarraysize-1];
	Pixel Biggest;
	Pixel tmp;
	int j = 0;

	if (y >= height || x >= width)
		return;

	for (int i = 0; i < filterarraysize; i++)// the pixels around (x,y) (how many depends on filtersize)
	{
		__syncthreads();
		int target_x = x -filtersize + (i % (filtersize*2+1));
		int target_y = y -filtersize + (i / (filtersize*2+1));
		if (target_x < 0 || target_x >= width || target_y < 0 || target_y >= height || i==4)
			continue;
		//Place the biggest pixel furthest in the array
		Biggest = input[target_x + target_y*width];
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
	//Now that temp[] has all neighbors, take the pixel in the middle (the median)
	//If the amount of neighbors is even (n%2=0), take the avarge of the two middle pixels.
	if (j % 2 == 0)//Even
	{
		output[x+y*width].R = (temp[j / 2].R + temp[(j / 2 + 1)].R) / 2;
		output[x+y*width].G = (temp[j / 2].G + temp[(j / 2 + 1)].G) / 2;
		output[x+y*width].B = (temp[j / 2].B + temp[(j / 2 + 1)].B) / 2;
		output[x+y*width].value = (output[x+y*width].R + output[x+y*width].G + output[x+y*width].B) / 3;
	}
	else//Odd
		output[x+y*width] = temp[j / 2 + 1];
}


ImageData cudaFilter(ImageData imageInput)
{
	ImageData output;
	
	int height = output.height = imageInput.height;
	int width = output.width = imageInput.width;
	int total = height * width; 

	Pixel* matrix = new Pixel[height*width];	//Allocating a new matrix for the output
	
	Pixel *dev_input;
    Pixel *dev_matrix;

    // Allocate GPU buffers for three vectors (one input, one output).
    cudaMalloc((void**)&dev_input, total * sizeof(Pixel));
    cudaMalloc((void**)&dev_matrix, total * sizeof(Pixel));
    
    // Copy input vectors from host memory to GPU buffers.
	cudaMemcpy(dev_input, imageInput.data, total * sizeof(Pixel), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_matrix, matrix, total * sizeof(Pixel), cudaMemcpyHostToDevice);
    
    // Launch a 2D-kernel on the GPU! 
	dim3 block(16,16);
	//dim3 grid ((total+15)/16, (total+15)/16);
	dim3 grid ((width+15)/16, (height+15)/16);
	filterKernel<<<grid,block>>>(dev_input, dev_matrix, width, height);
    
    // Copy output vector from GPU buffer to host memory.
    cudaMemcpy(matrix, dev_matrix, total * sizeof(Pixel), cudaMemcpyDeviceToHost);
    
	cudaFree(dev_input);
	cudaFree(dev_matrix);

	output.data = matrix;
	return output;   
}