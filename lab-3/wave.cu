#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include <omp.h>

// Save wave state as binary file.
int setWaveSpace(int N, char *f, float *waveSpace)
{
	FILE *filestream = fopen(f, "w+b");
	
	if (!filestream)
	{
		printf("Couldn't open file.\n");
		return 1;
	}
	
	for (int i = 0; i < N; i++)
	{
		fwrite(&waveSpace[i], sizeof(float), N * N, filestream);
		// Print wave space as CSV format.
		for (int j = 0; j < N; j++)
			printf("%f;", waveSpace[N * i + j]);
		printf("\n");
	}
	
	fclose(filestream);

	return 0;
}

// Read a wave state from binary file.
int getWaveSpace(int N, char *f)
{
	FILE *filestream = fopen(f, "r+b");
	float ws[N * N];

	if (!filestream)
	{
		return 1;
	}

	fread(&ws, sizeof(float), N * N, filestream);
	
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			printf("%f;", ws[N * i + j]);
		printf("\n");
	}
	
	fclose(filestream);

	return 0;
}

// Filling wave space with T = 1.
void fillSpaceFirstStep(int N, float c, float dt, float dd, float *waveSpace, float *waveSpaceTMin1)
{
	for (int i = 1; i < N; i++)
		for (int j = 1; j < N - 1; j++)
			waveSpace[N * i + j] = waveSpaceTMin1[N * i + j] + (c * c)/2 * (dt/dd * dt/dd) * (waveSpaceTMin1[N * (i + 1) + j] + waveSpaceTMin1[N * (i - 1) + j] + waveSpaceTMin1[N * i + (j - 1)] + waveSpaceTMin1[N * i + (j + 1)] - 4 * waveSpaceTMin1[N * i + j]);
}

// Setting wave space as a grid with values "20" in center
// sorrounding with zero values.
void initializeSpace(int N, float *waveSpace)
{
	for (int i = 0; i < N * N; i++)
		waveSpace[i] = 0;
	
	for (int i = 0.4 * N; i < 0.6 * N; i++)
		for (int j = 0.4 * N; j < 0.6 * N; j++)
			waveSpace[N * i + j] = 20;
}

// Filling wave space with T > 1.
__device__ void fillSpaceTSteps(int N, int T, float c, float dt, float dd, float *waveSpace, float *waveSpaceTMin1, float *waveSpaceTMin2)
{

	int i = blockIdx.y * blockDim.y + threadIdx.y;
	int j = blockIdx.x * blockDim.x + threadIdx.x;

	/*for (int i = 1; i < N; i++)
		for (int j = 1; j < N - 1; j++)*/
			waveSpace[N * i + j] = 2 * waveSpaceTMin1[N * i + j] - waveSpaceTMin2[N * i + j] + (c * c) * (dt/dd * dt/dd) * (waveSpaceTMin1[N * (i + 1) + j] + waveSpaceTMin1[N * (i - 1) + j] + waveSpaceTMin1[N * i + (j - 1)] + waveSpaceTMin1[N * i + (j + 1)] - 4 * waveSpaceTMin1[N * i + j]);

	__syncthreads();
}

__global__ void schroedinger(float *waveSpace, float *waveSpaceTMin1, float *waveSpaceTMin2, int T, int N, char *f, int t)
{
	float c = 1.0;
	float dt = 0.1;
	float dd = 2.0;

	//printf("Inside kernel.\n");

	// Schroedinger ecuation, by a given step as input.
	for (int step = 2; step <= T; step++)
	{
		//printf("step: %d\n", step);
		/*switch(step)
		{
			case 0:
				initializeSpace(N, waveSpace);
				memcpy(waveSpaceTMin1, waveSpace, N * N * sizeof(float));
				break;
			case 1:
				fillSpaceFirstStep(N, c, dt, dd, waveSpace, waveSpaceTMin1);
				memcpy(waveSpaceTMin2, waveSpaceTMin1, N * N * sizeof(float));
				memcpy(waveSpaceTMin1, waveSpace, N * N * sizeof(float));
				break;
			default:*/
				fillSpaceTSteps(N, T, c, dt, dd, waveSpace, waveSpaceTMin1, waveSpaceTMin2);
				memcpy(waveSpaceTMin2, waveSpaceTMin1, N * N * sizeof(float));
				memcpy(waveSpaceTMin1, waveSpace, N * N * sizeof(float));
				/*break;
		}*/

		// Save step image specified by parameter t.
		//if (step == t)
		//	break;
			//setWaveSpace(N, f, waveSpace);
	}
}

