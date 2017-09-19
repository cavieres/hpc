#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>

float *waveSpace, *waveSpaceTMin1, *waveSpaceTMin2;

int setWaveSpace(int iteration, int sizeGrid, char *filename)
{
	FILE *filestream = fopen(filename, "w+b");
	
	if (!filestream)
	{
		printf("Couldn't open file.\n");
		return 1;
	}
	
	for (int i = 0; i < sizeGrid; i++)
	{
		for (int j = 0; j < sizeGrid; j++)
		{
			//fwrite(&waveSpace, sizeof(float) * sizeGrid * sizeGrid, 1, filestream);
			printf("%f;", waveSpace[sizeGrid * i + j]);
		
		}
		printf("\n");
	}
	
	fclose(filestream);
}

void fillSpaceFirstStep(int N, float c, float dt, float dd)
{
	for (int i = 1; i < N; i++)
		for (int j = 1; j < N - 1; j++)
			waveSpace[N * i + j] = waveSpaceTMin1[N * i + j] + (c * c)/2 * (dt/dd * dt/dd) * (waveSpaceTMin1[N * (i + 1) + j] + waveSpaceTMin1[N * (i - 1) + j] + waveSpaceTMin1[N * i + (j - 1)] + waveSpaceTMin1[N * i + (j + 1)] - 4 * waveSpaceTMin1[N * i + j]);
}

void initializeSpace(int N)
{
	for (int i = 0.4 * N; i < 0.6 * N; i++)
		for (int j = 0.4 * N; j < 0.6 * N; j++)
			waveSpace[N * i + j] = 20;
}

void fillSpaceTSteps(int N, int T, float c, float dt, float dd)
{

	
	
}

int main(int argc, char **argv)
{
	int N, T, H, t;
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
				break;
			case 't':
				t = atoi(optarg);
				break;
		}
	}
	
	
	waveSpace = (float *)malloc(N * N * sizeof(float));
	waveSpaceTMin1 = (float *)malloc(N * N * sizeof(float));
	waveSpaceTMin2 = (float *)malloc(N * N * sizeof(float));
	
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
				break;
			default:
				fillSpaceTSteps(N, T, c, dt, dd);
				break;
		}
	
		if (step == t)
			setWaveSpace(t, N, f);
	}




	//fillSpace(N, T, H, F, t, c, dt, dd);

	//setWaveSpace(t, N, f);

	return 0;
}
