
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
#define SCREEN_CENTER_Y 400
#define TIME_SCALE 12000
#define THINKING_TO_EATING_RATIO 1

#define SAFE lockMutex(safeSpace)
#define UNSAFE unlockMutex(safeSpace)

int philState[MAX_PHILOSOPHERS];
int philMutex[MAX_PHILOSOPHERS];
int philPID[MAX_PHILOSOPHERS];

int neighborsMutex;
int philAmount = 0;
int tableRadius = 200;
int philosopherRadius = 40;
int safeSpace;
boolean deleteLast;

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
void seppuku(int id,int left, int right);
void deleteInstructionsPhilo();


void monitor(){
    int i,cons=0;
//    printf("monitoring");
    while (1){

        for(i=0;i<philAmount;i++){
            if(philState[i] == EATING) {
                if(cons > 0){
//                    printStatus();
                    sleep(12000);
                    printError("There are 2 philosophers sharing!\n");
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

void sendPhilosophersHome(int monitorPID){
    int i;
    kill(monitorPID,0);
    lockMutex(neighborsMutex);
    for (i=0;i<philAmount;i++){
        kill(philPID[i],0);
    }
    for (i=0;i<philAmount;i++){
        releaseMutex(philMutex[i]);
    }
    unlockMutex(neighborsMutex);
    releaseMutex(neighborsMutex);
    releaseMutex(safeSpace);
    philAmount=0;
}

void philosphers(){
    clear();
    philAmount=0;
    deleteLast=false;
    neighborsMutex = createMutex("philoNeighbors");
    safeSpace = createMutex("philoSafeSpace");

    void** parg = (void**)malloc(sizeof(void*));
    parg[0] = (void*)"philosopherMonitor";
    int monitorPID = exec(&monitor, 1, parg, 0);

    for(int i=0; i < 8; i++){
        addPhilosopher();
    }

    clear();
    drawPhilosophers(philAmount);
    printf("Welcome the philosophers! They will be starving for your entertainment.\n");
    printf("Press 'q' to refresh the screen.\n Press 'a' to add philosophers and 'd' to remove them.\n");
    printf("Press 'e' to EXIT and let the philosophers continue with their nihilist thoughts.\n");

    while(1) {
        int c = getc();
        if (c != EOF) {
            if (c == 'q') {
                clear();
                drawPhilosophers(philAmount);
                printf("Welcome the philosophers! They will be starving for your entertainment.\n");
                printf("Press 'q' to refresh the screen.\n Press 'a' to add philosophers and 'd' to remove them.\n");
                printf("Press 'e' to EXIT and let the philosophers continue with their nihilist thoughts.\n");

            } else if (c == 'a') {
                deleteInstructionsPhilo();
                printf("Adding philosopher, please wait.\n");
                addPhilosopher();
                printf("Philosopher added! Total: %d\n",philAmount);
            } else if (c == 'd') {
                deleteInstructionsPhilo();
                printf("Removing philosopher, please wait.\nPhilosophers are prone to steal forks when they leave.\n");
                printf("We have to be careful when we let them go.\n");
                removePhilosopher();
            }else if (c == 'e') {
                sendPhilosophersHome(monitorPID);
                clear();
                return;
            }
        }
        sleep(100);
    }
}



void removePhilosopher(){
    deleteLast = true;
    lockMutex(safeSpace);
    lockMutex(neighborsMutex);
    if(philState[philAmount-1] == THINKING){
        kill(philPID[philAmount-1],2);
    }
    unlockMutex(neighborsMutex);
    unlockMutex(safeSpace);
}

void addPhilosopher(){
    if(philAmount!=0) lockMutex(philMutex[0]);
    lockMutex(neighborsMutex);

    char* mutexName=getMutexName(philAmount);
    philMutex[philAmount]=createMutex(mutexName);
    philState[philAmount]=THINKING;
//    clear();
    void** parg = (void**)malloc(sizeof(void*) * 2);
//    printf("%s\n",mutexName);
//    printf("mutex %d is %d\n",philAmount,philMutex[philAmount]);
    parg[0] = (void*)mutexName;
    parg[1] = (void*)philAmount;
    philPID[philAmount]=exec(&philosophize,2,parg, 0);


    killPhilosophers(philAmount);
    philAmount++;
    drawPhilosophers(philAmount);
    unlockMutex(neighborsMutex);

    if(philAmount!=1) unlockMutex(philMutex[0]);
}


void philosophize( int carg, void** args){
    int id=args[1];
    //printf("Philo %d is born pid: %d\n",id,philPID[id]);
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
                lockMutex(safeSpace);
                drawMyself(id);

                break;
            case HUNGRY:
                drawMyself(id);
                unlockMutex(safeSpace);
                if(philAmount >1){
                    lockMutex(neighborsMutex);
                    left = leftFrom(id);
                    right = rightFrom(id);
                    unlockMutex(neighborsMutex);

                    if(id == philAmount -1) {
                        lockMutex(philMutex[left]);
                        lockMutex(philMutex[right]);

                        if(deleteLast == true) seppuku(id,left,right);
                    } else {
                        lockMutex(philMutex[right]);
                        lockMutex(philMutex[left]);
                    }
                    philState[id] = EATING;
                    drawMyself(id);
                }
                break;
            case EATING:
                drawMyself(id);
                sleep(randBound(TIME_SCALE,2*TIME_SCALE));

                philState[id] = THINKING;
                drawMyself(id);
                unlockMutex(philMutex[left]);
                unlockMutex(philMutex[right]);
                break;
        }
    }
}

void seppuku(int id, int left, int right){

//    printf("killing philo %d, pid:%d\n",id,philPID[id]);
    lockMutex(neighborsMutex);
    killPhilosophers(philAmount);
    philAmount--;
    drawPhilosophers(philAmount);
    deleteLast = false;
    unlockMutex(philMutex[left]);
    unlockMutex(philMutex[right]);
    releaseMutex(left>right?philMutex[left]:philMutex[right]);
    unlockMutex(neighborsMutex);
//    printf("killing %d, philo %d",getPID(),philAmount+1);
    printf("Kicked philo %d out safely\n",id);
    kill(philPID[id],0);
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

void deleteInstructionsPhilo(){
    setCursorPos(0);
    drawCSquare(0,0,SCREEN_CENTER_Y-tableRadius-philosopherRadius,1000,0x000000);
}