__host__ int main(int argc, char **argv)
{
	double start = omp_get_wtime();

	dim3 numblocks;
	dim3 sizeblocks;	
	
	int N, T, X, Y, t;
	
	int selection;
	char *f;
	
	while ((selection = getopt(argc, argv, "N:T:X:Y:f:t:")) != -1)
	{
		switch(selection) {
			case 'N':
				N = atoi(optarg);
				break;
			case 'T':
				T = atoi(optarg);
				break;
			case 'X':
				X = atoi(optarg);
				break;
			case 'Y':
				Y = atoi(optarg);
				break;
			case 'f':
				f = optarg;
				remove(f);
				break;
			case 't':
				t = atoi(optarg);
				break;
		}
	}
	
	if (t > T)
	{
		printf("Error: output iteration can't be bigger than step quantity.\n");
		exit(1);
	}

	numblocks.x = (int)ceil((float)N/X);
	numblocks.y = (int)ceil((float)N/Y);

	/*printf("N: %d\n", N);
	printf("Y: %d\n", Y);
	printf("numblocks.x: %d\n", numblocks.x);
	printf("ceil: %d\n", (int)ceil((float)N/X));*/

	sizeblocks.x = X;
	sizeblocks.y = Y;

	//printf("numBlocks: (%d, %d)\n", numblocks.x, numblocks.y);
	//printf("sizeBlocks: (%d, %d)\n", sizeblocks.x, sizeblocks.y);

	// Setting wave spaces, saving states t, t - 1 and t - 2.
	float *waveSpace, *waveSpaceTMin1, *waveSpaceTMin2;
	waveSpace = (float *)malloc(N * N * sizeof(float));
	waveSpaceTMin1 = (float *)malloc(N * N * sizeof(float));
	waveSpaceTMin2 = (float *)malloc(N * N * sizeof(float));
	
	// Setting wave spaces in GPU.
	float *waveSpace_d, *waveSpaceTMin1_d, *waveSpaceTMin2_d;
	cudaMalloc((void**)&waveSpace_d, N * N * sizeof(float));
	cudaMalloc((void**)&waveSpaceTMin1_d, N * N * sizeof(float));
	cudaMalloc((void**)&waveSpaceTMin2_d, N * N * sizeof(float));

	float c = 1.0;
	float dt = 0.1;
	float dd = 2.0;
	//printf("T: %d\n", T);
	for (int step = 0; step <= T; step++)
	{
		//printf("step: %d\n", step);
		switch(step)
		{
			case 0:
				initializeSpace(N, waveSpace);
				memcpy(waveSpaceTMin1, waveSpace, N * N * sizeof(float));
				break;
			case 1:
				fillSpaceFirstStep(N, c, dt, dd, waveSpace, waveSpaceTMin1);
				memcpy(waveSpaceTMin2, waveSpaceTMin1, N * N * sizeof(float));
				memcpy(waveSpaceTMin1, waveSpace, N * N * sizeof(float));
				break;
			default:
				//printf("default\n");
				break;
		}
		
		if (step == 2)
		{
			//printf("step 2\n");
			break;
		}
	}

	// Copying values from CPU to GPU.
	cudaMemcpy(waveSpace_d, waveSpace, N * N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(waveSpaceTMin1_d, waveSpaceTMin1, N * N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(waveSpaceTMin2_d, waveSpaceTMin2, N * N * sizeof(float), cudaMemcpyHostToDevice);
	//printf("executing kernel...\n");
	// Executing kernel.
	schroedinger<<<numblocks,sizeblocks>>>(waveSpace_d, waveSpaceTMin1_d, waveSpaceTMin2_d, T, N, f, t);
	cudaDeviceSynchronize();
	
	if (T >= 2)
	{
		printf("T: %d\n", T);
		cudaMemcpy(waveSpace, waveSpace_d, N * N * sizeof(float), cudaMemcpyDeviceToHost);
	}

	setWaveSpace(N, f, waveSpace);

	double end = omp_get_wtime();
	
	printf("Time spent: %f\n", end - start);
	
	return 0;
}
