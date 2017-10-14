#include <stdio.h>

__global__ void hello2D()
{
	int blocksize = blockIdx.y * blockIdx.x;
	int blockId = gridDim.x * blockIdx.y + blockIdx.x;
	int tid = blockId * blocksize + blockDim.x * threadIdx.y + threadIdx.x;
	
	printf("I am thread (%d, %d) in block (%d, %d). Global thread ID = %d\n", threadIdx.y, threadIdx.x, blockIdx.y, blockIdx.x, tid);
}

__host__ int main(void)
{
	dim3 blocksize;
	dim3 gridsize;
	
	// 2D blocks in a 2D grid.
	gridsize.x = 3;
	gridsize.y = 2;
	blocksize.x = 3;
	blocksize.y = 4;
	
	hello2D<<<gridsize, blocksize>>>();
	cudaDeviceSynchronize();
	return 0;
}
