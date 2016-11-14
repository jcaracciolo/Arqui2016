#include "include/stdlib.h"
#include "include/shell.h";
#include "include/types.h"

extern void int80(qword rdi, qword rsi, qword rdx, qword rcx, qword r8, qword r9);

int createMutex(char * name) {
	int mutex = 0;
	int80(19, name, &mutex, 0, 0,0);
	return mutex;
}

int releaseMutex(int mutex) {
	int ret = 0;
	int80(20, mutex, &ret, 0, 0,0);
	return ret;
}

int waitMutex(int mutex) {
	int ret = 0;
	int80(21, mutex, &ret, 0, 0,0);
	return ret;
}
