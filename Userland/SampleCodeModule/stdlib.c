#include "include/stdlib.h"
#include "include/shell.h";
#include "include/types.h"

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

void  free(){ return;}

void setConsoleSize() {
	int80(17,&ROWS,&COLS,0,0,0);
}

int exec(void * entry_point) {
	int pid;
	int80(22, entry_point, &pid, 0, 0, 0);
	return pid;
}

void ps() {
	int80(23, 0, 0, 0, 0, 0);
}

void kill(int pid, int msg) {
	int80(24, pid, msg, 0, 0, 0);
}

void leave() {
	int80(24, 0, -1, 0, 0, 0);	// removes process from scheduler
	int80(25, 0, 0, 0, 0, 0);	// calls next process
}
