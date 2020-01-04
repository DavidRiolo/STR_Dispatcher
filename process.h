 #ifndef PROCESS_H_
#define PROCESS_H_
#define True 1
#define False 0
typedef struct process{
	int PID; //process ID
	int run_time; //how long it needs to be in cpu to finish
	int start_time; //time at which the process can start in the cpu
	int interupts[100];//array of interrupts
	int interupts_index; //index for interupts array
	int current_run_time; //how long its running in the cpu
	int suspended_time; // time spent suspended
	int ready_time; //how long it stays in the ready queue
	int run_time_remaining; // time remmaining in cpu until process is done
	int move_time; //time left to move from current array
	int num_exchanges;//total interupts
} PROCESS;
PROCESS* create_process(int PID, int start_time, int runtime, int interupts[],int num_exchanges);
#endif /* PROCESS_H_ */
