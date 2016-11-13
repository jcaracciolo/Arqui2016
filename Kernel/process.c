#include "include/process.h"
#include "include/lib.h"

#define INIT_PROCESS_PAGES 5

static uint64_t nextPID = 1;

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
	return p1->pid - p2->pid;
}


void freeProcess(process * process) {
	free(process);
}

process * createProcess(void * entryPoint) {
	process * newProcess = (process *)malloc(sizeof(process*));
	newProcess->entry_point = entryPoint;
	newProcess->stack_base = allocatePages(INIT_PROCESS_PAGES);
	newProcess->cantPages = INIT_PROCESS_PAGES;
	newProcess->stack_pointer = fill_stack(entryPoint, newProcess->stack_base + newProcess->cantPages * PAGE_SIZE);
	newProcess->pid = nextPID++;

	return newProcess;

}

void * fill_stack(void * entryPoint, void * stack_base) {
	stack_frame * frame = (stack_frame *) stack_base - 1;

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
	frame->rsi =	0x00B;
	frame->rdi =	0x00C;
	frame->rbp =	0x00D;
	frame->rdx =	0x00E;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;
	frame->rip =	entryPoint;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return (void *)frame;
}