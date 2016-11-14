//
// Created by juanfra on 13/11/16.
//

#include "include/mutex.h"
#include "include/testAndSet.h"

#define NULL  (char*)0
static char mutexNames[256][16];
static char mutexes[256];
static char freeSpace[256];
static lastIndex=0;

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
    for(i=0;i<15 && name!='\0';i++,name++){
        mutexNames[index][i]=*name;
    }
    mutexNames[index][i]='\0';
}
int whereIs(char* name){
    int i;
    for(i=0;i<lastIndex;i++){
        if(strcmp(name,mutexNames[i])==0)
            return i;
    }
    return -1;
}

int nextFree(){
    int i,j;
    for(i=lastIndex,j=0; j<256 && freeSpace[i]!=0  ; i=(i+1)%256 , j++);
    if(freeSpace[i]==0) return i;
    return -1;
}

int createMutex(char* name){
    if(lastIndex<0) return -1;
    if(*name=='\0') return -1;
    if(whereIs(name) != -1) return -1;

    int pos=nextFree();
    lastIndex=pos;
    if(pos<0) return -1;
    saveName(pos,name);
    mutexes[pos]=0;
    freeSpace[pos]=1;


    return pos;
}

int releaseMutex(int mutex){
        if(mutex<0 || mutex >=256) return -1;
        mutexNames[mutex][0]='\0';
        mutexes[mutex]=0;
        freeSpace[mutex]=0;
        if(lastIndex<0) lastIndex=mutex;
        return 0;
}

int waitMutex(int mutex){
    if(mutex<0 || mutex >=256) return -1;
            if(testAndSet(mutexes[mutex])==1){
                return 1;
            }else{
                //LOCK PROCESS, WAIT FOR THIS MUTEX
            }
    return 0;
}

