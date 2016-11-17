#include "include/process.h"
#include "include/lib.h"
#include "include/scheduler.h"
#include "include/defs.h"
#include "include/videoDriver.h"
#include "include/mutex.h"

extern void * _get_rsp();
extern void _set_rsp(void * rsp);
extern void * get_rip();
extern void _pushAll();
extern void _popAll();

static processSlot * current = NULL;
static processSlot * foreground = NULL;
static int cantProcesses = 0;

static int amountFreeableProcess=0;
static processSlot * processToFree[100];

int debug=0;


int insertProcess(void * entryPoint, int cargs, void ** pargs) {
	process * p = createProcess(entryPoint, (int)cargs, (void **)pargs);
    return addProcessSlot(p);

}

int addProcessSlot(process * process) {

	processSlot * slot = (processSlot *)malloc(sizeof(processSlot));
    lockScheduler();

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

    unlockScheduler();

	return process->pid;
}

int getCurrentPid() {
	return current==NULL?-1:current->process->pid;
}

int getForegroundPid() {
	return foreground->process->pid;
}

void setForeground(int pid) {
	int i = 0;
    lockScheduler();
    processSlot * slot = foreground;
	for (; i < cantProcesses; i++) {
		if (slot->process->pid == pid) {
			// new foreground process found
			foreground = slot;
			unlockScheduler();
			return;
		}
		slot = slot->next;
	}
	// pid does not exists
	unlockScheduler();
	return;
}

void changeProcessState(int pid, processState state) {
	int i = 0;
    lockScheduler();
    processSlot * slot = current;
	for (; i < cantProcesses; i++) {
		if (slot->process->pid == pid) {
			// process found
			slot->process->state = state;

			unlockScheduler();
			return;
		}
		slot = slot->next;
	}
	unlockScheduler();
	// pid does not exists
	print("pid not found: "); printNum(pid); print("\n");
	return;
}

void addToFreeQueue(processSlot * slot){
    processToFree[amountFreeableProcess]=slot;
    amountFreeableProcess++;
}

void removeProcess(int pid) {
	if (current == NULL) {
		return;

	} else if(equalProcesses(current->process, current->next->process) && current->process->pid == pid) {
		// process to remove is the current and only one process in list
        addToFreeQueue(current);
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
	cantProcesses--;
    addToFreeQueue(slotToRemove);

}

void printAllProcesses() {
	lockScheduler();
	processSlot * slot  = current;
	int i = 0;
	for(; i < cantProcesses; i++) {
		print(slot->process->descr);
		print(" pid: "); printNum(slot->process->pid);
		print(" "); print(stateDescription[slot->process->state]);
		if (getForegroundPid() == slot->process->pid) {
			print(" "); print("fg");
		} else {
			print(" "); print("bg");
		}
		print("\n");
		slot = slot->next;
	}
	unlockScheduler();
}

void freeProcessSlot(processSlot * slot) {
	freeProcess(slot->process);
	free(slot);

}

void freeWaitingProcess(){
    for(int i=0;i<amountFreeableProcess;i++){
        freeProcessSlot(processToFree[i]);
    }
}

void * next_process(int current_rsp) {
	if (current == NULL || !lockScheduler()) {
		return current_rsp;
	}
	current->process->stack_pointer = current_rsp;

	schedule();
    int ans=current->process->stack_pointer;
    unlockScheduler();

    return ans;
}

void schedule() {
    amountFreeableProcess=0;

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
		} else{
				current = current->next;
		}
	}

	current->process->state = RUNNING;
}

void beginScheduler() {
	((int (*)(void))(current->process->entry_point))();
}