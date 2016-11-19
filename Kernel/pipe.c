//
// Created by juanfra on 15/11/16.
//

#include "include/pipe.h"
#include "include/buddyMemManager.h"
#include "include/lib.h"
#include "include/strings.h"
#include "include/mutex.h"
#include "include/scheduler.h"
#include "include/videoDriver.h"

static pipe_t pipes[MAX_PIPES];
static int pipeMutex;


void initIPC(){
    for (int i = 0; i < MAX_PIPES; ++i) {
        pipes[i]=(pipe_t )0;
    }
    pipeMutex = getMutex(PIPES_MUTEX);
}

pipe_t createPipe(char* name){
    char* mname[16]={'p','_','\0'};
    strcpy(mname+2,name,13);
    mname[15]='\0';
    pipe_t newPipe= malloc(sizeof(*newPipe));

    newPipe->mutex=getMutex(mname);
    newPipe->buffer=buddyAllocatePages(1);
    newPipe->bufferSize=0;
    newPipe->initialIndex=0;

    newPipe->name=malloc(15);

    mname[0]='R';
    newPipe->readMutex=getMutex(mname);
    mname[0]='W';
    newPipe->writeMutex=getMutex(mname);

    initCondVar(&(newPipe->readCondVar));
    initCondVar(&(newPipe->writeCondVar));

    strcpy(newPipe->name,name,15);
    return newPipe;
}

int addPipe(pipe_t p){
    if(p==0) return 0;

    process* me=getMyProcessData();
    int i;
    for (i = 0; i < 5; ++i) {
        if(me->fd[i]==0){
            me->fd[i]=p;
            break;
        }else if(me->fd[i] == p){
            break;
        }
    }
    return (i==5)?-1:i;
}

pipe_t getPipe(char* name){
    lockMutex(pipeMutex);

    int pos=whereIsPipe(name);
    if(pos!=-1){
        unlockMutex(pipeMutex);
        return pipes[pos];
    }

    pos = nextfreePipe(name);
    if(pos!=-1){
        pipes[pos]=createPipe(name);
        unlockMutex(pipeMutex);
        return pipes[pos];
    }
    unlockMutex(pipeMutex);
    return (pipe_t)0;
}

int whereIsPipe(char* name){
    for (int i = 0; i < MAX_PIPES; ++i) {
        if(pipes[i]!=0) {
            if (strcmp(pipes[i]->name, name) == (pipe_t)0)
                return i;
        }
    }
    return -1;
}

void releasePipe(char* name){
    lockMutex(pipeMutex);
    int pos = whereIsPipe(name);
    if(pos!=-1){
        deletePipe(pipes[pos]);
        pipes[pos]=0;
    }
    unlockMutex(pipeMutex);
}

int nextfreePipe(char* name){
    int i;
    for(i=0;i<MAX_PIPES;i++){
        if(pipes[i]==0){
            return i;
        }
    }
    return -1;
}

void deletePipe(pipe_t pipe){
    free(pipe->name);
    buddyFree(pipe->buffer);
    free(pipe);
}

int writePipe(pipe_t pipe,char* msg, uint64_t amount){
    int i;
    lockMutex(pipe->writeMutex);
    while (pipe->bufferSize >= MINPAGE){
        waitCondVar(&pipe->writeCondVar,pipe->writeMutex);
    }
    lockMutex(pipe->mutex);
    for(i=0;i<amount;i++){
        pipe->buffer[(pipe->initialIndex + pipe->bufferSize) %MINPAGE]=msg[i];
        pipe->bufferSize ++;
    }
    signalCondVar(&pipe->readCondVar);
    unlockMutex(pipe->mutex);
    unlockMutex(pipe->writeMutex);
    return 1;
}

int readPipe(pipe_t pipe,char* ans,uint64_t amount){
    int j,i;
    lockMutex(pipe->readMutex);
    while (pipe->bufferSize <= 0){
        waitCondVar(&pipe->readCondVar,pipe->readMutex);
    }

    lockMutex(pipe->mutex);
    for(j=0;j<amount && pipe->bufferSize>0;j++){
        ans[j]=pipe->buffer[pipe->initialIndex%MINPAGE];
        pipe->initialIndex = (pipe->initialIndex + 1)%MINPAGE;
        pipe->bufferSize--;

    }
    signalCondVar(&pipe->writeCondVar);
    unlockMutex(pipe->mutex);

    unlockMutex(pipe->readMutex);
    return 1;
}