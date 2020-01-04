#include "queue.h"
// this class was taken from cp264 and modified for the process class
void enqueue(QNODE **frontp, QNODE **rearp, PROCESS *pro) 
{
	QNODE *node = (QNODE*) malloc(sizeof(QNODE));
	QNODE *ptr = *rearp;
	node->p = pro;
	
	if (*frontp == NULL) {
		*frontp = node;
		node->next = NULL;
		*rearp = node;
	}
	else {
		ptr->next = node;
		*rearp = node;
		node->next = NULL;
	}
}

PROCESS* dequeue(QNODE **frontp, QNODE **rearp) 
{
	PROCESS *p;
	
	if (*frontp == NULL) {
		printf("Underflow\n");
	}
	else {
		p = (*frontp)->p;
		QNODE *ptr = *frontp;
		*frontp = (*frontp)->next;
		free(ptr);
	}
	
	return p;
}
PROCESS* peek(QNODE *frontp, QNODE *rearp)
{
	PROCESS *p;
	p= (frontp)->p;

	return p;
}
