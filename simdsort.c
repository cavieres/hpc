#include <stdio.h>
#define TOTAL_SECUENCES 4

int* getSecuences(char* filename)
{
	static int secuences[TOTAL_SECUENCES] = {-5459555, -1419643, 9206201, 4325544};
	return secuences;
}

int sortSecuenceAsc(int secuence)
{
	min();
	max(); 
}

int min()
{
}

int max()
{}

int shuffle()
{}

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
	//const int TOTAL_SECUENCES = 4;

	int* secuences;
	secuences = getSecuences(filename);

	int i = 0;

	for (int i = 0; i < TOTAL_SECUENCES; i++)
		printf("%d\n", *(secuences + i));

	return 0;
}
