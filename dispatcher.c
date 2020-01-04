/*
dispatcher.c

Student Name : David Riolo
Student ID # : 170718380

Dispatch Algorithm : Shortest time remaining 
*/

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#define MAX_LINE_LENGTH 100

/*
Any required standard libraries and your header files here
*/
#include "priority_queue.h"
#include "queue.h"
void dispatcher(FILE *fd, int harddrive){
    /*
        Your code here.
        You may edit the following code
    */
		
    char line_buffer[MAX_LINE_LENGTH];
    int start_time, run_time, process_id, num_exchanges, exchange_time;
    char *token;
	QNODE *initialq_front = NULL, *initialq_rear = NULL;//queue used to put all processes in at the start
	PQNODE *readyq_front = NULL, *readyq_rear = NULL;//processes that are ready to be put in the cpu
	QNODE *cpuq_front = NULL, *cpuq_rear = NULL;//processes in the cpu
	QNODE *harddrive_front = NULL, *harddrive_rear = NULL;//processtht is in the harddrive
	QNODE *finishedq_front = NULL, *finishedq_rear = NULL;//process that are finished 
	int timer=0;
	int swap=0;
    //make all queues 
    //PROCESS simulation input line by line
    while (fgets(line_buffer, MAX_LINE_LENGTH, fd) != NULL && line_buffer[0] != '\n'){
        token = strtok(line_buffer, " ");
        sscanf(token, "%d",&start_time);
       
        token = strtok(NULL, " ");
        sscanf(token, "%d",&process_id);
        
        token = strtok(NULL, " ");
        sscanf(token, "%d",&run_time);
        int harddrive_ex[100];
        num_exchanges = 0;
        token = strtok(NULL, " ");	
		int i=0;
        while ( token != NULL ){
			
			
            num_exchanges += sscanf(token, "%d",&exchange_time);
			harddrive_ex[i]=exchange_time;
            token = strtok(NULL, " ");
			i++;
        }
		//make a process
		PROCESS *p =create_process(process_id,start_time,run_time,harddrive_ex,num_exchanges);
		//enqueue to inital queue
		enqueue (&initialq_front,&initialq_rear,p);
		printf("PROCESS %3d wants to start at %6dms and run for %6dms and has %3d hard drive exchanges\n",  process_id, start_time, run_time, num_exchanges);    
    }	
	

	PROCESS *p,*p2;	
	int fake_array[0];
	PROCESS *p_zero=create_process(0,0,0,fake_array,0);	
	enqueue(&cpuq_front,&cpuq_rear,p_zero);
	while (initialq_front!=NULL ||readyq_front !=NULL|| cpuq_front->p !=p_zero|| harddrive_front!=NULL){
		PQNODE *pqtemp = readyq_front;
		//add timer value to all processes in queues
		//ready queue timer
		while (pqtemp!=NULL){
			
			pqtemp->p->ready_time++; 
			pqtemp=pqtemp->next;
		}
		
		QNODE *temp = harddrive_front;
		//harddrive timer
		if (temp!=NULL)
			temp->p->move_time--;
		while (temp!= NULL){
			temp->p->suspended_time++;
			temp=temp->next;
		}
		
		if (cpuq_front->p != p_zero){
			cpuq_front->p->run_time_remaining--;
			cpuq_front->p->current_run_time++;
			cpuq_front->p->move_time--;
		}
		else{
			p_zero->run_time++;
		}
		timer++;		
		
		//move to harddrive from cpu queue if move time is current time and there is atleast one exchange left	

		if (peek(cpuq_front,cpuq_rear)->move_time==0&&peek(cpuq_front,cpuq_rear)->num_exchanges-1>=peek(cpuq_front,cpuq_rear)->interupts_index)
		{	
			p=dequeue(&cpuq_front,&cpuq_rear);
			enqueue(&cpuq_front,&cpuq_rear,p_zero);			
			p->interupts_index++;
			p->move_time=harddrive;
			enqueue(&harddrive_front,&harddrive_rear,p);			
		}
		
		//move to finished queue from cpu queue if process is done		
		if (peek(cpuq_front,cpuq_rear)->run_time_remaining==0&&peek(cpuq_front,cpuq_rear)!=p_zero){
			p=dequeue(&cpuq_front,&cpuq_rear);
			enqueue(&cpuq_front,&cpuq_rear,p_zero);
			enqueue(&finishedq_front,&finishedq_rear,p);
			}		
		// move to ready queue from harddrive else if harddrive has somthing in it and its time for it to move and there is process time remaining,
		 if (harddrive_front!=NULL&&peek(harddrive_front,harddrive_rear)->move_time==0&&peek(harddrive_front,harddrive_rear)->run_time_remaining==0)
		{
			swap=0;
			p=dequeue(&harddrive_front,&harddrive_rear);
			enpqueue(&readyq_front,&readyq_rear,p);
			
			//swap ready queue and cpu queue if new ready queue value has smaller run time
			if (readyq_front->p->run_time_remaining<cpuq_front->p->run_time_remaining){
				p2=dequeue(&cpuq_front,&cpuq_rear);
				p=depqueue(&readyq_front,&readyq_rear);
				enqueue(&cpuq_front,&cpuq_rear,p);
				enpqueue(&readyq_front,&readyq_rear,p2);
				swap=1;
			}			
		}		
		//move it to ready queue from intial queue if the start time equals current timer 
		if (initialq_front!=NULL&&peek(initialq_front,initialq_rear)->start_time==timer)
		{
			swap=0;
			p=dequeue(&initialq_front,&initialq_rear);
			enpqueue(&readyq_front,&readyq_rear,p);			
			//swap ready queue and cpu queue if new ready queue value has smaller run time
			if (readyq_front->p->run_time_remaining<cpuq_front->p->run_time_remaining){
				p2=dequeue(&cpuq_front,&cpuq_rear);
				p=depqueue(&readyq_front,&readyq_rear);
				enqueue(&cpuq_front,&cpuq_rear,p);
				enpqueue(&readyq_front,&readyq_rear,p2);
				swap=1;
			}
		}
		
		//move to cpu queue from ready queue if there is something waiting and cpu is empty
		//appropiately update when the process should move
		if(peek(cpuq_front,cpuq_rear)==p_zero&&readyq_front!=NULL){
			p=depqueue(&readyq_front,&readyq_rear);
			p_zero=dequeue(&cpuq_front,&cpuq_rear);
			//gets the time if there is no itterupt			
			if (swap==0){
				if(p->num_exchanges==0){
					p->move_time=p->run_time_remaining;					
				}
				//get the time until the first interupt
				else if (p->num_exchanges==1||	p->interupts_index==0){
					p->move_time=p->interupts[p->interupts_index];	
				}
				//if there is more than one intterupt 
				else {
					p->move_time=(p->interupts[p->interupts_index]-p->interupts[p->interupts_index-1]);
				}
			}
			enqueue(&cpuq_front,&cpuq_rear,p);				
		}
		
		
		//to stop getting timer counts incorrect by one I run all possiblities through again
		
		//move to harddrive from cpu queue if move time is current time and there is atleast one exchange left	
		if (peek(cpuq_front,cpuq_rear)->move_time==0&&peek(cpuq_front,cpuq_rear)->num_exchanges-1>=peek(cpuq_front,cpuq_rear)->interupts_index)
		{	
			p=dequeue(&cpuq_front,&cpuq_rear);
			enqueue(&cpuq_front,&cpuq_rear,p_zero);			
			p->interupts_index++;
			p->move_time=harddrive;
			enqueue(&harddrive_front,&harddrive_rear,p);			
		}	
		
		//move to finished from cpu
		if (peek(cpuq_front,cpuq_rear)->run_time_remaining==0&&peek(cpuq_front,cpuq_rear)!=p_zero){
			p=dequeue(&cpuq_front,&cpuq_rear);
			enqueue(&cpuq_front,&cpuq_rear,p_zero);
			enqueue(&finishedq_front,&finishedq_rear,p);
		}
		
		
		// move to ready queue from harddrive else if harddrive has somthing in it and its time for it to move and there is process time remaining,
		if (harddrive_front!=NULL&&peek(harddrive_front,harddrive_rear)->move_time==0){
			p=dequeue(&harddrive_front,&harddrive_rear);
			swap=0;
			enpqueue(&readyq_front,&readyq_rear,p);
			//swap ready queue and cpu queue if new ready queue value has smaller run time
			if (readyq_front->p->run_time_remaining<cpuq_front->p->run_time_remaining){
				p2=dequeue(&cpuq_front,&cpuq_rear);
				p=depqueue(&readyq_front,&readyq_rear);
				enqueue(&cpuq_front,&cpuq_rear,p);
				enpqueue(&readyq_front,&readyq_rear,p2);
				swap=1;
			}
		}	
		//move it to ready queue from intial queue if the start time equals current timer 
		if (initialq_front!=NULL&&peek(initialq_front,initialq_rear)->start_time==timer)
		{
			swap=0;
			p=dequeue(&initialq_front,&initialq_rear);
			enpqueue(&readyq_front,&readyq_rear,p);		
			//swap ready queue and cpu queue if new ready queue value has smaller run time
			if (readyq_front->p->run_time_remaining<cpuq_front->p->run_time_remaining){
				p2=dequeue(&cpuq_front,&cpuq_rear);
				p=depqueue(&readyq_front,&readyq_rear);
				enqueue(&cpuq_front,&cpuq_rear,p);
				enpqueue(&readyq_front,&readyq_rear,p2);
				swap=1;
			}
		}		
		//move to cpu queue from ready queue if there is something waiting and cpu is empty
		//appropiately update when the process should move
		if(peek(cpuq_front,cpuq_rear)==p_zero&&readyq_front!=NULL){
			p=depqueue(&readyq_front,&readyq_rear);
			p_zero=dequeue(&cpuq_front,&cpuq_rear);
			//gets the time if there is no itterupt
			if (swap==0){
				if(p->num_exchanges==0){
					p->move_time=p->run_time_remaining;			
				}
				//get the time until the first interupt
				else if (p->num_exchanges==1||p->interupts_index==0){
					p->move_time=p->interupts[p->interupts_index];			
				}
				//if there is more than one intterupt 
				else {
					p->move_time=(p->interupts[p->interupts_index]-p->interupts[p->interupts_index-1]);
				}
			}
			enqueue(&cpuq_front,&cpuq_rear,p);			
		}			
	}
	printf("\n%6d%6d\n",p_zero->PID,p_zero->run_time);
	while(finishedq_front!=NULL){
		printf("%6d%6d%6d%6d\n",finishedq_front->p->PID,finishedq_front->p->run_time,finishedq_front->p->ready_time,finishedq_front->p->suspended_time);
		finishedq_front=finishedq_front->next;
	}	
}