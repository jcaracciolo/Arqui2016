#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct processSlot_t{
	struct processSlot_t * next;
	process * process;
} processSlot;

void addProcess(process * process);

void removeProcess(process * process);

void freeProcessSlot(processSlot * slot);

#endif