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
	print("inserted stack: "); printNum(process->stack_base); print(" "); printNum(current->process->stack_base); print("\n");
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

void * next_process(int current_rsp) {
	print("change rsp to: "); printNum(current_rsp); print("\n");
	if (current == NULL) {
		return current_rsp;
	}
	current->process->stack_pointer = current_rsp;
	current->process->entry_point = _get_rip();

	current = current->next;
	print("new rsp to: "); printNum(current->process->stack_pointer); print("\n");
	return current->process->stack_pointer;
}

void schedule() {
	_pushAll();
	print("a");
	current->process->stack_pointer = _get_rsp();
	current = current->next;
	_set_rsp(current->process->stack_pointer);
	_popAll();

	//print("schedule pid: "); printNum(current->process->pid); print(" schedule stack"); printNum(current->process->stack_base); print("\n");
}

void beginScheduler() {
	print("current pid: "); printNum(current->process->pid); print("\n");
	((int (*)(void))(current->process->entry_point))();
}