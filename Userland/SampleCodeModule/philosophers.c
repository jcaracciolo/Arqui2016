
#include "sync.h"
#include "stdio.h"
#include "stdlib.h"
#include "strings.h"

#define MAX_PHILOSOPHERS 50
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int philState[MAX_PHILOSOPHERS];
int philMutex[MAX_PHILOSOPHERS];
int philAmount = 0;

void philosophize();
void addPhilosopher();
int leftFrom(int index);
int rightFrom(int index);
char * getMutexName(int philNumber);

void philosphers(){
    int a = createMutex("asda");
    lockMutex(a);
    lockMutex(a);
//  addPhilosopher();
//  addPhilosopher();
}

void addPhilosopher(){
    printf("%s\n", getMutexName(philAmount));
    philMutex[philAmount]=createMutex(getMutexName(philAmount));
    printf("mutex %d\n", philMutex[philAmount]);
    printf("Philo %d got %d fork",philAmount,lockMutex(philMutex[0]));
    philAmount++;

//    exec(&philosophize,0, (void**) 0);
}

void philosophize(){
    printf("Philo  is born\n");
    int id = philAmount-1;
    int status = THINKING;
    int left;
    int right;
    int n;
    while (1){
        switch (status){
            case THINKING:
                printf("Philo %d is thinking",id);
                n = 4000;
                while (n--);
                status = HUNGRY;
            break;
            case HUNGRY:
                if(philAmount >1){
                    printf("Philo %d is HUNGRY",id);
                    left = leftFrom(id);
                    right = rightFrom(id);
                    if(id == philAmount-1) {
                        lockMutex(philMutex[right]);
                        printf("Philo %d got %d fork",id,right);
                        lockMutex(philMutex[left]);
                        printf("Philo %d got %d fork",id,left);
                    } else {
                        lockMutex(philMutex[left]);
                        printf("Philo %d got %d fork",id,left);
                        lockMutex(philMutex[right]);
                        printf("Philo %d got %d fork",id,right);
                    }
                    status = EATING;
                }
                break;
            case EATING:
                printf("Philo %d is eating",id);
                n = 2500;
                while (n--);
                unlockMutex(philMutex[left]);
                unlockMutex(philMutex[right]);
                status = THINKING;
            break;
        }
    }
}

int leftFrom(int index){
  return index;
}
int rightFrom(int index){
    if(index == philAmount-1) return 0;
    else return index + 1;
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
