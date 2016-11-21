
#include "include/sync.h"
#include "include/stdio.h"
#include "include/stdlib.h"
#include "include/strings.h"
#include "include/stdvid.h"
#include "include/math.h"
#include "include/philosophers.h"
#include "include/stdtime.h"

#define MAX_PHILOSOPHERS 40
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define DEAD 3
#define SCREEN_CENTER_X 450
#define SCREEN_CENTER_Y 400
#define TIME_SCALE 12000
#define THINKING_TO_EATING_RATIO 1
#define MAX_PHIL_RADIUS 50
#define NULL (void*)0

#define MIN_PHIL_RADIUS 5
#define SAFE lockMutex(safeSpace)

#define UNSAFE unlockMutex(safeSpace)
int philState[MAX_PHILOSOPHERS];
int philMutex[MAX_PHILOSOPHERS];

int philPID[MAX_PHILOSOPHERS];
int neighborsMutex;
int philAmount = 0;
int tableRadius = 275;
int philosopherRadius = 40;
int safeSpace;

boolean deleteLast;
void philosophize( int carg, void** args);
void addPhilosopher();
char * getMutexName(int philNumber);
void drawPhilosopher(int id, int status, int currPhil);
int getPhilosopherX(int id, int currPhil);
int getPhilosopherY(int id, int currPhil);
void killPhilosophers(int currAmount);
void drawMyself(int id);
void drawPhilosophers(int currPhil);
void seppuku(int id);
void deleteInstructionsPhilo();
void deletePhiloGrafics();

int getPhilRadius();

int left(int i);
int right(int i);
void takeForks(int id);
void putForks(int id);
void test(int i);

void takeForks(int id) {
    lockMutex(neighborsMutex);	//Crit zone
    //Set state
    philState[id] = HUNGRY;
    drawPhilosophers(philAmount);

    test(id);								//Try to acquire forks
    unlockMutex(neighborsMutex);		//Crit zone exit
    lockMutex(philMutex[id]);	//Locks if forks not acquired
}

void putForks(int id) {
    lockMutex(neighborsMutex);				//Crit zone

    //Set state
    philState[id] = THINKING;
    //Think and release forks
    drawPhilosophers(philAmount);

    test(left(id));							//Try to acquire forks for left
    test(right(id));						//Try to acquire forks for right
    unlockMutex(neighborsMutex);			//Crit zone exit
}

void test(int id) {
    if (philState[id] == HUNGRY &&				//Philosopher is hungry
        philState[left(id)] != EATING &&		//Both philosophers at
        philState[right(id)] != EATING) {		//left and right are not eating

        //Set state
        philState[id] = EATING;					//Philosopher can eat!
        //Take forks and eat

        drawPhilosophers(philAmount);

        unlockMutex(philMutex[id]);
    }
}

int left(int i) {
    return (i + philAmount - 1) % philAmount;
}

int right(int i) {
    return (i + 1) % philAmount;
}

void sendPhilosophersHome(){
    int i;
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
    deleteLast=0;
    neighborsMutex = createMutex("philoNeighbors");
    safeSpace = createMutex("philoSafeSpace");

    for(int i=0; i < 8; i++){
        addPhilosopher();
    }

    clear();
    drawPhilosophers(philAmount);
    printf("Welcome the philosophers! They will be starving for your entertainment.\n");
    printf("Press 'q' to refresh the screen.\nPress 'a' to add philosophers and 'd' to remove them.\n");
    printf("Press 'e' to EXIT and let the philosophers continue with their nihilist thoughts.\n");

    while(1) {
        int c = getc();
        if (c != EOF) {
            if (c == 'q') {
                clear();
                drawPhilosophers(philAmount);
                printf("Welcome the philosophers! They will be starving for your entertainment.\n");
                printf("Press 'q' to refresh the screen.\nPress 'a' to add philosophers and 'd' to remove them.\n");
                printf("Press 'e' to EXIT and let the philosophers continue with their nihilist thoughts.\n");

            } else if (c == 'a') {
                deleteInstructionsPhilo();
                if(philAmount >= MAX_PHILOSOPHERS){
                    printf("There is not enough food for the philosophers.\nThey do not like sharing.\n");
                    printf("You cant add more philosophers.");
                } else{
                    printf("Adding philosopher, please wait.\n");
                    addPhilosopher();
                    printf("Philosopher added! Total: %d\n",philAmount);
                }
            } else if (c == 'd') {
                deleteInstructionsPhilo();
                printf("Removing philosopher, please wait.\nPhilosophers are prone to steal forks when they leave.\n");
                printf("We have to be careful when we let them go.\n");
                removePhilosopher();
            }else if (c == 'e') {
                sendPhilosophersHome();
                clear();
                return;
            }
        }
        sleep(100);
    }
}

void removePhilosopher(){
    deleteLast = 1;
    lockMutex(safeSpace);
    lockMutex(neighborsMutex);
//    if(philState[philAmount-1] == THINKING){
//        kill(philPID[philAmount-1],2);
//    }
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


//    killPhilosophers(philAmount);
    philAmount++;
    philosopherRadius = getPhilRadius();
    deletePhiloGrafics();
    drawPhilosophers(philAmount);
    unlockMutex(neighborsMutex);
    if(philAmount!=1) unlockMutex(philMutex[0]);
}

int getPhilRadius(){
    int x = (2*3.14*tableRadius)/(philAmount*2);
    if(x > MAX_PHIL_RADIUS) return MAX_PHIL_RADIUS;
    if( x < MIN_PHIL_RADIUS) return MIN_PHIL_RADIUS;
    return x;
}

void philosophize( int carg, void** args){
    int id=args[1];

    while (1) {
//        lockMutex(safeSpace);
        sleep(randBound(TIME_SCALE*THINKING_TO_EATING_RATIO,2*TIME_SCALE*THINKING_TO_EATING_RATIO));
        takeForks(id);

        if(id==philAmount-1 && deleteLast == 1 ){
            seppuku(id);
        }

        sleep(randBound(TIME_SCALE*THINKING_TO_EATING_RATIO,2*TIME_SCALE*THINKING_TO_EATING_RATIO));
        putForks(id);
    }
}

void seppuku(int id){

//    printf("killing philo %d, pid:%d\n",id,philPID[id]);
    lockMutex(neighborsMutex);
    int pleft=left(id);
    int pright=right(id);
    philAmount--;
    deletePhiloGrafics();
    philosopherRadius = getPhilRadius();

    drawPhilosophers(philAmount);
    deleteLast = 0;
    unlockMutex(philMutex[pleft]);
    unlockMutex(philMutex[pright]);
    releaseMutex(pleft>pright?philMutex[pleft]:philMutex[pright]);
    unlockMutex(neighborsMutex);
//    printf("killing %d, philo %d",getPID(),philAmount+1);
    printf("Kicked philo %d out safely\n",id);
    kill(philPID[id],0);
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
void deletePhiloGrafics(){
    drawCSquare(0,SCREEN_CENTER_Y-tableRadius-philosopherRadius,(tableRadius+philosopherRadius)*2+20,1000,0x000000);
}