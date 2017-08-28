#include <stdio.h>

int* getSecuences(char* filename)
{
	static int secuences[2] = {30, 67};
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

	int* elements;
	elements = getSecuences(filename);

	int i = 0;

	for (i = 0; i < 2; i++)
		printf("%d\n", *(elements + i));

	return 0;
}
