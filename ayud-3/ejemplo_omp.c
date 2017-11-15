#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include <omp.h>



double pi(double left,double right, double dx){
	double x;
	double y;
	double sum=0;
	#pragma omp parallel for schedule(dynamic,4)
	for( x = left; x< right; x+= dx){
		y = sqrt(1.0-x*x);
		sum += dx*y;
	}
	return sum*4;
}

int main(int argc, char* const argv[]){
	double dx = atof(argv[1]);
	int nthread = atoi(argv[2]);
	omp_set_num_threads(nthread);
	double _pi = pi(0,1,dx);
	printf("%f\n",_pi);
}
