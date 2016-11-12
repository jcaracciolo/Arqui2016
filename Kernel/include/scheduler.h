#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

struct processSlot_t{
	process * process;
	struct processSlot_t * next;
	int a;	// Do not remove, if it isn't here the struct does not work well (why??)
};

typedef struct processSlot_t processSlot;

void addProcess(process * process);

void removeProcess(process * process);

void freeProcessSlot(processSlot * slot);

void schedule();

void beginScheduler();

int getCurrentPid();

#endif