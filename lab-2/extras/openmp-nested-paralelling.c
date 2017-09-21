#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif


void print_num_threads(int level, int threadNum) {
	#pragma omp single
		printf("Nivel %d: %d threads - thread num: %d\n", level, omp_get_num_threads(), threadNum);
}

void main() {
	omp_set_nested(1);
	#pragma omp parallel num_threads(2)
	{
		print_num_threads(1, omp_get_thread_num());
		#pragma omp parallel num_threads(3)
		{
			print_num_threads(2, omp_get_thread_num());
			#pragma omp parallel num_threads(2)
			{
				print_num_threads(3, omp_get_thread_num());
			}
		}
	}
}