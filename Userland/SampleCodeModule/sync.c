#include "include/stdlib.h"
#include "include/shell.h"
#include "include/types.h"
#include "include/sync.h"

extern void int80(qword rdi, qword rsi, qword rdx, qword rcx, qword r8, qword r9);

int createMutex(char * name) {
	int mutex = 0;
	int80(19, name,(qword) &mutex, 0, 0,0);
	return mutex;
}

int releaseMutex(int mutex) {
	int ret = 0;
	int80(20, mutex, &ret, 0, 0,0);
	return ret;
}

int lockMutex(int mutex) {
	int ret = 0;
	int80(21, LOCK, mutex,(qword) &ret, 0, 0);
	return ret;
}

int tryLockMutex(int mutex) {
	int ret = 0;
	int80(21, TRY_LOCK, mutex,(qword) &ret, 0, 0);
	return ret;
}

int unlockMutex(int mutex) {
	int ret = 0;
	int80(21, UNLOCK, mutex,(qword) &ret, 0, 0);
	return ret;
}
