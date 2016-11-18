//
// Created by julian on 11/18/16.
//

#ifndef ARQUI2016_CONDVAR_H
#define ARQUI2016_CONDVAR_H

#define MAX_COND_VAR_NAME_LENGHT 50
#define MAX_COND_VAR_QUEUE_SIZE 50
#define MAX_COND_VAR 30


typedef struct {
    int queue[MAX_COND_VAR_QUEUE_SIZE];
    int queueSize;
    int queueIndex;
    int mutex;
}condVar_t;

void initCondVar(condVar_t * condVar);
void waitCondVar(condVar_t * condVar, int mutex, int pid);
void signalCondVar(condVar_t * condVar);

#endif //ARQUI2016_CONDVAR_H
