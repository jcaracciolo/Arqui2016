#include "include/process.h"
#include "include/lib.h"
#include "include/scheduler.h"
#include "include/defs.h"
#include "include/videoDriver.h"

extern void * _get_rsp();
extern void _set_rsp(void * rsp);
extern void * get_rip();
extern void _pushAll();
extern void _popAll();

static processSlot * current = NULL;
static processSlot * foreground = NULL;
static int cantProcesses = 0;


int insertProcess(void * entryPoint) {
	process * p = createProcess(entryPoint);
	return addProcessSlot(p);
}

int addProcessSlot(process * process) {
	processSlot * slot = (processSlot *)malloc(sizeof(processSlot));
	slot->process = process;

	if (current == NULL) {
		current = slot;
		foreground = slot;
		current->next = current;
	} else {
		slot->next = current->next;
		current->next = slot;
	}
	cantProcesses++;

	return process->pid;
}

int getCurrentPid() {
	return current->process->pid;
}

int getforegroundPid() {
	return foreground->process->pid;
}

void setForeground(int pid) {
	int i = 0;
	processSlot * slot = foreground;
	for (; i < cantProcesses; i++) {
		if (slot->process->pid == pid) {
			// new foreground process found
			foreground = slot;
			return;
		}
		slot = slot->next;
	}
	// pid does not exists
	return;
}

void killProcess(int pid) {
	int i = 0;
	processSlot * slot = current;
	for (; i < cantProcesses; i++) {
		if (slot->process->pid == pid) {
			// process found
			slot->process->state = DEAD;
			return;
		}
		slot = slot->next;
	}
	// pid does not exists
	return;
}



void removeProcess(int pid) {
	if (current == NULL) {
		return;
	} else if(equalProcesses(current->process, current->next->process) && current->process->pid == pid) {
		// process to remove is the current and only one process in list
		freeProcessSlot(current);
		current = NULL;
		foreground = NULL;
		cantProcesses--;
		return;
	}

	processSlot * prevSlot = current;
	processSlot * slotToRemove = current->next;

	while (slotToRemove->process->pid != pid) {
		prevSlot = slotToRemove;
		slotToRemove = slotToRemove->next;
	}

	if (equalProcesses(slotToRemove->process, current->process)) {
		// process to remove is the current
		current = current->next;
	}

	prevSlot->next = slotToRemove->next;
	freeProcessSlot(slotToRemove);
	cantProcesses--;
	printAllProcesses();
}

void printAllProcesses() {
	print("current: "); printNum(current->process->pid); print("\n");

	processSlot * slot  = current;
	int i = 0;
	for(; i < cantProcesses; i++) {
		print("pid: "); printNum(slot->process->pid); print(" state: "); printNum(slot->process->state); print("\n");
		slot = slot->next;
	}
}

void freeProcessSlot(processSlot * slot) {
	freeProcess(slot->process);
	free(slot);
}

void * next_process(int current_rsp) {
	//printAllProcesses();

	if (current == NULL) {
		return current_rsp;
	}
	current->process->stack_pointer = current_rsp;
	//current->process->entry_point = _get_rip();

	schedule();

	return current->process->stack_pointer;
}

void schedule() {
	if (current->process->state == DEAD) {
			print("Process found DEAD.\n");
			removeProcess(current->process->pid);
	}

	current->process->state = READY;

	current = current->next;
	while (current->process->state != READY) {
		if (current->process->state == DEAD) {
			print("Process found DEAD.\n");
			removeProcess(current->process->pid);
		} else 
		current = current->next;
	}

	current->process->state = RUNNING;
}

void beginScheduler() {
	print("current pid: "); printNum(current->process->pid); print("\n");

	((int (*)(void))(current->process->entry_point))();
}