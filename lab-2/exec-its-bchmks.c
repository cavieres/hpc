#include <stdio.h>

int main()
{
	int grillas[2] = {128, 256};
	int pasos[3] = {2000, 4000, 8000};
	
	for (int i = 1; i <= 14; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 3; k++)
				printf("./wave -N %d -T %d -H %d -f wave-%d-%d-%d.raw -t %d\n", grillas[j], pasos[k], i, grillas[j], pasos[k], i, pasos[k]);
	
}