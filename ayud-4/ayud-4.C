extern "C" {
	#include <bsp.h>
}

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

// 0, 1
double spmd_pi(double left, double right, float dx)
{
}

float getArea(float dx)
{
	float radio = 1;
	float area = 0;

	for (float i = 0; i < 1; i = i + dx)
	{
		float x = dx;
		float y = sqrt(abs(radio - pow(x, 2)));
		area += x * y;
	}
	
	return area * 4;
}

int main(int argc, char *argv[])
{
	int tag = 0;
	
	bsp_init(spmd_pi, argc, argv);
	bsp_send(0, &tag, $r, double);
	bsp_sync();
	bsp_end();
	
	return 0;
}
