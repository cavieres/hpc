#include <stdio.h>
#include <pmmintrin.h>
#include <string.h>
#include <getopt.h>
//#include "stack-array.h"
#define SIZE_SECUENCES 16
#define SUB_SECUENCES 4

__m128 bmnSubSec1, bmnSubSec2;
__m128 inRegSubSec1, inRegSubSec2, inRegSubSec3, inRegSubSec4;

float *stack;
int *stackCount;

//////////////////////////////////////////////////////////////
void push(float value, int secuence)
{
	stackCount[secuence]++;
	//stack[secuence][stackCount[secuence]] = value;
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
//////////////////////////////////////////////////////////////

float *getSecuencesStatic()
{
	//static float secuences[SIZE_SECUENCES] = {-5459555, -1419643, 9206201, 4325544, 7233019, -7826876, -5901765, 1576008, 6165429, -2737032};
	//static float secuences[SIZE_SECUENCES] = {1,2,3,4, 8,7,6,5, 30, 45};
	//static float secuences[SIZE_SECUENCES] = {5,20,25,30, 18,9,7,2, 30, 45};
	//static float secuences[SIZE_SECUENCES] = {12,21,4,13, 9,8,6,7,1,14,3,0,5,11,15,10};
	
	// 32 float secuences.
	//static float secuences[SIZE_SECUENCES * 2] = {814.91,2.1506,905.89,2.8994,127.86,1.7234,913.46,3.3448,632.73,2.853,98.443,2.0071,279.22,3.8897,547.33,3.4277,957.55,3.0258,964.92,2.8684,158.46,1.1031,970.62,1.9998,957.21,1.5015,485.89,2.5329,800.48,2.4418,142.74,2.1809};

	//static float secuences[SIZE_SECUENCES * 2] = {814.91,2.1506,905.89,2.8994,127.86,1.7234,913.46,3.3448,632.73,2.853,98.443,2.0071,279.22,3.8897,547.33,3.4277,957.55,3.0258,964.92,2.8684,158.46,1.1031,970.62,1.9998,957.21,1.5015,485.89,2.5329,800.48,2.4418,142.74,2.1809};
	
	static float secuences[SIZE_SECUENCES * 2] = {814723.687500, 46171.390625, 547215.500000, 337122.656250, 181847.031250, 131973.296875, 817627.687500, 262211.750000, 98712.281250, 712694.500000, 291984.093750, 669175.312500, 425259.312500, 875371.625000, 473486.000000, 416158.593750, 192028.343750, 429921.406250, 678652.312500, 729751.875000, 123932.281250, 301306.062500, 208946.671875, 761425.875000, 890475.687500, 205674.515625, 272938.781250, 184433.671875, 422645.656250, 540884.062500, 317427.875000, 640116.562500};

	return secuences;
}

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
		printf("%f\n", secuencesp[i]);
		fwrite(&secuencesp, sizeof(float) * secsQty * SIZE_SECUENCES, 1, filestream);
	}
	
	fclose(filestream);
}

void getSecuences(char *filename, int secsQty, float *secuencesp)
{
	//float secuences[SIZE_SECUENCES * secsQty];
		
	FILE *filestream = fopen(filename, "r+b");
	float num[SIZE_SECUENCES * secsQty];
	//int num[SIZE_SECUENCES * secsQty];
	
	if (!filestream)
	{
		printf("Couldn't open file.\n");
		return 1;
	}
	

	//printf("secs desde archivo:\n");
	for (int i = 0; i < secsQty * SIZE_SECUENCES; i++)
	{
		fread(&num, sizeof(float) * secsQty * SIZE_SECUENCES, 1, filestream);
		//fread(&num, sizeof(int) * secsQty * SIZE_SECUENCES, 1, filestream);
		secuencesp[i] = (float)num[i];
		//printf("%f\n", num[i]);
	}
	
	fclose(filestream);

	//printf("\nparams:\n");
	//printf("%s\n", filename);
	//printf("%d\n\n", secsQty);
	
	//secuencesp = secuences;
	//return secuences;
}

