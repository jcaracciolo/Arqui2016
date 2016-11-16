#include "include/stdlib.h"
#include "include/shell.h"
#include "include/types.h"
#include "include/sync.h"

extern void int80(qword rdi, qword rsi, qword rdx, qword rcx, qword r8, qword r9);

int createMutex(char * name) {
	int mutex = 0;
	int80(23, name,(qword) &mutex, 0, 0,0);
	return mutex;
}

int releaseMutex(int mutex) {
	int ret = 0;
	int80(24, mutex, &ret, 0, 0,0);
	return ret;
}

int lockMutex(int mutex) {
	int ret = 0;
	int80(25, mutex,(qword) &ret, 0, 0, 0);
	return ret;
}

int unlockMutex(int mutex) {
	int ret = 0;
	int80(26, mutex,(qword) &ret, 0, 0, 0);
	return ret;
}
