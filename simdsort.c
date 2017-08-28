#include <stdio.h>
#include <pmmintrin.h>
#define TOTAL_SECUENCES 4

int* getSecuences(char* filename)
{
	static int secuences[TOTAL_SECUENCES] = {-5459555, -1419643, 9206201, 4325544};
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

int bmn(int sec1, int sec2)
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

	char* filename = "numbers.raw";
	int* secuences;
	secuences = getSecuences(filename);

	__m128i A, A1, A2;
	int a[4] __attribute__((aligned(16)));

	for (int i = 0; i < TOTAL_SECUENCES; i++)
	{
		printf("%d\n", *(secuences + i));
		a[i] = *(secuences + i);
	}

	return 0;
}
