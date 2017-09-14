#include <stdio.h>
#include <pmmintrin.h>
#include <string.h>
#include <getopt.h>
#define SIZE_SECUENCES 16
#define SUB_SECUENCES 4

__m128 bmnSubSec1, bmnSubSec2;
__m128 inRegSubSec1, inRegSubSec2, inRegSubSec3, inRegSubSec4;


// BEGIN Implementing stack routines.
float *stack;
int *stackCount;

void push(float value, int secuence)
{
	stackCount[secuence]++;
	stack[SIZE_SECUENCES * secuence + stackCount[secuence]] = value;
}

float pop(int secuence)
{
	if (stackCount[secuence] == 0)
		return 0;
		
	float value = stack[SIZE_SECUENCES * secuence + stackCount[secuence]];
	stackCount[secuence]--;
	return value;
}

float peek(int secuence)
{
	if (stackCount[secuence] == 0)
		return 0;	
	return stack[SIZE_SECUENCES * secuence + stackCount[secuence]];
}

int isStackEmpty(int secuence)
{
	if (stackCount[secuence] == 0)
		return 1;
	else
		return 0;
}
// END Implementing stack routines.

// Writing ordered secuences in file.
void setSecuences(char *filename, int secsQty, float *secuencesp)
{
	FILE *filestream = fopen(filename, "w+b");
	
	if (!filestream)
	{
		printf("Couldn't open file.\n");
		return 1;
	}
	
	for (int i = 0; i < secsQty * SIZE_SECUENCES; i++)
	{
		fwrite(&secuencesp, sizeof(float) * secsQty * SIZE_SECUENCES, 1, filestream);
	}
	
	fclose(filestream);
}

// Getting secuences from file to order.
void getSecuences(char *filename, int secsQty, float *secuencesp)
{	
	FILE *filestream = fopen(filename, "r+b");
	float num[SIZE_SECUENCES * secsQty];

	if (!filestream)
	{
		printf("Couldn't open file.\n");
		return 1;
	}

	for (int i = 0; i < secsQty * SIZE_SECUENCES; i++)
	{
		fread(&num, sizeof(float) * secsQty * SIZE_SECUENCES, 1, filestream);
		secuencesp[i] = (float)num[i];
	}
	
	fclose(filestream);

}


// Transform sub secuences distribution, to arrange in a way properly to operate in SIMD routines.
__m128 arrangeBmnSubSecs(__m128 subSec1, __m128 subSec2, int subSecNumber)
{	
	__m128 arrangedSubSec1, arrangedSubSec2, arrangedSubSecFinal;
	
	if (subSecNumber == 1)
	{
		arrangedSubSec1 = _mm_shuffle_ps(subSec1, subSec2, _MM_SHUFFLE(0, 0, 0, 0));
		arrangedSubSec2 = _mm_shuffle_ps(subSec1, subSec2, _MM_SHUFFLE(1, 1, 1, 1));
	}
	else 
	{
		arrangedSubSec1 = _mm_shuffle_ps(subSec1, subSec2, _MM_SHUFFLE(0 + 2, 0 + 2, 0 + 2, 0 + 2));
		arrangedSubSec2 = _mm_shuffle_ps(subSec1, subSec2, _MM_SHUFFLE(1 + 2, 1 + 2, 1 + 2, 1 + 2));
	}
	
	arrangedSubSec1 = _mm_shuffle_ps(arrangedSubSec1, arrangedSubSec1, _MM_SHUFFLE(3, 1, 2, 0));
	
	arrangedSubSec2 = _mm_shuffle_ps(arrangedSubSec2, arrangedSubSec2, _MM_SHUFFLE(3, 1, 2, 0));
	
	arrangedSubSecFinal = _mm_shuffle_ps(arrangedSubSec1, arrangedSubSec2, _MM_SHUFFLE(3, 2, 1, 0));
	
	return arrangedSubSecFinal;
}

// Bitonic merge network.
void bmn(__m128 subSec1, __m128 subSec2)
{
	// Get two 4-lenght ascending ordered secuences.
	__m128 subSecMin, subSecMax;
	
	
	int i = 0;
	
	while (1)
	{
		subSecMin = _mm_min_ps(subSec1, subSec2);
		subSecMax = _mm_max_ps(subSec1, subSec2);
		
		if (i == 3)
			break;
	
		subSec1 = arrangeBmnSubSecs(subSecMin, subSecMax, 1);
		subSec2 = arrangeBmnSubSecs(subSecMin, subSecMax, 2);
		i++;
	}
	
	bmnSubSec1 = subSecMin;
	bmnSubSec2 = subSecMax;

}

