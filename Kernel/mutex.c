//
// Created by juanfra on 13/11/16.
//

#include "include/mutex.h"
#include "include/testAndSet.h"
#include "include/types.h"
#include "include/scheduler.h"
#include "include/process.h"
#include "include/videoDriver.h"
#define NULL  (char*)0


typedef struct {
    char name[MAX_MUTEX_NAME_LENGHT+1];
    int queue[MAX_MUTEX_QUEUE_SIZE];
    int usingPids[MAX_MUTEX_PIDS];
    qword mutex;
    qword waiting;
    qword using;
    int lastIndex;
    int firstIndex;
}mutex_t;

static mutex_t mutexes[MAX_MUTEXES];
static int savedMutexes=0;
static qword schedulerMutex=0;

void initializeMutex(){
    for(int i=0;i<MAX_MUTEXES;i++){
        mutexes[i].name[0]=='\0';
    }
}

void saveName(int index,char* name){
    if(index<0 || index>=MAX_MUTEX_NAME_LENGHT) return;
    int i;
    for(i=0;i<MAX_MUTEX_NAME_LENGHT && (*name)!='\0';i++,name++){
        mutexes[index].name[i]=*name;
    }
    mutexes[index].name[i]='\0';
}

/* FUNCIONES NO TESTEADAS */

/* Returns mutex ID if pid is waiting a mutex */
int isPidWaitingMutex(int pid) {
    int i;
    for(i=0;i<MAX_MUTEXES;i++) {
        if (isPidWaiting(mutexes[i], pid)) {
            return i;
        }
    }
    return -1;
}

int isPidWaiting(mutex_t mutex, int pid) {
    int i;
    for (i = 0; i < mutex.waiting; i++) {
        if (mutex.queue[i] == pid) {
            return 1;
        }
    }
    return 0;
}

/* FIN DE FUNCIONES NO TESTEADAS */

int whereIs(char* name){
    if(*name=='\0') return -1;
    int i;
    for(i=0;i<MAX_MUTEXES;i++){
        if(strcmp(name,mutexes[i].name)==0){
            return i;
        }
    }
    return -1;
}

int amIUsing(int index){

    if(index<0 || index>=MAX_MUTEX_NAME_LENGHT || mutexes[index].name[0]=='\0') return -1;

    int myPid=getCurrentPid();
    for(int i=0;i<mutexes[index].using;i++){
        if(myPid==mutexes[index].usingPids[i]){
            return i;
        }
    }
    return -1;
}

/*gets or Creates a mutex with the given Name*/
int getMutex(char* name){
    lockScheduler();

    if(savedMutexes == MAX_MUTEXES) return -1;
    if(*name=='\0') return -1;

    int pos = whereIs(name);

    if(pos==-1){
        /* Not Found */
        pos= nextfree();
        if(pos!=-1) {
            /* There is Place */
            mutex_t* m=&mutexes[pos];
            saveName(pos,name);
            m->mutex=0;

            m->usingPids[0]=getCurrentPid();
            m->using=1;

            m->waiting=0;
            m->lastIndex=0;
            m->firstIndex=0;

            savedMutexes++;

        }
    }else if(amIUsing(pos) == -1){

        if(mutexes[pos].using==MAX_MUTEX_PIDS){
            /*No More Place*/
            unlockScheduler();
            return -1;
        }



        /* Not Using it */
        mutexes[pos].usingPids[mutexes[pos].using]=getCurrentPid();
        mutexes[pos].using +=1;
    }

    unlockScheduler();
    return pos;
}



int nextfree(){
    if(savedMutexes == MAX_MUTEXES) return -1;
    int i;
    for(i=0;i<MAX_MUTEXES;i++){
        if(mutexes[i].name[0]=='\0'){
            return i;
        }
    }
    return -1;
}

/* release a mutex if it exists*/
int releaseMutex(char* name){
    lockScheduler();
    int pos=whereIs(name);
    if(pos!=-1){
        mutex_t* m=&mutexes[pos];
        if(m->using>1){
            int myPos=amIUsing(pos);
            m->usingPids[myPos]=-1;
            m->using-=1;
        }else{
            mutexes[pos].name[0]='\0';
            mutexes[pos].mutex=1;
            savedMutexes-- ;
        }
    }
    unlockScheduler();

    return 0;
}

// locks the mutex, blocks if the mutex is not available
int lockMutex(int mutex){
    if(mutex<0 || mutex >= savedMutexes || mutexes[mutex].name[0]=='\0') return -1;

    lockScheduler();

    if (! testAndSet(&(mutexes[mutex].mutex)) ) {

        mutex_t* m=&mutexes[mutex];
        if(m->waiting<MAX_MUTEX_QUEUE_SIZE){

            int myPid=getCurrentPid();
            m->waiting+=1;
            m->queue[m->lastIndex] = myPid;
            m->lastIndex = (m->lastIndex+1)%MAX_MUTEX_QUEUE_SIZE;
            changeProcessState(myPid,BLOCKED);
            unlockScheduler();
            _yield();
        }
    }

    unlockScheduler();
    return 1;
}


int unlockMutex(int mutex){
    if(mutex<0 || mutex >= savedMutexes || mutexes[mutex].name[0]=='\0') return -1;

    lockScheduler();

    mutex_t* m=&mutexes[mutex];

    if(m->waiting>0){

        m->waiting-=1;
        changeProcessState(m->queue[m->firstIndex],READY);
        m->firstIndex=(m->firstIndex+1)%MAX_MUTEX_QUEUE_SIZE;

    }

    if(m->waiting==0){
        m->mutex=0;
    }


    unlockScheduler();
    return 1;
}


int lockScheduler(){
    return testAndSet(&schedulerMutex);
}

void unlockScheduler(){
    schedulerMutex=0;
}

int tryScheduler(){
    return schedulerMutex;
}
