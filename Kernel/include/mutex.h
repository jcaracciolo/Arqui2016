//
// Created by juanfra on 13/11/16.
//

#ifndef ARQUI2016_MUTEX_H
#define ARQUI2016_MUTEX_H


#include "IPCstructs.h"

int getMutex(char* name);
int releaseMutex(char* name);
int lockMutex(int mutex);
int unlockMutex(int mutex);

int tryScheduler();
int lockScheduler();
void unlockScheduler();

void initializeMutex();
void unlockAndSleep(int mutex, int pid);

int releaseMutexFromPos(int pos);

int getMuxeseNames(ipcs* ans, int cant);

#define MALLOCMUTEX "__MALLOC__MUTEX"
#define PAGESMUTEX "__PAGES__MUTEX"

#define MAX_MUTEXES 256
#define MAX_MUTEX_NAME_LENGHT 17
#define MAX_MUTEX_QUEUE_SIZE 30
#define MAX_MUTEX_PIDS 30

#endif //ARQUI2016_MUTEX_H
