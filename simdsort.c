#include <stdio.h>
#include <pmmintrin.h>
#include <string.h>
#define TOTAL_SECUENCES 10
#define SUB_SECUENCES 4

__m128 orderedSubSec1, orderedSubSec2;

float *getSecuences(char *filename)
{
	//static float secuences[TOTAL_SECUENCES] = {-5459555, -1419643, 9206201, 4325544, 7233019, -7826876, -5901765, 1576008, 6165429, -2737032};
	static float secuences[TOTAL_SECUENCES] = {1,2,3,4, 8,7,6,5, 30, 45};
	//static float secuences[TOTAL_SECUENCES] = {5,20,25,30, 18,9,7,2, 30, 45};
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

__m128 arrangeBmnSubSecs(__m128 subSec1, __m128 subSec2, int subSecNumber)
{	
	if (subSecNumber == 1)
		subSecNumber = 0;
		
	__m128 arrangedSubSec1, arrangedSubSec2, arrangedSubSecFinal;
	
	arrangedSubSec1 = _mm_shuffle_ps(subSec1, subSec2, _MM_SHUFFLE(0, 0, 0, 0));
	arrangedSubSec1 = _mm_shuffle_ps(arrangedSubSec1, arrangedSubSec1, _MM_SHUFFLE(3, 1, 2, 0));
	
	arrangedSubSec2 = _mm_shuffle_ps(subSec1, subSec2, _MM_SHUFFLE(1, 1, 1, 1));
	arrangedSubSec2 = _mm_shuffle_ps(arrangedSubSec2, arrangedSubSec2, _MM_SHUFFLE(3, 1, 2, 0));
	
	arrangedSubSecFinal = _mm_shuffle_ps(arrangedSubSec1, arrangedSubSec2, _MM_SHUFFLE(3, 2, 1, 0));
	
	return arrangedSubSecFinal;
}

void bmn(__m128 subSec1, __m128 subSec2)
{
	// Get two 4-lenght ascending ordered secuences.
	__m128 subSecMin, subSecMax;
	
	//for (int i = 0; i < 3; i++)
	//{
	subSecMin = _mm_min_ps(subSec1, subSec2);
	subSecMax = _mm_max_ps(subSec1, subSec2);
	
	subSec1 = arrangeBmnSubSecs(subSecMin, subSecMax, 1);
	subSec2 = arrangeBmnSubSecs(subSecMin, subSecMax, 2);
	//}
	
	printf("subSec1: %f %f %f %f\n", subSec1[0], subSec1[1], subSec1[2], subSec1[3]);
	printf("subSec2: %f %f %f %f\n", subSec2[0], subSec2[1], subSec2[2], subSec2[3]);
	
	orderedSubSec1 = subSecMin;
	
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

	//__m128 *res = bmn(A1, A2);
	bmn(A1, A2);
	
	printf("A1: %f %f %f %f\n", A1[0], A1[1], A1[2], A1[3]);
	printf("A2: %f %f %f %f\n", A2[0], A2[1], A2[2], A2[3]);
	printf("orderedSubSec1: %f %f %f %f\n", orderedSubSec1[0], orderedSubSec1[1], orderedSubSec1[2], orderedSubSec1[3]);
	
	return 0;
}
