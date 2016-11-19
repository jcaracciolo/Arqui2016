//
// Created by juanfra on 15/11/16.
//

#ifndef ARQUI2016_PIPE_H
#define ARQUI2016_PIPE_H

#include <stdint.h>
#include "condvar.h"
#define MAX_PIPES 50
#define PIPES_MUTEX "__PIPES__MUTEX"

typedef struct{
    char* buffer;
    int bufferSize;
    int initialIndex;
    int mutex;
    int readMutex;
    int writeMutex;
    condVar_t readCondVar;
    condVar_t writeCondVar;
    char* name;
}pipe_s;

typedef pipe_s* pipe_t;

pipe_t getPipe(char* name);
void releasePipe(char* name);
int writePipe(pipe_t pipe,char* msg , uint64_t amount);
int readPipe(pipe_t pipe,char* ans,uint64_t amount);
void initIPC();
#endif //ARQUI2016_PIPE_H
