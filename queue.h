#ifndef QUEUE_H
#define QUEUE_H
// this class was taken from cp264 and modified for the process class
#include <stdio.h>
#include <malloc.h>
#include "process.h"
typedef struct qnode {
  struct process *p;
  struct qnode *next;
} QNODE;

void enqueue(QNODE **frontp, QNODE **rearp, PROCESS* p);
PROCESS* dequeue(QNODE **frontp, QNODE **rearp);
PROCESS* peek(QNODE *frontp, QNODE *rearp);
#endif 