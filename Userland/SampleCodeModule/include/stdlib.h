#ifndef STDLIB_H
#define STDLIB_H

int isAlpha(char c);

void* malloc(int bytes);

int isNum(char c);

void free();

int readData();

void setConsoleSize();

void kill(int pid, int msg);

void ps();

int exec(void * entry_point, int cargs, void ** pargs);

#endif
