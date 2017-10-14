#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ void add(float *c, float* a, float *b, int values){
	int blockD = blockDim.x;
	int blockX = blockIdx.x;
	int threadX = threadIdx.x;

	int i = blockX * blockD + threadX;
	if(i < values)
		c[i] = a[i] + b[i];
	//printf("Hello Im thread %d in block %d of %d threads\n", threadX, blockX, blockD);
}

__host__ int main (int argc, char *argv[]){

	int numValues = atoi(argv[1]);
	int blocksize = atoi(argv[2]);
	
	printf("Using program with %d values and %d blocksize\n", numValues, blocksize);

	float *c = (float*)malloc(numValues*sizeof(float));
	float *a = (float*)malloc(numValues*sizeof(float));
        float *b = (float*)malloc(numValues*sizeof(float));
	
	float *c_d, *b_d, *a_d;
	cudaMalloc((void**)&c_d, numValues*sizeof(float));
	cudaMalloc((void**)&b_d, numValues*sizeof(float));
	cudaMalloc((void**)&a_d, numValues*sizeof(float));

	for(int i=0; i < numValues; i++){
		c[i] = 0.0;
		a[i] = 3.0;
		b[i] = 5.0;
	}
	printf("Done init\n");
	int numBlocks = numValues/blocksize;
	
	printf("Copying arrays from host to device\n");
	cudaMemcpy(a_d, a, numValues*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(b_d, b, numValues*sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(c_d, c, numValues*sizeof(float), cudaMemcpyHostToDevice);

	add<<<numBlocks,blocksize>>>(c_d, a_d, b_d, numValues);
	cudaDeviceSynchronize();
	
	printf("Copying values back to host\n");

	cudaMemcpy(c, c_d, numValues*sizeof(float), cudaMemcpyDeviceToHost);
	
	for(int i=0; i < numValues; i++)
		printf("C[%d] = %f\n", i, c[i]);
	return 0;
}
