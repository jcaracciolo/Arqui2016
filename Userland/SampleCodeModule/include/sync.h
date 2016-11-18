//
// Created by juanfra on 13/11/16.
//

#ifndef ARQUI2016_SYNC_H
#define ARQUI2016_SYNC_H

#define MAX_COND_VAR_QUEUE_SIZE 50

int createMutex(char* name);

int lockMutex(int mutex);

int unlockMutex(int mutex);

int releaseMutex(int mutex);


typedef struct {
    int queue[MAX_COND_VAR_QUEUE_SIZE];
    int queueSize;
    int queueIndex;
    int mutex;
}condVar_t;

void initCondVar(condVar_t * condVar);

void waitCondVar(condVar_t * condVar, int mutex);

void signalCondVar(condVar_t * condVar);

#endif //ARQUI2016_SYNC_H
