
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
#define TIME_SCALE 12000
#define THINKING_TO_EATING_RATIO 1

#define SAFE lockMutex(safeSpace)
#define UNSAFE unlockMutex(safeSpace)

int philState[MAX_PHILOSOPHERS];
int philMutex[MAX_PHILOSOPHERS];
int neighborsMutex;
int philAmount = 0;
int tableRadius = 200;
int philosopherRadius = 30;
int safeSpace;
int step;

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
void drawPhilosophers(int currPhil);
//void philosphersSO();


void monitor(){
    int i,cons=0;
    printf("monitoring");
    while (1){

        for(i=0;i<philAmount;i++){
            if(philState[i] == EATING) {
                if(cons > 0){
                    printStatus();
                    sleep(12000);
                }
                cons++;
            } else cons = 0;
        }
    }
}

void printStatus(){
    int i;
    for(i=0;i<philAmount;i++){
        printf("(%d)",philState[i]);
    }
}
void philosphers(){
    clear();
    neighborsMutex = createMutex("philoNeighbors");
    safeSpace = createMutex("philoSafeSpace");
    lockMutex(safeSpace);

    void** parg = (void**)malloc(sizeof(void*));
    parg[0] = (void*)"gedit";
    exec(&monitor, 1, parg, 0);

    for(int i=0; i < 8; i++){
//        drawPhilosopher(i,HUNGRY,20);
        addPhilosopher();
        //sleep(2000);
    }
//    return;
    step = 1;
    while(1) {
        int c = getc();
        unlockMutex(safeSpace);
        //       if (c != EOF) {
            if (c == 'q') {
                clear();
                drawPhilosophers(philAmount);
            }
//            } else if (c == '\b') {
//            } else {
//                printf("%c",c);
//            }
//        }
    }
}



void removePhilosopher(){
    clear();
    philAmount--;
    drawPhilosophers(philAmount);
}

void addPhilosopher(){
    if(philAmount!=0) lockMutex(philMutex[0]);

    char* mutexName=getMutexName(philAmount);
    philMutex[philAmount]=createMutex(mutexName);
    philState[philAmount]=THINKING;
//    clear();
    void** parg = (void**)malloc(sizeof(void*) * 2);
    printf("%s\n",mutexName);
    printf("mutex %d is %d\n",philAmount,philMutex[philAmount]);
    parg[0] = (void*)mutexName;
    parg[1] = (void*)philAmount;
    lockMutex(neighborsMutex);
    exec(&philosophize,2,parg, 0);


    killPhilosophers(philAmount);
    philAmount++;
    unlockMutex(neighborsMutex);
    drawPhilosophers(philAmount);

    if(philAmount!=1) unlockMutex(philMutex[0]);
}


void philosophize( int carg, void** args){
    int id=args[1];
    printf("Philo %d is born\n",id);
    int left;
    int right;
    int n;

    while (1){

//        lockMutex(safeSpace);

        switch (philState[id]){
            case THINKING:
                drawMyself(id);
                sleep(randBound(TIME_SCALE*THINKING_TO_EATING_RATIO,2*TIME_SCALE*THINKING_TO_EATING_RATIO));
                philState[id] = HUNGRY;
                drawMyself(id);
//                printf("%d is hungry \n", id);

                break;
            case HUNGRY:
                drawMyself(id);
                if(philAmount >1){
//                    lockMutex(neighborsMutex);
                    left = leftFrom(id);
                    right = rightFrom(id);
//                    unlockMutex(neighborsMutex);

                    if(1) {
//                        printf("%d try loc %d, %d\n",id, right,left);
//                        printf("%d try left locked %d\n", id,left);
                        lockMutex(philMutex[left]);
//                        printf("%d left locked %d\n", id,left);
                        lockMutex(philMutex[right]);
//                        printf("%d right locked %d\n", id,right);
                    } else {
//                        printf("%d try right locked %d\n", id,right);
                        lockMutex(philMutex[right]);
//                        printf("%d right locked %d\n", id,right);
                        lockMutex(philMutex[left]);
//                        printf("%d left locked %d\n", id,left);
                    }
//                    printf("%d is eating \n", id);
                    philState[id] = EATING;
                    drawMyself(id);

                }
                break;
            case EATING:
                drawMyself(id);
                sleep(randBound(TIME_SCALE,2*TIME_SCALE));

                philState[id] = THINKING;
                drawMyself(id);
//                printf("%d is thinkin \n", id);
                unlockMutex(philMutex[left]);
                unlockMutex(philMutex[right]);
//                printf("%d released %d and %d\n", id,right,left);
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


////COPIED PHILOSOPHER IMPLEMENTATION FROM SO
//int mutexSo;
//
//void addPhiloSO(){
//    char* mutexName=getMutexName(philAmount);
//    void** parg = (void**)malloc(sizeof(void*) * 2);
//    parg[0] = (void*)mutexName;
//    parg[1] = (void*)philAmount;
//    exec(&philosphizeSO,2,parg, 0);
//    philAmount++;
//
//}
//
//void philosphersSO(){
//    mutexSo = createMutex("neighBors");
//    philAmount=0;
//    int i;
//
//    for(i=0;i<7;i++){
//        addPhiloSO();
//    }
//}
//
//void philosphizeSO(int id){
//    while(1) {
//        //Think
//        //sleep(10);
//        sleep(randBound(4000, 8000));
//
//        takeForks(id);
//
//        //Eat
//        //sleep(10);
//        sleep(randBound(4000, 8000));
//
//        putForks(id);
//    }
//}