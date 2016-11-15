
#include "sync.h"
#include "stdio.h"
#include "stdlib.h"

#define MAX_PHILOSOPHERS 50
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int philState[MAX_PHILOSOPHERS];
int philMutex[MAX_PHILOSOPHERS];
int philAmount = 0;
char * getMutexName(int philNumber);
void addPhilosopher();

int philosphers(){
  addPhilosopher();
  addPhilosopher();
  addPhilosopher();
  addPhilosopher(); 
}

void addPhilosopher(){
    printf("Hey there\n");
    printf("%s\n", getMutexName(philAmount));
    philAmount++;
}

char * getMutexName(int philNumber){
  char * mutexName = (char*) malloc(16);
  char base[] = "philo";
  if(philAmount > 9) {
    strcpy(mutexName,base,5);
    mutexName[5] = (philNumber/10)+'0';
    mutexName[6] = (philNumber%10)+'0';
    mutexName[7] = '\0';
  }else {
    strcpy(mutexName,base,5);
    mutexName[5] = philNumber+'0';
    mutexName[6] = '\0';
  }
  return mutexName;
}
