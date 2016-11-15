#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

struct processSlot_t{
	process * process;
	struct processSlot_t * next;
};

typedef struct processSlot_t processSlot;

int insertProcess(void * entryPoint, int cargs, void ** pargs);

int addProcessSlot(process * process);

void removeProcess(int pid);

void freeProcessSlot(processSlot * slot);

void schedule();

void beginScheduler();

int getCurrentPid();

void printAllProcesses();

void setForeground(int pid);

int getforegroundPid();

int getCurrentPid();

void changeProcessState(int pid, processState state);

#endif