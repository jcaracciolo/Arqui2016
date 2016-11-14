#ifndef PROCESS_H
#define PROCESS_H

#include "lib.h"

typedef enum processState_t {RUNNING, READY, BLOCKED} processState;

typedef struct process_t{
	void * entry_point;		// instruction pointer
	void * stack_base;		// beggining of stack's pages
	void * stack_pointer;	// relative to stack_base

	uint64_t cantPages;
	processState state;

	uint64_t pid;
	uint64_t ppid;
} process;


/* Returns 1 if p1 is the same process as p2* and 0 if different */
int equalProcesses(process * p1, process * p2);

void freeProcess(process * process);


void * fill_stack(void * entryPoint, void * stack_base);
process * createProcess(void * entryPoint);

#endif