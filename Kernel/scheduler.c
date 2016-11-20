#include "include/process.h"
#include "include/lib.h"
#include "include/scheduler.h"
#include "include/defs.h"
#include "include/videoDriver.h"
#include "include/mutex.h"
#include "include/scheduler.h"
#include "include/strings.h"
#include "include/keyboardDriver.h"

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

process* getMyProcessData(){
	return current->process;
}


int insertProcess(void * entryPoint, int cargs, void ** pargs) {
	process * p = createProcess(entryPoint, (int)cargs, (void **)pargs);
    return addProcessSlot(p);

}

int addProcessSlot(process * process) {

	processSlot * slot = (processSlot *)malloc(sizeof(processSlot));
	int notPreviouslyLocked=lockScheduler();

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
	if(notPreviouslyLocked) unlockScheduler();

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
	int notPreviouslyLocked=lockScheduler();

	processSlot * slot = foreground;
	for (; i < cantProcesses; i++) {
		if (slot->process->pid == pid) {
			// new foreground process found
			foreground = slot;
//			print("\nnew foreground proceed: ");
//			printNum(getForegroundPid());
			signalCondVar(getSTDINCondVar());
			if(notPreviouslyLocked) unlockScheduler();

			return;
		}
		slot = slot->next;
	}
	// pid does not exists
	if(notPreviouslyLocked) unlockScheduler();

	return;
}

void changeProcessState(int pid, processState state) {

	int i = 0;
    int notPreviouslyLocked=lockScheduler();
    processSlot * slot = current;
	for (; i < cantProcesses; i++) {
		if (slot->process->pid == pid) {
			// process found
			slot->process->state = state;

			if (slot->process->pid == getForegroundPid() && state == DEAD) {
				setForeground(1);
//				printAllProcesses();
			}
			if(notPreviouslyLocked) unlockScheduler();
			return;
		}
		slot = slot->next;
	}
	if(notPreviouslyLocked) unlockScheduler();
	// pid does not exists
	print("pid not found: "); printNum(pid); print("\n");
	print("state: "); printNum(state); print("\n");
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

void sprintAllProcesses(char* buff, int size){
    int index=0;
    lockScheduler();
    processSlot * slot  = current;
    int i = 0;
    int copied;
    char pid[0];
    for(; i < cantProcesses; i++) {

        copied=strcpy(buff+index,slot->process->descr,size);
        index+=copied;
        size-=copied;
        if(size<=0) break;

        copied=strcpy(buff+index," pid: ",size);
        index+=copied;
        size-=copied;
        if(size<=0) break;

        intToString(pid,slot->process->pid);
        copied=strcpy(buff+index,pid,size);
        index+=copied;
        size-=copied;
        if(size<=0) break;

        copied=strcpy(buff+index," ",size);
        index+=copied;
        size-=copied;
        if(size<=0) break;

        copied=strcpy(buff+index,stateDescription[slot->process->state],size);
        index+=copied;
        size-=copied;
        if(size<=0) break;


        copied=strcpy(buff+index," ",size);
        index+=copied;
        size-=copied;
        if(size<=0) break;

        if(getForegroundPid() == slot->process->pid){
            copied=strcpy(buff+index," fg\n",size);
            index+=copied;
            size-=copied;
            if(size<=0) break;
        }else{
            copied=strcpy(buff+index," bg\n",size);
            index+=copied;
            size-=copied;
            if(size<=0) break;
        }
        slot = slot->next;
    }
    buff[index]='\0';
    unlockScheduler();
}

void printAllProcesses() {
	int notPreviouslyLocked=lockScheduler();

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
	if(notPreviouslyLocked) unlockScheduler();

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
			//print("Process found DEAD.\n");
			removeProcess(current->process->pid);
	}

	if(current->process->state == RUNNING)
		current->process->state = READY;

	current = current->next;
	while (current->process->state != READY) {
		if (current->process->state == DEAD) {
			//print("Process found DEAD.\n");
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