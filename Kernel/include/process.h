#ifndef PROCESS_H
#define PROCESS_H



typedef struct process_t process;


/* Returns 1 if p1 is the same process as p2* and 0 if different */
int equalProcesses(process * p1, process * p2);

void freeProcess(process * process);


void * fill_stack(void * entryPoint, void * stack_base);
process * createProcess(void * entryPoint);

#endif