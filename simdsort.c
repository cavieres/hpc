#include <stdio.h>
#include <pmmintrin.h>
#include <string.h>
#define TOTAL_SECUENCES 10
#define SUB_SECUENCES 4

float *getSecuences(char* filename)
{
	// TODO: Get array from file as integer.
	static float secuences[TOTAL_SECUENCES] = {-5459555, -1419643, 9206201, 4325544, 7233019, -7826876, -5901765, 1576008, 6165429, -2737032};

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
	// Get two secuences lenght 8.
	
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

	printf("a1: %f %f %f %f\n", a1[0], a1[1], a1[2], a1[3]);
	printf("a2: %f %f %f %f\n", a2[0], a2[1], a2[2], a2[3]);

	return 0;
}