// Traspose a matrix.
void transposeRegisterMatrix(__m128 subSec1, __m128 subSec2, __m128 subSec3, __m128 subSec4)
{
	__m128 arranged1 = arrangeBmnSubSecs(subSec1, subSec2, 1);
	__m128 arranged2 = arrangeBmnSubSecs(subSec3, subSec4, 1);
	__m128 arranged3 = arrangeBmnSubSecs(subSec1, subSec2, 2);
	__m128 arranged4 = arrangeBmnSubSecs(subSec3, subSec4, 2);
	
	inRegSubSec1 = _mm_shuffle_ps(arranged1, arranged2, _MM_SHUFFLE(1, 0, 1, 0));
	inRegSubSec2 = _mm_shuffle_ps(arranged1, arranged2, _MM_SHUFFLE(3, 2, 3, 2));
	inRegSubSec3 = _mm_shuffle_ps(arranged3, arranged4, _MM_SHUFFLE(1, 0, 1, 0));
	inRegSubSec4 = _mm_shuffle_ps(arranged3, arranged4, _MM_SHUFFLE(3, 2, 3, 2));
}

// Order two-pair registers, ascending.
void inRegisterSorting(__m128 subSec1, __m128 subSec2, __m128 subSec3, __m128 subSec4)
{
	__m128 subSecAux;
	
	subSecAux = _mm_min_ps(subSec1, subSec2);
	subSec2 = _mm_max_ps(subSec1, subSec2);
	subSec1 = subSecAux;
	
	subSecAux = _mm_min_ps(subSec3, subSec4);
	subSec4 = _mm_max_ps(subSec3, subSec4);
	subSec3 = subSecAux;
	
	subSecAux = _mm_min_ps(subSec1, subSec3);
	subSec3 = _mm_max_ps(subSec1, subSec3);
	subSec1 = subSecAux;
	
	subSecAux = _mm_min_ps(subSec2, subSec4);
	subSec4 = _mm_max_ps(subSec2, subSec4);
	subSec2 = subSecAux;
	
	subSecAux = _mm_min_ps(subSec1, subSec4);
	subSec4 = _mm_max_ps(subSec1, subSec4);
	subSec1 = subSecAux;
	
	subSecAux = _mm_min_ps(subSec2, subSec3);
	subSec3 = _mm_max_ps(subSec2, subSec3);
	subSec2 = subSecAux;

	transposeRegisterMatrix(subSec1, subSec2, subSec3, subSec4);
}

// Invert a sub secuence.
__m128 invertSubSec(__m128 subSec)
{
	return _mm_shuffle_ps(subSec, subSec, _MM_SHUFFLE(0, 1, 2, 3));
}

// Join two sub secuences to get one ordered secuence.
float *mergeSimd(__m128 subSec1, __m128 subSec2, __m128 subSec3, __m128 subSec4)
{
	// Get a secuence lenght 16.
	__m128 O1, O2;
	static float sortedSec[SIZE_SECUENCES];
	float sortedSubSec[4] __attribute__((aligned(16)));
	
	O1 = subSec1;
	O2 = subSec3;
	
	bmn(O1, invertSubSec(O2));
	
	_mm_store_ps(sortedSubSec, bmnSubSec1);

	memcpy(sortedSec, sortedSubSec, SUB_SECUENCES * 4);
	
	O1 = bmnSubSec2;
	O2 = subSec2;
	
	bmn(O1, invertSubSec(O2));
	
	O1 = bmnSubSec1;
	O2 = subSec4;
	
	bmn(O1, invertSubSec(O2));
	
	_mm_store_ps(sortedSubSec, bmnSubSec1);
	
	memcpy(sortedSec + 4, sortedSubSec, SUB_SECUENCES * 4);
	
	O1 = bmnSubSec2;
	O2 = subSec2;
	
	bmn(O1, invertSubSec(O2));
	
	_mm_store_ps(sortedSubSec, bmnSubSec1);
	memcpy(sortedSec + 8, sortedSubSec, SUB_SECUENCES * 4);
	
	_mm_store_ps(sortedSubSec, bmnSubSec2);
	memcpy(sortedSec + 12, sortedSubSec, SUB_SECUENCES * 4);

	return sortedSec;
}

float minValueIndexFound;

// Ask for number of secuence with minimun value 
// (among a collection of minimun value of secuences).
float GetMinValueIndexFromSecs(float secs[][2], int qty)
{
	const int secValue = 0;
	const int secNumber = 1;
	int aux;
	
	for (int i = 0; i < qty; i++)
		for (int j = i + 1; j < qty; j++)
		{
			if (secs[j][secValue] == 0) // End of list.
			{
				minValueIndexFound = secs[0][secNumber];
				return minValueIndexFound;
				
			}
		
			if (secs[i][secValue] > secs[j][secValue])
			{
				aux = secs[i][secValue];
				secs[i][secValue] = secs[j][secValue];
				secs[j][secValue] = aux;
				
				aux = secs[i][secNumber];
				secs[i][secNumber] = secs[j][secNumber];
				secs[j][secNumber] = aux;
			}
		}
	
	minValueIndexFound = secs[0][secNumber];
	return minValueIndexFound;
}

