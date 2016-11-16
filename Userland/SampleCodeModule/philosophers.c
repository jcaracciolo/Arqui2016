
#include "include/sync.h"
#include "include/stdio.h"
#include "include/stdlib.h"
#include "include/strings.h"
#include "include/stdvid.h"
#include "include/math.h"

#define MAX_PHILOSOPHERS 50
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define SCREEN_CENTER_X 350
#define SCREEN_CENTER_Y 350

int philState[MAX_PHILOSOPHERS];
int philMutex[MAX_PHILOSOPHERS];
int philAmount = 0;
int tableRadius = 200;
int philosopherRadius = 30;

void philosophize();
void addPhilosopher();
int leftFrom(int index);
int rightFrom(int index);
char * getMutexName(int philNumber);
void drawPhilosopher(int id, int status);
int getPhilosopherX(int id);

void philosphers(){
    philAmount = 15;

    for(int i=0; i < philAmount; i++){
        drawPhilosopher(i, i%3);
    }
    int a = createMutex("asda");
    lockMutex(a);
    unlockMutex(a);
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



void drawPhilosopher(int id, int status){
    qword color = 0x00AAAAAA;
    switch (status){
        case THINKING:
            color = 0x00FFFF00;
            break;
        case HUNGRY:
            color = 0x00B22222;
            break;
        case EATING:
            color = 0x00BA55D3;
            break;
    }
    drawCFullCircle(getPhilosopherX(id),getPhilosopherY(id),philosopherRadius,color);
//    drawCSquare(getPhilosopherX(id),40,100,100,0x00AAAAAA);
}


int getPhilosopherX(int id){
    float angle = (M_2_PI / philAmount)*id;
    float res;
    cos(angle,&res);
    return res * tableRadius + SCREEN_CENTER_X;
}
int getPhilosopherY(int id){
    float angle = (M_2_PI / philAmount)*id;
    float res;
    sin(angle,&res);
    return res * tableRadius + SCREEN_CENTER_Y;
}


