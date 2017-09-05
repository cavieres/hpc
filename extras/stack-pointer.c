#include<stdio.h>
#include<stdlib.h>
#define TOTAL_SECUENCES 16

typedef struct node {
	int secNum;
	float secData[TOTAL_SECUENCES];
	int stackCount;
	struct node *next;
} queue;

queue *head = NULL;

void push(float value, int secuence)
{
	queue *temp = (queue*)malloc(sizeof(queue));
	
	temp->stackCount++;
	temp->secData[temp->stackCount] = value;
	temp->next = NULL;
	
	if(head == NULL){
		head = temp;
		return;
	}
	/*else {
		queue *list = head;
		
		while (list != NULL)
		{
			if (list->secNum == secuence)
			{
				list->stackCount++;
				list->secData[list->stackCount] = value;
				list->next = NULL;
			}
			
			list = list->next;
		}
	}*/
	
	head->next = temp;
}

float peek(int secuence)
{
	queue *list = head;
	
	while (list != NULL)
	{
		if (list->secNum == secuence)
		{
			printf("queue: %f\n", list->secData[list->stackCount]);
			return 1;
		}
		
		list = list->next;
	}
	printf("No lo encontre.\n");
}

int main()
{
	//head = malloc(sizeof(queue));
	push(13, 0);
	
	peek(0);
	
	return 1;
}