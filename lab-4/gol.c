#include <stdlib.h>
#include<stdio.h>
#include <getopt.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <mpi.h>

//mpirun -np PROCESSORS ./gol -r NROWS -c NCOLS -i ITERATIONS -m PARTITIONING_METHOD -t NTHREADS -s SEED

void setSeeds(int SEED, int **Matrix, int NROWS_METHOD, int NCOLS_METHOD, int iniCol, int finCol);
void printValues (int **Matrix, int NROWS, int NCOLS);
void initialize(int **Matrix, int NROWS, int NCOLS);

int main(int argc, char *argv[]){

	//Clock
	double start_time = omp_get_wtime();
		
	MPI_Init(NULL, NULL);
	
	int nprocs;
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	
	int myrank;
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


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
	
	//Num HIlos
	omp_set_num_threads(NTHREADS);

	int **Matrix = (int **)malloc(NROWS*sizeof(int*));
	for(int i=0;i<NROWS;i++){
		Matrix[i] = (int*) malloc(NCOLS*sizeof(int));
	}
	
	initialize(Matrix, NROWS, NCOLS);

	
	int NROWS_METHOD = NROWS;
	int NCOLS_METHOD = NCOLS;
		
	if(PARTITIONING_METHOD == 0){
		//strip decomposition
		NCOLS_METHOD = NCOLS/nprocs;
		
	}else if(PARTITIONING_METHOD == 1){
		//block decomposition
		NROWS_METHOD = NROWS/nprocs;
		NCOLS_METHOD = NCOLS/nprocs;
		
	}
	
	//////////////////////////////////////
	

	
	if(myrank == 0){
		
		int iniCol = 0;
		
		for(int i=0; i < nprocs; i++){
			
			int finCol = NCOLS_METHOD +  finCol;
			printf("ini: %d fin: %d\n", iniCol, finCol);
			setSeeds(SEED, Matrix, NROWS_METHOD, NCOLS_METHOD, iniCol, finCol);
			printValues(Matrix, NROWS, NCOLS);
			
			iniCol = iniCol + NCOLS_METHOD;
			//iniCol = NCOLS_METHOD;
		}
		
	}
	
	MPI_Finalize();
	
	double time = omp_get_wtime() - start_time;
	printf("\nClock: %lf\n", time);
	
	return 0;
}

void initialize(int **Matrix, int NROWS, int NCOLS){
	
	for(int i=0;i<NROWS;i++){
		for(int j=0;j<NCOLS;j++){
			Matrix[i][j]=0;
		}
	}
	
}
	
void setSeeds(int SEED, int **Matrix, int NROWS_METHOD, int NCOLS_METHOD, int iniCol, int finCol){
	
	int numUnos = 0;
	int abort = 0;
	//TODO: mas unos de los requeridos (seed)
	#pragma omp parallel shared(numUnos) 
	#pragma omp for schedule(static, 1) 
	for(int i=0;i<NROWS_METHOD;i++){
		if (abort == 0) {
			for(int j=iniCol;j<finCol;j++){

				float value = (double)rand()/(double)RAND_MAX;

				if (value > 0.5){  
				  Matrix[i][j] = 1 ;
				  numUnos++;
				}else{
				  Matrix[i][j] = 0 ;
				}
				if(numUnos == SEED){
					abort = 1;
				}
			}
			if(numUnos == SEED){
					abort = 1;
			}
		}
	}
	//TODO: Validar que la cantidad semillas sea ingresada en la matriz
		
}


void printValues (int **Matrix, int NROWS, int NCOLS){
	
	for(int i=0;i<NROWS;i++){
		for(int j=0;j<NCOLS;j++){
 
			  printf("%d , ", Matrix[i][j]);

			}
		printf("\n");
		}
		
}	


void stripDecomposition(int **Matrix, int PROCESSORS,int NROWS, int NCOLS){
	
	for(int i=0;i<NROWS;i++){
		
		for(int x=0;x<(NCOLS/PROCESSORS);x++){
			
		}
	}
}