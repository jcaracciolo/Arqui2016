#include "include/process.h"
#include "include/lib.h"
#include "include/buddyMemManager.h"
#include "include/videoDriver.h"
#include "include/syscall.h"
#include "include/pipe.h"

#define INIT_PROCESS_PAGES 4

static uint64_t nextPID = 0;

typedef struct {
	//Registers restore context
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;

	uint64_t ret;
} stack_frame;

int equalProcesses(process * p1, process * p2) {
	return p1->pid == p2->pid;
}

void freeProcess(process * process) {
	buddyFree(process->stack_base);
	free(process);
}

process * createProcess(void * entryPoint, int cargs, void ** pargs) {
	process * newProcess = (process *)malloc(sizeof(process));
	newProcess->entry_point = entryPoint;
	newProcess->stack_base = buddyAllocatePages(INIT_PROCESS_PAGES);
	newProcess->cantPages = INIT_PROCESS_PAGES;
	newProcess->stack_pointer = fill_stack(entryPoint, newProcess->stack_base + newProcess->cantPages * PAGE_SIZE, cargs, pargs);
	newProcess->pid = nextPID++;
	for(int i=0;i<5;i++){
		newProcess->fd[i]=0;
	}
	newProcess->state = READY;
	if(cargs != 0) {
		newProcess->descr = pargs[0];
//		print("NAME->"print(newProcess->descr);
	} else {
		newProcess->descr = "no_descr";
	}
	return newProcess;

}

void callProcess(int cargs, void ** pargs, void * entryPoint) {
	((int (*)(int, void**))(entryPoint))(cargs, pargs);

	sys_leave(0, 0, 0, 0, 0);
}

void * fill_stack(void * entryPoint, void * stack_base, int cargs, void ** pargs) {
	stack_frame * frame =  (stack_frame *)(stack_base - sizeof(stack_frame) -1);

	frame->gs =		0x001;
	frame->fs =		0x002;
	frame->r15 =	0x003;
	frame->r14 =	0x004;
	frame->r13 =	0x005;
	frame->r12 =	0x006;
	frame->r11 =	0x007;
	frame->r10 =	0x008;
	frame->r9 =		0x009;
	frame->r8 =		0x00A;
	frame->rsi =	pargs;
	frame->rdi =	cargs;
	frame->rbp =	0x00D;
	frame->rdx =	entryPoint;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;

    //iret hook
	frame->rip =	(void*) &callProcess;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return (void *)frame;
}