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
int getNeighbors(int *Matrix, int x, int y, int NROWS, int NCOLS);

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

	int *Matrix = (int *)malloc(sizeof(int)*NROWS*NCOLS); // Tablero con poblacion.

	MPI_Datatype coltypeAll;
	MPI_Datatype coltypeReturn;
	
	MPI_Init(NULL, NULL); // Inicializar trabajo en MPI.
	
	// Obtener cantidad de procesadores en ejecucion.
	int nprocs;
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	
	int NROWS_METHOD = NROWS;
	int NCOLS_METHOD = NCOLS;
	
	const int GHOST_COLS_MIDDLE = 2;
	const int GHOST_COLS_RIGHT = 1;
	
	// Tipo de particionado seleccionado.
	if(PARTITIONING_METHOD == 0){
		//strip decomposition
		NCOLS_METHOD = NCOLS/nprocs;
		
	}else if(PARTITIONING_METHOD == 1){
		//block decomposition
		NROWS_METHOD = NROWS/nprocs;
		NCOLS_METHOD = NCOLS/nprocs;
		
	}
	
	// Getting core in execution.
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	// Setting size of each strip column to send to each core.
	MPI_Type_vector(NROWS, NCOLS, NROWS, MPI_INT, &coltypeAll);
	MPI_Type_vector(NROWS, NCOLS_METHOD, NCOLS, MPI_INT, &coltypeReturn);

	MPI_Type_commit(&coltypeAll);
	MPI_Type_commit(&coltypeReturn);
	
	//Establecer cantidad hilos para paralelizar con OpenMP.
	omp_set_num_threads(NTHREADS);
	
	// Inicializar tablero.
	initialize(Matrix, NROWS, NCOLS);

	
	int iniCol = 0; // Indice inicial de particionado por cada CPU.

	// Plantar semillas por cada CPU.
	for(int i=0; i < nprocs; i++){

		int finCol = NCOLS_METHOD +  finCol;
		printf("ini: %d fin: %d\n", iniCol, finCol);
		setSeeds(SEED, Matrix, NROWS_METHOD, NCOLS_METHOD, iniCol, finCol,NCOLS);

		iniCol = iniCol + NCOLS_METHOD;
	}
	
	printf("Tablero inicial:\n");
	printValues(Matrix, NROWS, NCOLS);
	
	// Ejecución de algoritmo por iteraciones seleccionadas.
	for(int it=0;it<ITERATIONS;it++){
		
		// Ejecución de maestro.
		if(rank == 0){

			// Envio particiones del tablero a esclavos.
			for(int rankEnvio = 1; rankEnvio < nprocs; rankEnvio++)
				MPI_Send(&Matrix[0],1,coltypeAll,rankEnvio,rankEnvio,MPI_COMM_WORLD);

			// Establecer poblacion viva y muerta del particionado de maestro.
			setLifeAndDead(Matrix, NROWS, NCOLS, 0, NCOLS_METHOD);

			// Proceso

			// Unificar resultados 

			int iniCol2 = NCOLS_METHOD; // Discounting master's work.

			// Recuperar particiones del tablero modificadas por esclavos.
			for(int rankEnvio=1; rankEnvio < nprocs; rankEnvio++){
				MPI_Recv(&Matrix[iniCol2], 1, coltypeReturn, rankEnvio, rankEnvio, MPI_COMM_WORLD, &status);
				
				iniCol2 += NCOLS_METHOD;
			}

		// Ejecucion de esclavos.
		} else {
			
			// Recuperar particion del tablero enviada de maestro.
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
			
			// Establecer poblacion viva y muerta del particionado enviado por maestro.
			setLifeAndDead(Matrix, NROWS, NCOLS, inicio, fin);
			
			printf("rank = %d;Rango a Considerar = (%d, %d); \n", rank, inicio, fin);
			//printValues(Matrix, NROWS, NCOLS);
			
			// Enviar particionado modificado de vuelta a maestro.
			MPI_Send(&Matrix[rank*NCOLS_METHOD], 1, coltypeReturn, 0, rank, MPI_COMM_WORLD);
		}
	}
	
	// Impresion de tablero con resultado final.
	printf("Result:\n");
	printValues(Matrix, NROWS, NCOLS);
	
	MPI_Finalize(); // Finalizar proceso MPI.
	
	// Computo tiempo final.
	double time = omp_get_wtime() - start_time;
	printf("\nClock: %lf\n", time);
	
	return 0;
}

// Inicializa tablero en ceros.
void initialize(int *Matrix, int NROWS, int NCOLS){
	
	for(int i=0;i<NROWS;i++){
		for(int j=0;j<NCOLS;j++){
			Matrix[i*NCOLS + j]=0;
		}
	}
	
}

// Planta semillas en tablero.
void setSeeds(int SEED, int *Matrix, int NROWS_METHOD, int NCOLS_METHOD, int iniCol, int finCol, int NCOLS){
	
	int numUnos = 0;
	int abort = 0;
	
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
	
}

// Visualiza tablero.
void printValues (int *Matrix, int NROWS, int NCOLS){
	
	for(int i=0;i<NROWS;i++){
		for(int j=0;j<NCOLS;j++){
 
			  printf("%d , ", Matrix[i*NCOLS+j]);

			}
		printf("\n");
		}
		
}	

// Establecer poblacion viva y muerta del tablero.
void setLifeAndDead(int *Matrix, int NROWS, int NCOLS, int iniCol, int finCol) {

	int *temp = (int *)malloc(sizeof(int)*NROWS*NCOLS);	
	
	for(int i=0;i<NROWS;i++){
		for(int j=0;j<NCOLS;j++){
 
			  temp[i*NCOLS+j] = Matrix[i*NCOLS+j];

			}
	}
	
    int neighbours = 0;

	// Obtiene cantidad de vecinos 
	// y marca sujetos como vivos o muertos.
    for(int fila=0;fila<NROWS;fila++){
		for(int columna=iniCol;columna<finCol;columna++){
            neighbours = getNeighbors(Matrix, columna, fila, NROWS, NCOLS);
            if (neighbours < 3 && Matrix[fila*NCOLS + columna] == 1) {
                temp[fila*NCOLS + columna] = 0;
            } else if (neighbours > 4 && Matrix[fila*NCOLS + columna] == 1) {
                temp[fila*NCOLS + columna] = 0; 
            } else if (neighbours == 3 && Matrix[fila*NCOLS + columna] == 0) {
                temp[fila*NCOLS + columna] = 1; 
            }
        }
    }
	
	if(iniCol == 0){
		//Rank 0
		finCol--;
		
	}else if(finCol == NCOLS){
		//ULtimo Rank
		iniCol--;
	}else{
		//Ranks de en medio
		finCol--;
		iniCol--;
	}
	
    for(int fila=0;fila<NROWS;fila++){
		for(int columna=iniCol;columna<finCol;columna++){
            Matrix[fila*NCOLS + columna] = temp[fila*NCOLS + columna];
        }
    }
}

// Entrega cantidad de vecinos vivos de un vecindario.
int getNeighbors(int *Matrix, int x, int y, int NROWS, int NCOLS) {

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
