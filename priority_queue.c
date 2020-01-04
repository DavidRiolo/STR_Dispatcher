#include "priority_queue.h"
//TAKEN FROM CP264 AND MODIFIED

void enpqueue(PQNODE **pqfrontp, PQNODE **pqrearp, PROCESS *p) 
{
// your code
	PQNODE *node = (PQNODE*) malloc(sizeof(PQNODE));
	node->p = p;
	PQNODE *temp = *pqfrontp;
	PQNODE *prev = NULL;
	
	if (*pqfrontp == NULL) {
		*pqfrontp = node;
		node->next = NULL;
		*pqrearp = node;
	}
	else {
		while (temp->next != NULL && (temp->p->run_time_remaining) <= (node->p->run_time_remaining)) {
			prev = temp;
			temp = temp->next;
		}
		
		if (temp->next == NULL && (temp->p->run_time_remaining) <= (node->p->run_time_remaining)) {
			temp->next = node;
			*pqrearp = node;
			node->next = NULL;
		}
		else if (prev == NULL) {
			node->next = temp;
			*pqfrontp = node;
		}
		else {
			prev->next = node;
			node->next = temp;
		}
	}
}

PROCESS* depqueue(PQNODE **pqfrontp, PQNODE **pqrearp) 
{
// your code
	PROCESS *p;
	
	if (*pqfrontp == NULL) {
		printf("PRIORITY QUEUE EMPTY\n");
	}
	else {
		p = (*pqfrontp)->p;
		PQNODE *ptr = *pqfrontp;
		*pqfrontp = (*pqfrontp)->next;
		free(ptr);
	}
	
	return p;
}

int pqpeek(PQNODE *pqfront) 
{
// your code
	PQNODE *ptr = pqfront;
	
	int value = ptr->p->run_time_remaining;
	
	return value;
}

int pqis_empty(PQNODE *pqfront) {
	if (pqfront) {
		return 1;
	}
	else {
		return 0;
	}
}