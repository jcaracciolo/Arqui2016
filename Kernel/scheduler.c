#include "include/process.h"
#include "include/lib.h"
#include "include/scheduler.h"
#include "include/defs.h"
#include "include/videoDriver.h"

extern _get_rsp();
extern _set_rsp(void * rsp);

static processSlot * current = NULL;


void insertProcess(void * entryPoint) {
	process * p = createProcess(entryPoint);
	addProcess(p);
}

void addProcess(process * process) {
	processSlot * slot = (processSlot *)malloc(sizeof(processSlot));
	slot->process = process;
	//slot->pid = process->pid;

	if (current == NULL) {
		print("== NULL\n");
		current = slot;
		current->next = current;
	} else {
		print("!= NULL\n");
		//print("antes: "); printNum(current->process->pid); print("\n");
		slot->next = current->next;
		current->next = slot;
		//print("despues: "); printNum(current->process->pid); print("\n");
	}
	print("inserted pid: "); printNum(process->pid); print(" "); printNum(current->process->pid); print("\n");
}

int getCurrentPid() {
	return current->process->pid;
}

void removeProcess(process * process) {
	if (current == NULL) {
		return;
	} else if(equalProcesses(current->process, current->next->process) && equalProcesses(current->process, process)) {
		// process to remove is the current and only one process in list
		freeProcessSlot(current);
		current = NULL;
		return;
	}

	processSlot * prevSlot = current;
	processSlot * slotToRemove = current->next;

	while (!equalProcesses(slotToRemove->process, process)) {
		prevSlot = slotToRemove;
		slotToRemove = slotToRemove->next;
	}

	if (equalProcesses(slotToRemove->process, current->process)) {
		// process to remove is the current
		current = current->next;
	}

	prevSlot->next = slotToRemove->next;
	freeProcessSlot(slotToRemove);
}

void freeProcessSlot(processSlot * slot) {
	freeProcess(slot->process);
	free(slot);
}

void schedule() {
	//print("pid: "); printNum(current->process->pid); print("\n");

	//current->process->stack_pointer = _get_rsp();
	current = current->next;
	//_set_rsp(current->process->stack_pointer);
}

void beginScheduler() {
	print("current pid: "); printNum(current->process->pid); print("\n");
	((int (*)(void))(current->process->entry_point))();
}