#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include <omp.h>

int nthreads;

// 0, 1
double pi(double left, double right, float dx)
{
}

float getArea(float dx)
{
	float radio = 1;
	float area = 0;

	#pragma omp parallel num_threads(nthreads)
	#pragma omp for schedule(static, 4)
	for (int i = 0; i < 1; i = i + 1)
	{
		float x = dx;
		float y = sqrt(abs(radio - pow(x, 2)));
		area += x * y;
	}
	
	return area * 4;
}

int main(int argc, char *argv[])
{
	double dx = atof(argv[1]);
	printf("dx: %f\n", dx);
	
	nthreads = atoi(argv[2]);
	
	float res = getArea(dx);
	
	printf("area: %f\n", res);
	
	return 0;
}