// Multiway Merge Sort.
// Order a collections of secuences, 
// invoques method to save secuences in a file
// and prints ordered secuences on screen.
float *mwms(int secsQty, char *outputFilename, int debug)
{

	const int secValue = 0;
	const int secNumber = 1;
	int j = 0, contListMinValues = 0;
	int minValueIndexFromSecs;
	
	float minValuesFromSecs[secsQty][2];
	float sortedNumbers[secsQty * SIZE_SECUENCES];

	while (j < secsQty * SIZE_SECUENCES)
	{
		for (int i = 0; i < secsQty; i++)
		{
			if (peek(i) == 0)
				continue;
			
			minValuesFromSecs[contListMinValues][secValue] = peek(i);
			minValuesFromSecs[contListMinValues][secNumber] = i;
			contListMinValues++;
		}
		
		minValueIndexFromSecs = GetMinValueIndexFromSecs(minValuesFromSecs, secsQty);
		minValueIndexFromSecs = minValueIndexFound;
		
		sortedNumbers[j] = pop(minValueIndexFromSecs);
		
		j++;
		contListMinValues = 0;
		
		// Reset list.
		for (int k = 0; k < secsQty; k++)
		{
			minValuesFromSecs[k][secValue] = 0;
			minValuesFromSecs[k][secNumber] = 0;
		}
	}
	
	if (debug == 1)
	{
		printf("final sorted nums:\n");
		for (int i = 0; i < secsQty * SIZE_SECUENCES; i++)
		printf("%f\n", sortedNumbers[i]);
	}
	
	setSecuences(outputFilename, secsQty, sortedNumbers);
}

int main(int argc, char **argv)
{
	char *inputFilename, *outputFilename;
	int selection, secsQty, debug;
	
	while ((selection = getopt(argc, argv, "i:o:N:d:")) != -1)
	{
		switch(selection) {
			case 'i':
				inputFilename = optarg;
				break;
			case 'o':
				outputFilename = optarg;
				break;
			case 'N':
				secsQty = atoi(optarg);
				break;
			case 'd':
				debug = atoi(optarg);
				break;
		}
	}
	
	stack = (float *)malloc(secsQty * SIZE_SECUENCES * sizeof(float));
	stackCount = (int *)malloc(secsQty * sizeof(int));
	
	float *secsSorted[2];

	float sec[SIZE_SECUENCES * secsQty];
	getSecuences(inputFilename, secsQty, &sec);

	__m128 A1, A2, A3, A4;
	float a1[4] __attribute__((aligned(16)));
	float a2[4] __attribute__((aligned(16)));
	float a3[4] __attribute__((aligned(16)));
	float a4[4] __attribute__((aligned(16)));

	for (int i = 0; i < secsQty; i++)
	{

		memcpy(a1, (sec + 0 + SIZE_SECUENCES * i), SUB_SECUENCES * 4);
		memcpy(a2, (sec + 4 + SIZE_SECUENCES * i), SUB_SECUENCES * 4);
		memcpy(a3, (sec + 8 + SIZE_SECUENCES * i), SUB_SECUENCES * 4);
		memcpy(a4, (sec + 12 + SIZE_SECUENCES * i), SUB_SECUENCES * 4);

		A1 = _mm_load_ps(a1);
		A2 = _mm_load_ps(a2);
		A3 = _mm_load_ps(a3);
		A4 = _mm_load_ps(a4);
		
		// 2.3. In-register sorting.
		inRegisterSorting(A1, A2, A3, A4);

		bmn(inRegSubSec1, invertSubSec(inRegSubSec2));
		inRegSubSec1 = bmnSubSec1;
		inRegSubSec2 = bmnSubSec2;
	
		bmn(inRegSubSec3, invertSubSec(inRegSubSec4));
		inRegSubSec3 = bmnSubSec1;
		inRegSubSec4 = bmnSubSec2;

		// 2.4. Merge SIMD.
		float *sortedSec;

		sortedSec = mergeSimd(inRegSubSec1, inRegSubSec2, inRegSubSec3, inRegSubSec4);

		for (int j = SIZE_SECUENCES - 1; j >= 0 ; j--)
		{
			push(*(sortedSec + j), i);
		}

	}

	// 2.5.Multiway merge sort (MWMS).
	float *sortedNumbers = mwms(secsQty, outputFilename, debug);

	return 0;
}
