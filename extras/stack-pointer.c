#include<stdio.h>
#include<stdlib.h>
#define TOTAL_SECUENCES 16

typedef struct node {
	int secNum;
	float secData[TOTAL_SECUENCES + 1];
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
				list->secNum = secuence;
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

float peek(int secuence)
{
	queue *list = head;
	
	while (list != NULL)
	{
		if (list->secNum == secuence)
		{
			printf("top queue: %f\n", list->secData[list->stackCount]);
			printf("sec. %d total: %f %f %f %f %f %f\n", secuence, list->secData[1], list->secData[2], list->secData[3], list->secData[4], list->secData[5], list->secData[6], list->secData[7]);
			return 1;
		}
		
		list = list->next;
		
	}
	printf("No data in sec. %d\n", secuence);
}

int main()
{
	//head = malloc(sizeof(queue));
	push(13, 0);
	push(21, 0);
	push(75.4, 0);
	push(150, 0);
	
	push(70, 2);
	push(90, 2);
	
	peek(0);
	peek(1);
	peek(2);
	
	return 1;
}