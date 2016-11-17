//
// Created by juanfra on 15/11/16.
//

#ifndef ARQUI2016_PIPE_H
#define ARQUI2016_PIPE_H

#include <stdint.h>

#define MAX_PIPES 50
#define PIPES_MUTEX "__PIPES__MUTEX"

typedef struct{
    char* buffer;
    int lastIndex;
    int initialIndex;
    int mutex;
    char* name;
}pipe_s;

typedef pipe_s* pipe_t;

pipe_t getPipe(char* name);
void releasePipe(char* name);
int writePipe(pipe_t pipe,char* msg , uint64_t amount);
int readPipe(pipe_t pipe,char* ans,uint64_t amount);
void initIPC();
#endif //ARQUI2016_PIPE_H
