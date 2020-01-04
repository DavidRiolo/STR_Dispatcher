#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"

PROCESS* create_process(int pid, int start_time, int run, int blocks[],int num_exchanges)
{
	int i=0; 
	PROCESS *p = (PROCESS*)malloc(sizeof(PROCESS));
	
	p->PID = pid;
	p->start_time=start_time;
	p->run_time=run;
	
	while (blocks[i]!=0){
	p->interupts[i] = blocks[i];
	i++;
	}
	p->interupts_index = 0;
	p->current_run_time = 0;
	p->suspended_time = 0;
	p->ready_time = 0;
	p->run_time_remaining = run;
	p->num_exchanges=num_exchanges;
	//gets the time if there is no itterupt
	if (p->num_exchanges==0){
		p->move_time=run;
		
	}
	//if there is an itterupt
	else{
		
		p->move_time=p->interupts[0];
	}
	
	
	return p;
}
