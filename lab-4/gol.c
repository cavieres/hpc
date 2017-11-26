#include <stdlib.h>
#include<stdio.h>
#include <getopt.h>
#ifdef _OPENMP
#include <omp.h>
#endif

//mpirun -np PROCESSORS ./gol -r NROWS -c NCOLS -i ITERATIONS -m PARTITIONING_METHOD -t NTHREADS -s SEED

int main(int argc, char *argv[]){

	//Clock
	double start_time = omp_get_wtime();
	
	int NROWS;
	int NCOLS;
	int ITERATIONS;
	int PARTITIONING_METHOD;
	int NTHREADS;
	int SEED;
	
	int selection;
	
	while ((selection = getopt(argc, argv, "r:c:i:m:t:s:")) != -1)
	{
		switch(selection) {
			case 'r':
				NROWS = atoi(optarg);
				break;
			case 'c':
				NCOLS = atoi(optarg);
				break;
			case 'i':
				ITERATIONS = atoi(optarg);
				break;
			case 'm':
				PARTITIONING_METHOD = atoi(optarg);
				break;
			case 't':
				NTHREADS = atoi(optarg);
				break;
			case 's':
				SEED = atoi(optarg);
				break;
		}
	}

	int **Matrix = (int **)malloc(NROWS*sizeof(int*));
	for(int i=0;i<NROWS;i++){
		Matrix[i] = (int*) malloc(NCOLS*sizeof(int));
	}
	
	//Num HIlos
	omp_set_num_threads(NTHREADS);
	
	if(PARTITIONING_METHOD == 0){
		//strip decomposition
		
	}else if(PARTITIONING_METHOD == 1){
		//block decomposition
		
	}
	
	
	double time = omp_get_wtime() - start_time;
	printf("\nClock: %lf\n", time); 
}

void stripDecomposition(int **Matrix, int PROCESSORS,int NROWS, int NCOLS){
	
	for(int i=0;i<NROWS;i++){
		
		for(int x=0;x<(NCOLS/PROCESSORS);x++){
			
		}
	}
}