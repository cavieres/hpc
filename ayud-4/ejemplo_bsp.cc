extern "C"{
#include<bsp.h>
}

#include<stdlib.h>
#include<math.h>
#include<stdio.h>

double pi(double left,double right, double dx){
	double x;
	double y;
	double sum=0;
	for( x = left; x< right; x+= dx){
		y = sqrt(1.0-x*x);
		sum += dx*y;
	}
	return sum*4;
}


void spmd_pi(){
	bsp_begin(bsp_nprocs());
	double localpi = pi(0,1,.0001);
	int tag = bsp_pid();
	bsp_send(0,&tag,&localpi,sizeof(double));
	bsp_sync();
	if(tag==0){
		int i,npaq,nbytes;
		bsp_qsize(&npaq,&nbytes);
		double total=0;
		for(i=0;i<npaq;i++){
			bsp_move(&localpi,sizeof(double));
			total+=localpi;
		}
		printf("%f\n",total);
	}
	bsp_end();
}

int main(int argc, char** const argv){
	bsp_init(spmd_pi,argc,argv);
	spmd_pi();
	return(0);
}
