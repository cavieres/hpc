#include <stdio.h>
#include <stdlib.h>
#define TOTAL_SECUENCES 16

float *stack;
int *stackCount;

void push(float value, int secuence)
{
	stackCount[secuence]++;
	//stack[secuence][stackCount[secuence]] = value;
	stack[TOTAL_SECUENCES * secuence + stackCount[secuence]] = value;
}

float pop(int secuence)
{
	if (stackCount[secuence] == 0)
		return 0;
		
	float value = stack[TOTAL_SECUENCES * secuence + stackCount[secuence]];
	stackCount[secuence]--;
	return value;
}

float peek(int secuence)
{
	if (stackCount[secuence] == 0)
		return 0;	
	return stack[TOTAL_SECUENCES * secuence + stackCount[secuence]];
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
	int secsQty = 3;
	stack = (float *)malloc(secsQty * TOTAL_SECUENCES * sizeof(float));
	stackCount = (int *)malloc(secsQty * sizeof(int));
	
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
	
	pop(2);
	pop(2);
	printf("peek sec. III: %f\n", peek(2));
}