#include <stdio.h>
#include <pmmintrin.h>
#include <string.h>
#define TOTAL_SECUENCES 10
#define SUB_SECUENCES 4

float *getSecuences(char* filename)
{
	// TODO: Get array from file as integer.
	//static float secuences[TOTAL_SECUENCES] = {-5459555, -1419643, 9206201, 4325544, 7233019, -7826876, -5901765, 1576008, 6165429, -2737032};
	static float secuences[TOTAL_SECUENCES] = {1,2,3,4, 8,7,6,5, 30, 45};
	return secuences;
}

int min()
{
}

int max()
{}

int shuffle()
{}

int sortSecuencesAsc(int sec1, int sec2, int sec3, int sec4)
{
	min();
	max(); 
	shuffle();
}

__m128 bmn(__m128 subSec1, __m128 subSec2)
{
	// Get two secuences lenght 4.
	int i = 0;
	
	__m128 subSecMin, subSecMax, orderedSec1, orderedSec2;

	while (i < 3)
	{
		subSecMin = _mm_min_ps(subSec1, subSec2);
		subSecMax = _mm_max_ps(subSec1, subSec2);

		//if (i = 0)
		//{
			//subSec1 = _mm_set_ps(subSecMax[2], subSecMin[2], subSecMax[0], subSecMin[0]);
			//subSec2 = _mm_set_ps(subSecMax[3], subSecMin[3], subSecMax[1], subSecMin[1]);
			subSec1 = _mm_set_ps(subSecMax[2], subSecMin[2], subSecMax[0], subSecMin[0]);
			subSec2 = _mm_set_ps(subSecMax[3], subSecMin[3], subSecMax[1], subSecMin[1]);
		//}
		//else
		//{
		//}

		i++;
	}
	

	printf("orderedSec1: %f, %f, %f, %f\n", subSec1[0], subSec1[1], subSec1[2], subSec1[3]);
	printf("orderedSec2: %f, %f, %f, %f\n", subSec2[0], subSec2[1], subSec2[2], subSec2[3]);
	
}

int mergeSimd(int secuence)
{
	// Get a secuence lenght 16.  
}

int main()
{
	printf("LAB1: SIMD-SSE\n");

	char *filename = "numbers.raw";
	float *sec;

	sec = getSecuences(filename);

	__m128 A1, A2;
	float a1[4] __attribute__((aligned(16)));
	float a2[4] __attribute__((aligned(16)));

	memcpy(a1, (sec + 0), SUB_SECUENCES * 4);
	memcpy(a2, (sec + 4), SUB_SECUENCES * 4);

	A1 = _mm_load_ps(a1);
	A2 = _mm_load_ps(a2);

	__m128 Res = bmn(A1, A2);
	printf("A1: %f %f %f %f\n", A1[0], A1[1], A1[2], A1[3]);
	printf("A2: %f %f %f %f\n", A2[0], A2[1], A2[2], A2[3]);

	return 0;
}
