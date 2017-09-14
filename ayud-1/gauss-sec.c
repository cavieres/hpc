#include <stdio.h>
#include <pmmintrin.h>
#include <math.h>


float gauss(float sigma, int x, int y)
{
	float g = exp( (-1 * (pow(x, 2) + pow(y, 2))) / (2 * pow(sigma, 2)));

	return g;
}


void llenarMatrizGauss()
{
	int m = 512;
	int n = 512;
	float sigma = 1;

	float matrizGauss[m][n];

	float sumMatrizGauss;

	for (int i = 0; i < m; i++)
	{
		int y = i - m/2;

		for (int j = 0; j < n; j++)
		{
			int x = j - n/2;

			matrizGauss[i][j] = gauss(sigma, x, y);

			printf("matrizGauss[%d][%d]: %f\n", i, j, matrizGauss[i][j]);

			sumMatrizGauss += matrizGauss[i][j];
		}
	}

	// Normalizar matriz gauss.
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			matrizGauss[i][j] = matrizGauss[i][j] / sumMatrizGauss;
}

void main()
{
	char *filename = "hltau_512.raw";
	int numsQty = 512;
	float nums[numsQty];

	FILE *filestream = fopen(filename, "r+b");
	
	if (!filestream)
	{
		printf("Error abriendo archivo.");
	}

	for (int i = 0; i < numsQty; i++)
	{
		fread(&nums, sizeof(float) * numsQty, 1, filestream);
		//printf("nums[%d]: %f\n", i, nums[i]);
	}
	
	fclose(filestream);

	llenarMatrizGauss();
}

