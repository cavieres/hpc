#include <stdlib.h>
#include<stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif

//mpirun -np PROCESSORS ./gol -r NROWS -c NCOLS -i ITERATIONS -m PARTITIONING_METHOD -t NTHREADS -s SEED

int main(int argc, char *argv[]){

  //Clock
  double start_time = omp_get_wtime();
  
  int PROCESSORS = atoi(argv[2]);
  int NROWS = atoi(argv[5]);
  int NCOLS = atoi(argv[7]);
  int ITERATIONS = atoi(argv[9]);
  int PARTITIONING_METHOD = atoi(argv[11]);
  int NTHREADS = atoi(argv[13]);
  int SEED = atoi(argv[15]);
  
  int **Matrix = (int **)malloc(NROWS*sizeof(int*));
  for(int i=0;i<NROWS;i++){
	  Matrix[i] = (int*) malloc(NCOLS*sizeof(int));
  }
  
  //Num HIlos
  omp_set_num_threads(Hilos);
  
  if(PARTITIONING_METHOD == 0){
	  //strip decomposition
	  
  }else if(PARTITIONING_METHOD == 1){
	  //block decomposition
	  
  }
  
  
  double time = omp_get_wtime() - start_time;
  printf("\nClock: %lf ", time); 
}

void stripDecomposition(int **Matrix, int PROCESSORS,int NROWS, int NCOLS){
	
	for(int i=0;i<NROWS;i++){
		
		for(int x=0;x<(NCOLS/PROCESSORS);x++){
			
		}
	}
}