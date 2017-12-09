#include <stdlib.h>
#include<stdio.h>
#include <getopt.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <mpi.h>

//mpirun -np PROCESSORS ./gol -r NROWS -c NCOLS -i ITERATIONS -m PARTITIONING_METHOD -t NTHREADS -s SEED

void setSeeds(int SEED, int *Matrix, int NROWS_METHOD, int NCOLS_METHOD, int iniCol, int finCol, int NCOLS);
void printValues (int *Matrix, int NROWS, int NCOLS);
void initialize(int *Matrix, int NROWS, int NCOLS);

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
	
	int rank;
	MPI_Status status;
	//int **Matrix = (int **)malloc(NROWS*sizeof(int*));
	//for(int i=0;i<NROWS;i++){
	//	Matrix[i] = (int*) malloc(NCOLS*sizeof(int));
	//}
	int *Matrix = (int *)malloc(sizeof(int)*NROWS*NCOLS);

	MPI_Datatype coltypeMiddle;
	MPI_Datatype coltypeRight;
	MPI_Datatype coltypeReturn;
	MPI_Init(NULL, NULL);
	
	int nprocs;
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	
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
	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	//MPI_Type_vector(NROWS,NCOLS_METHOD,NCOLS,MPI_INT,&coltype);
	MPI_Type_vector(NROWS, 4, NCOLS, MPI_INT, &coltypeMiddle);
	MPI_Type_commit(&coltypeMiddle);
	MPI_Type_vector(NROWS, NCOLS + 1, NROWS, MPI_INT, &coltypeRight);
	MPI_Type_commit(&coltypeRight);
	MPI_Type_vector(NROWS, 3, NCOLS, MPI_INT, &coltypeReturn);
	MPI_Type_commit(&coltypeReturn);
	
	
	
	
	//Num HIlos
	omp_set_num_threads(NTHREADS);
	
	
	
	initialize(Matrix, NROWS, NCOLS);

	
	
	
	//////////////////////////////////////

	
	if(rank == 0){
		
		int iniCol = 0;
		
		for(int i=0; i < nprocs; i++){
			
			int finCol = NCOLS_METHOD +  finCol;
			printf("ini: %d fin: %d\n", iniCol, finCol);
			setSeeds(SEED, Matrix, NROWS_METHOD, NCOLS_METHOD, iniCol, finCol,NCOLS);
			
			//BORRAR//BORRAR//BORRAR//BORRAR//BORRAR//BORRAR//BORRAR
			/*int m=0;
			for(int i=0;i<NROWS;i++){
				for(int j=0;j<NCOLS;j++){
					Matrix[i*NCOLS + j] = m;
					m++;
				}
			}*/
			//BORRAR//BORRAR//BORRAR//BORRAR//BORRAR//BORRAR//BORRAR
			
			iniCol = iniCol + NCOLS_METHOD;
			//iniCol = NCOLS_METHOD;
		}
		//TODO: ENviar partes de matriz
		
		printValues(Matrix, NROWS, NCOLS);
		
		
		for(int rankEnvio=1; rankEnvio < nprocs; rankEnvio++){
			
			if (rankEnvio == nprocs - 1){
				
				MPI_Send(&Matrix[0],1,coltypeRight,rankEnvio,rankEnvio,MPI_COMM_WORLD);
			} else {
				
				MPI_Send(&Matrix[0],1,coltypeMiddle,rankEnvio,rankEnvio,MPI_COMM_WORLD);
			}
			
		}
		
		setLifeAndDead();
		////////////////////////PROCESO
		
		//TODO: Barrera y unificar resultados 
		
		
		for(int rankEnvio=1; rankEnvio < nprocs; rankEnvio++){
			
			if (rankEnvio == nprocs - 1){
				MPI_Recv(&Matrix[6], 1, coltypeReturn, rankEnvio, rankEnvio, MPI_COMM_WORLD, &status);
			}else{
				MPI_Recv(&Matrix[2], 1, coltypeReturn, rankEnvio, rankEnvio, MPI_COMM_WORLD, &status);
			}
		}
		
		printValues(Matrix, NROWS, NCOLS);
	}else if (rank == nprocs - 1){
		

		MPI_Recv(&Matrix[0], 1, coltypeRight, 0, rank, MPI_COMM_WORLD, &status);
		/*
		else {
			MPI_Type_commit(&coltypeMiddle);
			MPI_Recv(Matrix[0][rank * 2 - 1], 1, coltypeMiddle, 0, rank, MPI_COMM_WORLD, &status);
		}
		
		printf("rec rank %d", rank);
		printValues(Matrix, NROWS, NCOLS);*/
		for(int i=0;i<NROWS;i++){
			Matrix[i*NCOLS+4] = 444;
			Matrix[i*NCOLS+5] = 777;
			Matrix[i*NCOLS+6] = 888;
			Matrix[i*NCOLS+7] = 111;
			
		}
		//printValues(Matrix, NROWS, NCOLS);
		
		MPI_Send(&Matrix[6], 1, coltypeReturn, 0, rank, MPI_COMM_WORLD);
	}else if (rank != nprocs - 1){
		
		MPI_Recv(&Matrix[0], 1, coltypeMiddle, 0, rank, MPI_COMM_WORLD, &status);
		
		for(int i=0;i<NROWS;i++){
			Matrix[i*NCOLS+2] = 222;
			Matrix[i*NCOLS+3] = 333;
			Matrix[i*NCOLS+4] = 444;
			Matrix[i*NCOLS+5] = 555;
			
		}
		
		MPI_Send(&Matrix[2], 1, coltypeReturn, 0, rank, MPI_COMM_WORLD);
		
		
	}
	
	MPI_Finalize();
	
	double time = omp_get_wtime() - start_time;
	printf("\nClock: %lf\n", time);
	
	return 0;
}

void initialize(int *Matrix, int NROWS, int NCOLS){
	
	for(int i=0;i<NROWS;i++){
		for(int j=0;j<NCOLS;j++){
			Matrix[i*NCOLS + j]=0;
		}
	}
	
}
	
void setSeeds(int SEED, int *Matrix, int NROWS_METHOD, int NCOLS_METHOD, int iniCol, int finCol, int NCOLS){
	
	int numUnos = 0;
	int abort = 0;
	//TODO: mas unos de los requeridos (seed)
	//#pragma omp parallel shared(numUnos) 
	//#pragma omp for schedule(static, 1) 
	for(int i=0;i<NROWS_METHOD;i++){
		if (abort == 0) {
			for(int j=iniCol;j<finCol;j++){

				float value = (double)rand()/(double)RAND_MAX;

				if (value > 0.5){  
				  Matrix[i*NCOLS + j] = 1 ;
				  numUnos++;
				}else{
				  Matrix[i*NCOLS + j] = 0 ;
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


void printValues (int *Matrix, int NROWS, int NCOLS){
	
	for(int i=0;i<NROWS;i++){
		for(int j=0;j<NCOLS;j++){
 
			  printf("%d , ", Matrix[i*NCOLS+j]);

			}
		printf("\n");
		}
		
}	

void setLifeAndDead(int *Matrix, int NROWS, int NCOLS, int iniCol, int finCol){
	
	for(int fila=0;fila<NROWS;fila++){
		
			for(int columna=iniCol;columna<=finCol;columna++){
				//Primera fila y primera columna
				if(fila==0 && columna == 0){
					
					if(Matrix[fila*NCOLS + columna] == 0){
						
					}
				}
		
			}	
	}
}
