#include <stdio.h>
#include <stdlib.h>
#define TOTAL_SECUENCES 16

float stack[][TOTAL_SECUENCES];
int stackCount[];

void push(float value, int secuence)
{
	stackCount[secuence]++;
	stack[secuence][stackCount[secuence]] = value;
}

float pop(int secuence)
{
	if (stackCount[secuence] == 0)
		return;
		
	float value = stack[secuence][stackCount[secuence]];
	stackCount[secuence]--;
}

float peek(int secuence)
{
	if (stackCount[secuence] == 0)
		return 0;	
	return stack[secuence][stackCount[secuence]];
}

int isStackEmpty(int secuence)
{
	if (stackCount[secuence] == 0)
		return 1;
	else
		return 0;
}

int main()
{
	// Secuence I.
	push(12, 0);
	push(15, 0);
	push(45, 0);
	
	// Secuence II.
	push(7, 1);
	push(8, 1);
	push(30, 1);
	
	// Secuence III.
	push(7, 2);
	push(8, 2);
	push(30, 2);
	
	printf("peek sec. I: %f\n", peek(0));
	pop(0);
	printf("peek sec. I: %f\n", peek(0));
	pop(0);

	printf("peek sec. I: %f\n", peek(0));
	pop(0);

	printf("peek sec. I: %f\n", peek(0));
	pop(0);

	if (isStackEmpty(0) == 1)	
		printf("I'm Empty.\n");
		
	printf("peek sec. II: %f\n", peek(1));
}