//
// Created by julian on 11/18/16.
//

#include "include/condvar.h"
#include "include/strings.h"
#include "include/mutex.h"
#include "include/process.h"
#include "include/scheduler.h"

void addToCondVarQueue(condVar_t * condVar, int pid);
int removeFromCondVarQueue(condVar_t * condVar);


void initCondVar(condVar_t * condVar){
    condVar->queueIndex=0;
    condVar->queueSize=0;
}

void waitCondVar(condVar_t * condVar, int mutex){
    lockScheduler();
    condVar->mutex = mutex;
    addToCondVarQueue(condVar,getCurrentPid());
    changeProcessState(getCurrentPid(),BLOCKED);
    unlockMutex(mutex);
    unlockScheduler();
    _yield();
    lockMutex(mutex);
}

void signalCondVar(condVar_t * condVar){
    int pid = removeFromCondVarQueue(condVar);
    if(pid != -1) changeProcessState(pid,READY);
//    _yield();
}

void addToCondVarQueue(condVar_t * condVar, int pid){
    if(condVar->queueSize == MAX_COND_VAR_QUEUE_SIZE) return;
    int index = (condVar->queueIndex + condVar->queueSize)%MAX_COND_VAR_QUEUE_SIZE;
    condVar->queue[index] = pid;
    condVar->queueSize ++;
}

int removeFromCondVarQueue(condVar_t * condVar){
    if(condVar->queueSize==0)return -1;
    int pid = condVar->queue[condVar->queueIndex];
    condVar->queueIndex = (condVar->queueIndex + 1) % MAX_COND_VAR_QUEUE_SIZE;
    condVar->queueSize --;
    return pid;
}