//
// Created by juanfra on 15/11/16.
//

#include "include/pipe.h"
#include "include/buddyMemManager.h"
#include "include/lib.h"

pipe_t createPipe(){
    //Create mutex for this pipe
    pipe_t newPipe= malloc(sizeof(*newPipe));
    newPipe->buffer=buddyAllocatePages(1);
    newPipe->lastIndex=0;
    newPipe->initialIndex=0;

    return newPipe;
}

void deletePipe(pipe_t pipe){
    //release mutex
    buddyFree(pipe->buffer);
}
int writePipe(pipe_t pipe,char* msg, uint64_t amount){
    int i;
    for(i=0;i<amount;i++){
        pipe->buffer[pipe->lastIndex%MINPAGE]=msg[i];
        pipe->lastIndex= (pipe->lastIndex+1)%MINPAGE;
    }
    return 1;
}

int readPipe(pipe_t pipe,char* ans,uint64_t amount){
    int i,j;
    for(j=0,i=pipe->initialIndex;j<amount && i!=pipe->lastIndex;i++,j++){
        ans[j]=pipe->buffer[i%MINPAGE];
    }
    pipe->initialIndex=i;
    return 1;
}