#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include <omp.h>

float *waveSpace, *waveSpaceTMin1, *waveSpaceTMin2;
int H;

// Save wave state as binary file.
int setWaveSpace(int N, char *f)
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
	}
	
	fclose(filestream);
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
}

// Filling wave space with T = 1.
void fillSpaceFirstStep(int N, float c, float dt, float dd)
{
	#pragma omp parallel num_threads(H)
	#pragma omp for schedule(static, 4)
	for (int i = 1; i < N; i++)
		for (int j = 1; j < N - 1; j++)
			waveSpace[N * i + j] = waveSpaceTMin1[N * i + j] + (c * c)/2 * (dt/dd * dt/dd) * (waveSpaceTMin1[N * (i + 1) + j] + waveSpaceTMin1[N * (i - 1) + j] + waveSpaceTMin1[N * i + (j - 1)] + waveSpaceTMin1[N * i + (j + 1)] - 4 * waveSpaceTMin1[N * i + j]);
}

// Setting wave space as a grid with values "20" in center
// sorrounding with zero values.
void initializeSpace(int N)
{
	for (int i = 0; i < N * N; i++)
		waveSpace[i] = 0;
	
	for (int i = 0.4 * N; i < 0.6 * N; i++)
		for (int j = 0.4 * N; j < 0.6 * N; j++)
			waveSpace[N * i + j] = 20;
}

// Filling wave space with T > 1.
void fillSpaceTSteps(int N, int T, float c, float dt, float dd)
{
	#pragma omp parallel num_threads(H)
	#pragma omp for schedule(static, 4)
	for (int i = 1; i < N; i++)
		for (int j = 1; j < N - 1; j++)
			waveSpace[N * i + j] = 2 * waveSpaceTMin1[N * i + j] - waveSpaceTMin2[N * i + j] + (c * c) * (dt/dd * dt/dd) * (waveSpaceTMin1[N * (i + 1) + j] + waveSpaceTMin1[N * (i - 1) + j] + waveSpaceTMin1[N * i + (j - 1)] + waveSpaceTMin1[N * i + (j + 1)] - 4 * waveSpaceTMin1[N * i + j]);
}

int main(int argc, char **argv)
{
	double start = omp_get_wtime();
	
	int N, T, t;
	float c = 1.0;
	float dt = 0.1;
	float dd = 2.0;
	
	
	int selection;
	char *f;
	
	while ((selection = getopt(argc, argv, "N:T:H:f:t:")) != -1)
	{
		switch(selection) {
			case 'N':
				N = atoi(optarg);
				break;
			case 'T':
				T = atoi(optarg);
				break;
			case 'H':
				H = atoi(optarg);
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
	
	// Setting wave spaces, saving states t, t - 1 and t - 2.
	waveSpace = (float *)malloc(N * N * sizeof(float));
	waveSpaceTMin1 = (float *)malloc(N * N * sizeof(float));
	waveSpaceTMin2 = (float *)malloc(N * N * sizeof(float));
	
	// Schroedinger ecuation, by a given step as input.
	for (int step = 0; step <= T; step++)
	{
	
		switch(step)
		{
			case 0:
				initializeSpace(N);
				memcpy(waveSpaceTMin1, waveSpace, N * N * sizeof(float));
				break;
			case 1:
				fillSpaceFirstStep(N, c, dt, dd);
				memcpy(waveSpaceTMin2, waveSpaceTMin1, N * N * sizeof(float));
				memcpy(waveSpaceTMin1, waveSpace, N * N * sizeof(float));
				break;
			default:
				fillSpaceTSteps(N, T, c, dt, dd);
				memcpy(waveSpaceTMin2, waveSpaceTMin1, N * N * sizeof(float));
				memcpy(waveSpaceTMin1, waveSpace, N * N * sizeof(float));
				break;
		}
	
		// Save step image specified by parameter t.
		if (step == t)
			setWaveSpace(N, f);
	}
	
	double end = omp_get_wtime();
	
	printf("Time spent: %f\n", end - start);
	
	return 0;
}
