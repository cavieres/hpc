#include <stdio.h>
#include <emmintrin.h>

int main()
{
	// Using four-number secuences.
	
	__m128 A, B, Cmin, Cmax; // 4 x 32 bits floats.
	float a[4] __attribute__((aligned(16))) = {6, 45, 9, 5};
	float b[4] __attribute((aligned(16))) = {68, 67, 0, 2};
	
	A = _mm_load_ps(a);
	B = _mm_load_ps(b);
	
	Cmin = _mm_min_ps(A, B);
	Cmax = _mm_max_ps(A, B);
	
	printf("cMin: %f, %f, %f, %f\n", Cmin[0], Cmin[1], Cmin[2], Cmin[3]);
	printf("cMax: %f, %f, %f, %f\n", Cmax[0], Cmax[1], Cmax[2], Cmax[3]);
	
	
	// Using two-number secuences.
	
	__m128 D, E, Fmin, Fmax; // 4 x 32 bits floats.
	
	float d[2] __attribute__((aligned(16))) = {a[0], a[1]};
	float e[2] __attribute__((aligned(16))) = {b[0], b[1]};
	
	D = _mm_load_ps(d);
	E = _mm_load_ps(e);
	
	Fmin = _mm_min_ps(D, E);
	Fmax = _mm_max_ps(D, E);
	
	printf("Fmin: %f, %f\n", Fmin[0], Fmin[1]);
	printf("Fmax: %f, %f\n", Fmax[0], Fmax[1]);
	
	return 0;
}
