#include <stdio.h>
#include <emmintrin.h>

int main()
{
	__m128i A, B, Cmin, Cmax, shB; // 4 x 32 bits ints.
	int a[4] __attribute__((aligned(16))) = {6, 45, 9, 5};
	int b[4] __attribute((aligned(16))) = {68, 67, 0, 2};
	
	A = _mm_load_si128(a);
	B = _mm_load_si128(b);
	
	//shB = _mm_shuffle_epi32(B, _MM_SHUFFLE(3,2,0,0));
	//Cmin = _mm_min_epi16(A, shB);
	
	Cmin = _mm_min_epi16(A, B);
	Cmax = _mm_max_epi16(A, B);
	
	printf("a: %d, %d, %d, %d\n", a[0], a[1], a[2], a[3]);
	printf("b: %d, %d, %d, %d\n", b[0], b[1], b[2], b[3]);
	
	//printf("shB: %d, %d, %d, %d\n", shB[0], shB[1], shB[2], shB[3]);
	
	printf("cMin: %d, %d, %d, %d\n", Cmin[0], Cmin[1], Cmin[2], Cmin[3]);
	printf("cMax: %d, %d, %d, %d\n", Cmax[0], Cmax[1], Cmax[2], Cmax[3]);
	
	return 0;
}
