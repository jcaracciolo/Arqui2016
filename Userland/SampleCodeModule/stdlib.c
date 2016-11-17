#include "include/stdlib.h"
#include "include/shell.h";
#include "include/types.h"
#include "include/stdio.h"

extern void int80(qword rdi, qword rsi, qword rdx, qword rcx, qword r8, qword r9);

int isAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isNum(char c){
	return (c >= '0' && c <= '9');
}

void* malloc(int bytes){
	void* address;
	int80(1,&address,bytes,0,0,0);
	return address;
}

int readData(){
	int n;
	int80(15,&n,0,0,0,0);
	return n;
}

void free(void * p){
	int80(2, p, 0, 0, 0, 0);
}

int openPipe(char * name){
	int fd;
	int80(32, name, &fd, 0, 0, 0);
	return fd;
}

void closePipe(char * name){
	int80(33, name, 0, 0, 0, 0);
}


void setConsoleSize() {
	int80(17,&ROWS,&COLS,0,0,0);
}

int exec(void * entry_point, int cargs, void ** pargs, int giveFg) {
	int pid;
	int80(27, entry_point, &pid, cargs, pargs, 0);
	if (giveFg) {
		kill(pid, 3); // set process to foreground
	}
	return pid;
}

void ps() {
	int80(28, 0, 0, 0, 0, 0);
}

void kill(int pid, int msg) {
	int80(29, pid, msg, 0, 0, 0);
}
