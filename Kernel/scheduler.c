#include "include/process.h"
#include "include/lib.h"
#include "include/scheduler.h"

static processSlot * current;

void addProcess(process * process) {
	processSlot * slot = (processSlot *)malloc(sizeof(processSlot));

	if (current == 0) {
		current = slot;
		current->next = current;
	} else {
		slot->next = current->next;
		current->next = slot;
	}
}

void removeProcess(process * process) {
	if (current == 0) {
		return;
	} else if(equalProcesses(current->process, current->next->process) && equalProcesses(current->process, process)) {
		// process to remove is the current and only one process in list
		freeProcessSlot(current);
		current = 0;
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