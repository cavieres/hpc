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
void setLifeAndDead(int *Matrix, int NROWS, int NCOLS, int iniCol, int finCol);
int num_neighbours(int *Matrix, int x, int y, int NROWS, int NCOLS);

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
	
	
	MPI_Status status;

	int *Matrix = (int *)malloc(sizeof(int)*NROWS*NCOLS);

	MPI_Datatype coltypeAll;
	MPI_Datatype coltypeReturn;
	
	MPI_Init(NULL, NULL);
	
	int nprocs;
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	
	int NROWS_METHOD = NROWS;
	int NCOLS_METHOD = NCOLS;
	
	const int GHOST_COLS_MIDDLE = 2;
	const int GHOST_COLS_RIGHT = 1;
		
	if(PARTITIONING_METHOD == 0){
		//strip decomposition
		NCOLS_METHOD = NCOLS/nprocs;
		
	}else if(PARTITIONING_METHOD == 1){
		//block decomposition
		NROWS_METHOD = NROWS/nprocs;
		NCOLS_METHOD = NCOLS/nprocs;
		
	}
	
	// Getting total of cores utilizing.
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	// Setting size of each strip column to send to each core.
	MPI_Type_vector(NROWS, NCOLS, NROWS, MPI_INT, &coltypeAll);
	MPI_Type_vector(NROWS, NCOLS_METHOD, NCOLS, MPI_INT, &coltypeReturn);

	MPI_Type_commit(&coltypeAll);
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
			
			iniCol = iniCol + NCOLS_METHOD;
		}
		//TODO: ENviar partes de matriz
		
		printValues(Matrix, NROWS, NCOLS);
		
		
		for(int rankEnvio = 1; rankEnvio < nprocs; rankEnvio++){
			
			if (rankEnvio == nprocs - 1){
				MPI_Send(&Matrix[0],1,coltypeAll,rankEnvio,rankEnvio,MPI_COMM_WORLD);
			} else {
				
				MPI_Send(&Matrix[0],1,coltypeAll,rankEnvio,rankEnvio,MPI_COMM_WORLD);
			}
			
		}
		
		
		
		
		
		////////////////////////PROCESO
		
		//TODO: Barrera y unificar resultados 
		
		int iniCol2 = NCOLS_METHOD; // Discounting master's work.
		
		for(int rankEnvio=1; rankEnvio < nprocs; rankEnvio++){

			MPI_Recv(&Matrix[iniCol2], 1, coltypeReturn, rankEnvio, rankEnvio, MPI_COMM_WORLD, &status);
			//printf("iniCol2: %d\n", iniCol2);
			iniCol2 += NCOLS_METHOD;
			
			printf("rank = %d; Result:\n", rankEnvio);
			printValues(Matrix, NROWS, NCOLS);
		}
		
		// Work in first strip associated to pid = 0.
		//setLifeAndDead(Matrix, NROWS, NCOLS, 0, NCOLS_METHOD);
		/*for(int i=0;i<NROWS;i++){
			for(int j=0;j<2;j++){
			Matrix[i*NCOLS+j] = 99;

			}
			
		}*/
		
		

	}
	else {
	
		if (rank == nprocs - 1)
			MPI_Recv(&Matrix[0], 1, coltypeAll, 0, rank, MPI_COMM_WORLD, &status);
		else
			MPI_Recv(&Matrix[0], 1, coltypeAll, 0, rank, MPI_COMM_WORLD, &status);
		
		//initialize(Matrix, NROWS, NCOLS);
		
		//printf("rank: %d\n", rank);
		/*for(int i=0;i<NROWS;i++){
			Matrix[i*NCOLS+0] = rank;
			Matrix[i*NCOLS+1] = rank;
			Matrix[i*NCOLS+2] = rank;
			Matrix[i*NCOLS+3] = rank;
			Matrix[i*NCOLS+4] = rank;
			Matrix[i*NCOLS+5] = rank;
			Matrix[i*NCOLS+6] = rank;
			Matrix[i*NCOLS+7] = rank;
			
		}*/
		int inicio = NCOLS_METHOD * rank;
			
		int fin = inicio + NCOLS_METHOD;
			
				printf("rank = %d;Rango a Modificar = (%d, %d); Result:\n", rank, inicio, fin);
		printValues(Matrix, NROWS, NCOLS);
		setLifeAndDead(Matrix, NROWS, NCOLS, inicio, fin);
		
				MPI_Send(&Matrix[rank*NCOLS_METHOD], 1, coltypeReturn, 0, rank, MPI_COMM_WORLD);
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
	#pragma omp parallel shared(numUnos) 
	#pragma omp for schedule(static, 1) 
	for(int i=0;i<NROWS_METHOD;i++){
		if (abort == 0) {
			for(int j=iniCol;j<finCol;j++){
				if (abort == 0) {

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

void setLifeAndDead(int *Matrix, int NROWS, int NCOLS, int iniCol, int finCol) {

	int *temp = (int *)malloc(sizeof(int)*NROWS*NCOLS);	
	
    int neighbours = 0;

    for(int fila=0;fila<NROWS;fila++){
		for(int columna=iniCol;columna<=finCol;columna++){
            neighbours = num_neighbours(Matrix, fila, columna, NROWS, NCOLS);
            if (neighbours < 2 && Matrix[fila*NCOLS + columna] == 1) {
                temp[fila*NCOLS + columna] = 0;
            } else if (neighbours > 3 && Matrix[fila*NCOLS + columna] == 1) {
                temp[fila*NCOLS + columna] = 0; 
            } else if (neighbours == 3 && Matrix[fila*NCOLS + columna] == 0) {
                temp[fila*NCOLS + columna] = 1; 
            }
        }
    }
	
    for(int fila=0;fila<NROWS;fila++){
		for(int columna=iniCol;columna<=finCol;columna++){
            Matrix[fila*NCOLS + columna] = temp[fila*NCOLS + columna];
        }
    }
}


int num_neighbours(int *Matrix, int x, int y, int NROWS, int NCOLS) {

    int num_adj = 0;
    int tmpy = y;
    int tmpx = x;
    
    if (y-1 < 0)
        tmpy = NROWS - 1;
    else
        tmpy = y - 1;
    if (Matrix[x*NCOLS + tmpy] == 1) num_adj++;
    if (y+1 >= NROWS)
        tmpy = 0;
    else
        tmpy = y + 1;
    if (Matrix[x*NCOLS + tmpy] == 1) num_adj++;
    if (x-1 < 0)
        tmpx = NCOLS - 1;
    else
        tmpx = x - 1;
    if (Matrix[tmpx*NCOLS + y] == 1) num_adj++;
    if (x+1 >= NCOLS)
        tmpx = 0;
    else
        tmpx = x + 1;
    if (Matrix[tmpx*NCOLS + y] == 1) num_adj++;
    if (y-1 < 0)
        tmpy = NROWS - 1;
    else
        tmpy = y - 1;
    if (x-1 < 0)
        tmpx = NCOLS - 1;
    else
        tmpx = x - 1;
    if (Matrix[tmpx*NCOLS + tmpy] == 1) num_adj++;
    if (x+1 >= NCOLS)
        tmpx = 0;
    else
        tmpx = x + 1;
    if (Matrix[tmpx*NCOLS + tmpy] == 1) num_adj++;
    if (y+1 >= NROWS)
        tmpy = 0;
    else
        tmpy = y + 1;
    if (x+1 >= NCOLS)
        tmpx = 0;
    else
        tmpx = x + 1;
    if (Matrix[tmpx*NCOLS + tmpy] == 1) num_adj++;
    if (x-1 < 0)
        tmpx = NCOLS - 1;
    else
        tmpx = x - 1;
    if (Matrix[tmpx*NCOLS + tmpy] == 1) num_adj++;
    return num_adj;
}
