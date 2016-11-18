
#include "include/sync.h"
#include "include/stdio.h"
#include "include/stdlib.h"
#include "include/strings.h"
#include "include/stdvid.h"
#include "include/math.h"
#include "include/stdtime.h"

#define MAX_PHILOSOPHERS 50
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define DEAD 3
#define SCREEN_CENTER_X 450
#define SCREEN_CENTER_Y 350
#define TIME_SCALE 8000
#define THINKING_TO_EATING_RATIO 2

#define SAFE lockMutex(safeSpace)
#define UNSAFE unlockMutex(safeSpace)

int philState[MAX_PHILOSOPHERS];
int philMutex[MAX_PHILOSOPHERS];
int neighborsMutex;
int philAmount = 0;
int tableRadius = 200;
int philosopherRadius = 30;
int safeSpace;

void philosophize();
void addPhilosopher();
int leftFrom(int index);
int rightFrom(int index);
char * getMutexName(int philNumber);
void drawPhilosopher(int id, int status, int currPhil);
int getPhilosopherX(int id, int currPhil);
int getPhilosopherY(int id, int currPhil);
void killPhilosophers(int currAmount);
void drawMyself(int id);

void philosphers(){
    clear();
    neighborsMutex = createMutex("philosophersNeighbors");
    safeSpace = createMutex("philosophersSafeSpace");
    for(int i=0; i < 5; i++){
        addPhilosopher();
//        sleep(2000);
    }
    int c = 0;
    while (c != 'x'){
        c = getc();
        if(c!=-1) {
            switch (c) {
                case 'q':
                case 'Q':
                    addPhilosopher();
                    break;
                case 'e':
                case 'E':
                    removePhilosopher();
                    break;
            }
        }
    }
}

void removePhilosopher(){
    clear();
    philAmount--;
    drawPhilosophers(philAmount);
}

void addPhilosopher(){
    char* mutexName=getMutexName(philAmount);
//    printf("%s\n", mutexName);
    philMutex[philAmount]=createMutex(mutexName);
    philState[philAmount]=THINKING;
    printf("mutex %d\n", mutexName);
//    printf("Philo %d got %d fork",philAmount,));
//
    void** parg = (void**)malloc(sizeof(void*) * 2);
    parg[0] = mutexName;
    parg[1] = (void*)philAmount;
//    lockMutex(philMutex[philAmount]);
    exec(&philosophize,philAmount,parg, 1);
//    killPhilosophers(philAmount);
//    clear();
    lockMutex(neighborsMutex);
    philAmount++;
    unlockMutex(neighborsMutex);
    drawPhilosophers(philAmount);
}

void philosophize( int id){
    printf("Philo %d is born\n",id);
    int left;
    int right;
    int n;

    while (1){
        switch (philState[id]){
            case THINKING:
                drawMyself(id);
                sleep(randBound(TIME_SCALE*THINKING_TO_EATING_RATIO,2*TIME_SCALE*THINKING_TO_EATING_RATIO));
                philState[id] = HUNGRY;
            break;
            case HUNGRY:
                drawMyself(id);
                if(philAmount >1){
                    lockMutex(neighborsMutex);
                    left = leftFrom(id);
                    right = rightFrom(id);
                    unlockMutex(neighborsMutex);

                    if(id == philAmount -1) {
                        lockMutex(philMutex[right]);
                        lockMutex(philMutex[left]);
                    } else {
                        lockMutex(philMutex[left]);
                        lockMutex(philMutex[right]);
                    }
                    philState[id] = EATING;
                }
                break;
            case EATING:
                drawMyself(id);
                sleep(randBound(TIME_SCALE,2*TIME_SCALE));

                philState[id] = THINKING;
                drawMyself(id);
                unlockMutex(philMutex[right]);
                unlockMutex(philMutex[left]);
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
  char* base = "philo";
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

void killPhilosophers(int currPhil){
    if(currPhil == 0) return;
    int i;
    for(i=0;i<currPhil;i++){
        drawPhilosopher(i,DEAD,currPhil);
    }
}

void drawPhilosophers(int currPhil){
    if(currPhil == 0) return;
    int i;
    for(i=0;i<currPhil;i++){
        drawPhilosopher(i,philState[i],currPhil);
    }
}

int getPhilosopherX(int id, int currPhil){
    float angle = (M_2_PI / currPhil)*id;
    float res;
    cos(angle,&res);
    return res * tableRadius + SCREEN_CENTER_X;
}
int getPhilosopherY(int id, int currPhil){
    float angle = (M_2_PI / currPhil)*id;
    float res;
    sin(angle,&res);
    return res * tableRadius + SCREEN_CENTER_Y;
}

void drawMyself(int id){
    drawPhilosopher(id,philState[id],philAmount);
}


void drawPhilosopher(int id, int status, int currPhil){
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
        case DEAD:
            color = 0x00000000;
            break;
    }
    drawCFullCircle(getPhilosopherX(id,currPhil),getPhilosopherY(id,currPhil)
            ,philosopherRadius,color);
}