/*void getSecuences2(char *nomAr, int nEntradas, float *data)
{
   int *cadena = (float*) malloc(nEntradas * 2 * sizeof(float));	// Depósito de los caracteres
   int leidos;

   // Apertura del fichero
   int fichero = open (nomAr, O_RDONLY);

   // Comprobación   
   if (fichero==-1){
        perror("Error al abrir fichero:");
        exit(1);
   }
   
  leidos = read(fichero, cadena, (nEntradas * 2 * sizeof(float)));
    
  //transformar de float a int
  for(int x=0;x<nEntradas * 2;x++){
    
    data[x] = (float) cadena[x];
  }
  
    close(fichero);
}*/

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
	
	/*printf("subSecMin: %f %f %f %f\n", subSecMin[0], subSecMin[1], subSecMin[2], subSecMin[3]);
	printf("subSecMax: %f %f %f %f\n", subSecMax[0], subSecMax[1], subSecMax[2], subSecMax[3]);*/
	
}

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
	
	/*printf("inRegSubSec1: %f %f %f %f\n", inRegSubSec1[0], inRegSubSec1[1], inRegSubSec1[2], inRegSubSec1[3]);
	printf("inRegSubSec2: %f %f %f %f\n", inRegSubSec2[0], inRegSubSec2[1], inRegSubSec2[2], inRegSubSec2[3]);
	printf("inRegSubSec3: %f %f %f %f\n", inRegSubSec3[0], inRegSubSec3[1], inRegSubSec3[2], inRegSubSec3[3]);
	printf("inRegSubSec4: %f %f %f %f\n", inRegSubSec4[0], inRegSubSec4[1], inRegSubSec4[2], inRegSubSec4[3]);*/
}

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

	//printf("subSec1: %f %f %f %f\n", subSec1[0], subSec1[1], subSec1[2], subSec1[3]);
	//printf("subSec2: %f %f %f %f\n", subSec2[0], subSec2[1], subSec2[2], subSec2[3]);
	//printf("subSec3: %f %f %f %f\n", subSec3[0], subSec3[1], subSec3[2], subSec3[3]);
	//printf("subSec4: %f %f %f %f\n", subSec4[0], subSec4[1], subSec4[2], subSec4[3]);
	
	transposeRegisterMatrix(subSec1, subSec2, subSec3, subSec4);
}

__m128 invertSubSec(__m128 subSec)
{
	return _mm_shuffle_ps(subSec, subSec, _MM_SHUFFLE(0, 1, 2, 3));
}

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
	
	//printf("sortedSec (mergeSimd): %f", sortedSec[0]);

	return sortedSec;
}

int areQuequesEmpty(int numQueues)
{
	for (int i = 0; i < numQueues; i++)
	{
		if (peek(i) != 0)
			return 0;
	}
	
	return 1;
}

float minValueIndexFound;
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
	
	// TODO: return local value instead of using global one.
	minValueIndexFound = secs[0][secNumber];
	return minValueIndexFound;
}

float *mwms(int secsQty, char *outputFilename)
{
	//int secsNum = secsQty;
	
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
		
		// TODO: Get value from returning method.
		minValueIndexFromSecs = GetMinValueIndexFromSecs(minValuesFromSecs, secsQty);
		minValueIndexFromSecs = minValueIndexFound;
		
		sortedNumbers[j] = pop(minValueIndexFromSecs);
		
		//printf("sortedNumbers[%d]: %f; indexFound: %d\n", j, sortedNumbers[j], minValueIndexFromSecs);
		
		j++;
		contListMinValues = 0;
		
		// Reset list.
		for (int k = 0; k < secsQty; k++)
		{
			minValuesFromSecs[k][secValue] = 0;
			minValuesFromSecs[k][secNumber] = 0;
		}
	}
	
	printf("final sorted nums:\n");
	//for (int i = 0; i < secsQty * SIZE_SECUENCES; i++)
	//	printf("%f\n", sortedNumbers[i]);
	
	setSecuences(outputFilename, secsQty, sortedNumbers);
	// TODO: Return value.
	//return sortedNumbers;
}

