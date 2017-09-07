#include<stdio.h>
#include<stdlib.h>
#define SIZE_SECUENCES 16

typedef struct node {
	int secNum;
	float secData[SIZE_SECUENCES + 1];
	int stackCount;
	struct node *next;
} queue;

queue *head = NULL;

void push(float value, int secuence)
{
	queue *temp = (queue*)malloc(sizeof(queue));
	
	temp->secNum = secuence;
	temp->stackCount++;
	temp->secData[temp->stackCount] = value;
	temp->next = NULL;
	
	if(head == NULL){
		head = temp;
		return;
	}
	else {
		queue *list = head;
		
		while (list != NULL)
		{
			if (list->secNum == secuence)
			{
				//list->secNum = secuence;
				list->stackCount++;
				list->secData[list->stackCount] = value;
				list->next = NULL;
				return;
			}
			
			list = list->next;
		}
	}
	
	head->next = temp;
}

float pop(int secuence)
{
	queue *list = head;
	
	if (list == NULL)
	{
		// List is empty.
		return 0;
	}
	
	while (list != NULL)
	{
		if (list->secNum == secuence)
		{
			if (list->stackCount == 0)
				return 0;
				
			list->stackCount--;
				
			return list->secData[list->stackCount + 1];
		}
		
		list = list->next;
	}
}

float peek(int secuence)
{
	queue *list = head;
	
	while (list != NULL)
	{
		if (list->secNum == secuence)
		{
			if (list->stackCount == 0)
			{
				//printf("No data in sec. %d\n", secuence);
				return 0;
			}
			/*printf("top queue sec. %d: %f\n", secuence, list->secData[list->stackCount]);
			printf("sec. %d total: ", secuence);
			for (int k = 1; k <= SIZE_SECUENCES; k++)
				printf("%f ", list->secData[k]);
			printf("\n");*/
			return list->secData[list->stackCount];
		}
		
		list = list->next;
		
	}
	printf("No data in sec. %d\n", secuence);
}

/*int main()
{
	//head = malloc(sizeof(queue));
	push(13, 0);
	push(21, 0);
	push(75.4, 0);
	push(150, 0);
	
	push(70, 2);
	push(90, 2);
	
	pop(0);
	pop(0);
	pop(0);
	pop(0);
	pop(0);
	pop(2);
	
	printf("sec. 0: %f\n", peek(0));
	printf("sec. 1: %f\n", peek(1));
	printf("sec. 2: %f\n", peek(2));
	
	return 1;
}*/