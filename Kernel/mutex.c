//
// Created by juanfra on 13/11/16.
//

#include "include/mutex.h"
#include "include/testAndSet.h"
#include "include/types.h"

#define MAX_MUTEXES 256
#define MAX_MUTEX_NAME_LENGHT 16
#define NULL  (char*)0
static char mutexNames[MAX_MUTEXES][MAX_MUTEX_NAME_LENGHT];
static qword mutexes[MAX_MUTEXES];

static int savedMutexes=0;

int strcmp(const char* str1, const char* str2) {
    while (*str1 != '\0') {
        if (*str1 - *str2) return *str1 - *str2;
        str1++; str2++;
    }
    if (*str2 == '\0') return 0;
    return -1;
}

void saveName(int index,char* name){
    int i;
    for(i=0;i<MAX_MUTEX_NAME_LENGHT && name!='\0';i++,name++){
        mutexNames[index][i]=*name;
    }
    mutexNames[index][i]='\0';
    savedMutexes++;
}

int whereIs(char* name){
    int i;
    for(i=0;i<savedMutexes;i++){
        if(strcmp(name,mutexNames[i])==0)
            return i;
    }
    return -1;
}

int createMutex(char* name){
    if(savedMutexes == MAX_MUTEXES) return -1;
    if(*name=='\0') return -1;
    int pos = whereIs(name);
    if(pos != -1) return pos;

    pos = savedMutexes;
    saveName(pos,name);
    mutexes[pos]=0;

    return pos;
}

int releaseMutex(int mutex){
        if(mutex<0 || mutex >= savedMutexes) return -1;
        if(mutex == savedMutexes -1){
            mutexNames[mutex][0] = '\0';
            mutexes[mutex]=0;
        } else {
            saveName(mutex,mutexNames[savedMutexes-1]);
            mutexes[mutex] = mutexes[savedMutexes-1];
            mutexNames[savedMutexes-1][0] = '\0';
            mutexes[savedMutexes-1]=0;
        }

        savedMutexes -= 1;
        return 0;
}

// 	tries to lock the mutex, returns if the mutex is not available
int tryLockMutex(int mutex){
    if(mutex<0 || mutex >= savedMutexes) return -1;
    if (testAndSet(&(mutexes[mutex]))==1) {
        return 1;
    }
    else return testAndSet(mutexes[mutex]);
}

// locks the mutex, blocks if the mutex is not available
int lockMutex(int mutex){
    if(mutex<0 || mutex >= savedMutexes) return -1;
    if (testAndSet(&(mutexes[mutex]))==1) {
        return 1;
    } else {
        //tellSchedulerThatYouAreWaitingForThisMutex(mutex);
        return 1;
    }
}


int unlockMutex(int mutex){
    if(mutex<0 || mutex >= savedMutexes) return -1;
    mutexes[mutex] = 0;
    //wakeUpProcessesThatNeedThisMutexToContinue(mutex);
    return 1;
}
