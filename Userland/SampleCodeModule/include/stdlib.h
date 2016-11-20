#ifndef STDLIB_H
#define STDLIB_H

#include "IPCstructs.h"
int isAlpha(char c);

void* malloc(int bytes);

int isNum(char c);

void free();

int readData();

void setConsoleSize();

void kill(int pid, int msg);

void ps();

int exec(void * entry_point, int cargs, void ** pargs, int giveFg);

int getPID();

void closePipe(char * name);

int openPipe(char * name);

void getIpcs(ipcs * ans,int mutexes,int pipes, int* cantMutexes, int* cantPipes);
#endif
