#include "include/stdlib.h"
#include "include/shell.h";
#include "include/types.h"
#include "include/stdio.h"
#include "include/IPCstructs.h"

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

void getIpcs(ipcs * ans,int mutexes,int pipes, int* cantMutexes, int* cantPipes){
	int80(40, ans, mutexes, pipes, cantMutexes, cantPipes);
}
int openPipe(char * name){
	int fd;
	int80(33, name, &fd, 0, 0, 0);
	return fd;
}

void closePipe(char * name){
	int80(34, name, 0, 0, 0, 0);
}

int getPID(){
	int pid;
	int80(32, &pid, 0, 0, 0, 0);
	return pid;}

int isRunningSync(char* psName){
	int ret;
	int80(41, psName, &ret, 0, 0, 0);
	return ret;
}

void setConsoleSize() {
	int80(17,&ROWS,&COLS,0,0,0);
}

void getMemoryUsed() {
	int memory;
	int80(42,&memory,0,0,0,0);
	printf("MEMORY USED: %d bytes\n",memory);
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
    char * buffer=malloc(3000);
	int80(28, buffer, 3000, 0, 0, 0);
    printf(buffer);
    free(buffer);
}

void kill(int pid, int msg) {
//	printf("someones is killing %d with %d",pid,msg);
	int80(29, pid, msg, 0, 0, 0);
}


void isRunning(int cargs, void ** pargs) {
	char * psName = (((char**)pargs)[1]);
	int ret;
	int80(41, psName, &ret, 0, 0, 0);
	printf("%s\n", ret == 0 ? "No" : "Yes");
}
