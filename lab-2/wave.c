#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float *waveSpace;

void fillSpaceFirstIteration(int N, float c, float dt, float dd)
{
	for (int i = 1; i < N; i++)
		for (int j = 1; j < N - 1; j++)
			waveSpace[N * i + j] = waveSpace[N * i + j] + (c * c)/2 * (dt/dd * dt/dd) * (waveSpace[N * (i + 1) + j] + waveSpace[N * (i - 1) + j] + waveSpace[N * i + (j - 1)] + waveSpace[N * i + (j + 1)] * 4 * waveSpace[N * i + j]);
}

void fillSpaceInitialCondition(int N)
{
	for (int i = 0.4 * N; i < 0.6 * N; i++)
		for (int j = 0.4 * N; j < 0.6 * N; j++)
			waveSpace[N * i + j] = 20;
}

void initializeSpace(int N)
{
	for (int i = 0.6 * N; i < N; i++)
		for (int j = 0.6; j < N; j++)
			waveSpace[N * i + j] = 0;
}

int main(int argc, char* argv)
{
	int N = 100;
	float c = 1.0;
	float dt = 0.1;
	float dd = 2.0;
	
	/*waveSpace = (float *)malloc(N * sizeof(float));

	initializeSpace(N);

	fillSpaceInitialCondition(N);

	fillSpaceFirstIteration(N, c, dt, dd);*/

	return 0;
}
