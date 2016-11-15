//
// Created by juanfra on 15/11/16.
//

#ifndef ARQUI2016_PIPE_H
#define ARQUI2016_PIPE_H

#include <stdint.h>

typedef struct{
    char* buffer;
    int lastIndex;
    int initialIndex;
}pipe_s;

typedef pipe_s* pipe_t;

pipe_t createPipe();
void deletePipe(pipe_t pipe);
int writePipe(pipe_t pipe,char* msg , uint64_t amount);
int readPipe(pipe_t pipe,char* ans,uint64_t amount);
#endif //ARQUI2016_PIPE_H