int main(int argc, char **argv)
{
	
	
	printf("LAB1: SIMD-SSE\n");
	
	char *inputFilename, *outputFilename;
	int selection, secsQty, debug;
	
	while ((selection = getopt(argc, argv, "i:o:N:d")) != -1)
	{
		switch(selection) {
			case 'i':
				inputFilename = optarg;
				break;
			case 'o':
				outputFilename = optarg;
			case 'N':
				secsQty = atoi(optarg);
				break;
			case 'd':
				debug = atoi(optarg);
		}
	}
	
	stack = (float *)malloc(secsQty * SIZE_SECUENCES * sizeof(float));
	stackCount = (int *)malloc(secsQty * sizeof(int));
	
	//int secsNum = 2;
	float *secsSorted[2];
	
	
	
	
	
	
	float sec[SIZE_SECUENCES * secsQty];
	getSecuences(inputFilename, secsQty, &sec);
	//getSecuences2(inputFilename, secsQty, &sec);

	//float *sec;
	//sec = getSecuencesStatic();

	/*printf("sec (main):\n");
	for (int j = 0; j < SIZE_SECUENCES * secsQty; j++)
		printf("%f, ", sec[j]);
	printf("\n\n");*/
	
	__m128 A1, A2, A3, A4;
	float a1[4] __attribute__((aligned(16)));
	float a2[4] __attribute__((aligned(16)));
	float a3[4] __attribute__((aligned(16)));
	float a4[4] __attribute__((aligned(16)));

	printf("Processing secuences...");

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
		
		//secsSorted[i] = sortedSec;

		//printf("sortedSec (%d):\n", i);
		//for (int j = 0; j < SIZE_SECUENCES; j++)
		//	printf("%f ", *(sortedSec + j));
		//printf("\n");

		for (int j = SIZE_SECUENCES - 1; j >= 0 ; j--)
		{
			//printf("push: %f\n", *(sortedSec + j));
			push(*(sortedSec + j), i);
			//printf("push: %f secNo: %d\n", *(sortedSec + j), i);
		}

		/*printf("sortedSec %d:", i);
		for (int j = 0; j < SIZE_SECUENCES; j++)
			printf(" %f", *(sortedSec + j));
		printf("\n");*/
	}

	// 2.5.Multiway merge sort (MWMS).
	float *sortedNumbers = mwms(secsQty, outputFilename);
	/*printf("final sorted nums:\n");
	for (int i = 0; i < secsQty * SIZE_SECUENCES - 1; i++)
		printf("%f, ", sortedNumbers[i]);*/




	
	/*printf("A1: %f %f %f %f\n", A1[0], A1[1], A1[2], A1[3]);
	printf("A2: %f %f %f %f\n", A2[0], A2[1], A2[2], A2[3]);
	printf("A3: %f %f %f %f\n", A3[0], A3[1], A3[2], A3[3]);
	printf("A4: %f %f %f %f\n", A4[0], A4[1], A4[2], A4[3]);*/
	
	
	/*printf("SortedSec1: %f %f %f %f %f %f %f %f\n", inRegSubSec1[0], inRegSubSec1[1], inRegSubSec1[2], inRegSubSec1[3], inRegSubSec2[0], inRegSubSec2[1], inRegSubSec2[2], inRegSubSec2[3]);
	printf("SortedSec2: %f %f %f %f %f %f %f %f\n", inRegSubSec3[0], inRegSubSec3[1], inRegSubSec3[2], inRegSubSec3[3], inRegSubSec4[0], inRegSubSec4[1], inRegSubSec4[2], inRegSubSec4[3]);*/
	
	return 0;
}
