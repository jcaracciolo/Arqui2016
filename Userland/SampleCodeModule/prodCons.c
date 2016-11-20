//
// Created by julian on 11/18/16.
//

#include "include/prodCons.h"
#include "include/sync.h"
#include "include/stdio.h"
#include "include/stdlib.h"
#include "include/stdtime.h"
#include "include/math.h"
#include "include/stdvid.h"

#define MAX_PRODUCTS_PER_ROW 6
#define MAX_PRODUCTS_ROWS 5
#define MAX_PRODUCT_QUEUE_SIZE (MAX_PRODUCTS_ROWS * MAX_PRODUCTS_PER_ROW)
#define WIDTH 800
#define HEIGHT 700
#define XOFFSET 150
#define YOFFSET 150
#define PACMAN_SIZE 30
#define PACMAN_XOFFSET -30
#define PACMAN_YOFFSET 30
#define GHOST_SIZE 40
#define TIME_SCALE 100
#define MAX_REST 50

void producer();
void consumer();
void addToProductQueue(int productNum);
int removeFromProductQueue();
int getXFromQueue(int index);
int getYFromQueue(int index);
void animateGhost(int index, int status);
void drawAnimatedPacman(int index, int status);
void drawFullGhost(int index, int status);
void deleteGhost(int index);
void animate();
void exitProducerConsumer(int prodPID, int consPID, int tickPID);
void deletePacman(int index);
void deleteInstructions();



static int queue[MAX_PRODUCT_QUEUE_SIZE];
static int queueSize;
static int queueIndex;
static condVar_t readCondVar;
static condVar_t writeCondVar;
static int readMutex;
static int writeMutex;
static int modifyQueueMutex;
static int frame;
static int producerRest;
static int consumerRest;


#define PRODCONSREAD "prodConsRead"
#define PRODCONSWRITE "prodConsWrite"
#define PRODCONSMODIFY "prodConsModify"

void producerConsumer(){
    sleep(200);
    printf("hi there");

    queueSize = 0;
    queueIndex =0;
    readMutex = createMutex(PRODCONSREAD);
    writeMutex= createMutex(PRODCONSWRITE);
    modifyQueueMutex= createMutex(PRODCONSMODIFY);
    initCondVar(&readCondVar);
    initCondVar(&writeCondVar);
    clear();
    drawAnimatedPacman(queueIndex,frame);
    consumerRest = MAX_REST / 2;
    producerRest = MAX_REST / 2;

    void** parg = (void**)malloc(sizeof(void*));
    parg[0] = (void*)"producer";
    int prodID = exec(&producer,1,parg, 0);
    parg[0] = (void*)"consumer";
    int consID = exec(&consumer,1,parg, 0);
    parg[0] = (void*)"pacmanTick";
    int tickID = exec(&animate,1,parg, 0);




    deleteInstructions();
    printf("Welcome the PACMAN! His bloodlust for ghost ectoplasm is endless.\n");
    printf("Press 'q' to refresh the screen.\n Press 'd' to speed up ghost breeding and 'a' slow it down\n");
    printf("Press 'c' to speed up PACMANS hunger and 'z' slow it down\n");
    printf("Press 'e' to EXIT the PACMAN feast\n");
    while(1) {
        int c = getc();
        if (c != EOF) {
            if (c == 'q') {
                deleteInstructions();
                printf("Welcome the PACMAN! His bloodlust for ghost ectoplasm is endless.\n");
                printf("Press 'q' to refresh the screen.\n Press 'd' to speed up ghost breeding and 'a' slow it down\n");
                printf("Press 'c' to speed up PACMANS hunger and 'z' slow it down\n");
                printf("Press 'e' to EXIT the PACMAN feast\n");
            } else if (c == 'd') {
                deleteInstructions();
                printf("Encouraging ghosts...\n");
                if(producerRest>=2) producerRest--;
                else printf("MAX speed reached");
            } else if (c == 'a') {
                deleteInstructions();
                printf("Scaring ghosts...\n");
                if(producerRest<MAX_REST) producerRest++;
                else printf("MIN speed reached");
            } else if (c == 'c') {
                deleteInstructions();
                printf("Giving pacman more pills...\n");
                if(consumerRest>=2) consumerRest--;
                else printf("MAX speed reached");

            } else if (c == 'z') {
                deleteInstructions();
                printf("Slowing pacman down...\n");
                if(consumerRest< MAX_REST) consumerRest++;
                else printf("MIN speed reached");

            } else if (c == 'e') {
                printf("Closing down, please wait\n");
                exitProducerConsumer(prodID,consID,tickID);
                clear();
                return;
            }
        }
    }
}

void exitProducerConsumer(int prodPID, int consPID, int tickPID){
    kill(prodPID,0);
    kill(consPID,0);
    kill(tickPID,0);
    releaseMutex(writeMutex);
    releaseMutex(readMutex);
    releaseMutex(modifyQueueMutex);
    return;
}


void producer(){
    printf("hi, i am a producer\n");

    int i = 0;
    lockMutex(writeMutex);
    int maxProduced = 50;
    while (i < maxProduced){
        while (queueSize >= MAX_PRODUCT_QUEUE_SIZE){
            waitCondVar(&writeCondVar,writeMutex);
        }

        sleep(randBound(producerRest*TIME_SCALE,(producerRest+1)*TIME_SCALE));

        addToProductQueue(i);
//        printf("prod %d\n",i);
        signalCondVar(&readCondVar);
    }
    unlockMutex(writeMutex);
//    printf("releasing size: %d, index: %d\n",queueSize,queueIndex);
}

void consumer(){

    lockMutex(readMutex);
    printf("hi, i am a consumer\n");
    while (1){
        while (queueSize <= 0){
            waitCondVar(&readCondVar,readMutex);
        }

        sleep(randBound(consumerRest*TIME_SCALE,(consumerRest+1)*TIME_SCALE));

        removeFromProductQueue();
        signalCondVar(&writeCondVar);

    }
    unlockMutex(readMutex);
}


void addToProductQueue(int productNum){
    lockMutex(modifyQueueMutex);
    if(queueSize == MAX_PRODUCT_QUEUE_SIZE) {
        printf("Illegal add state!\n");
        return;
    }
    int index = (queueIndex + queueSize)%MAX_PRODUCT_QUEUE_SIZE;
    drawFullGhost(index,frame);
    queue[index] = productNum;
    queueSize ++;
    unlockMutex(modifyQueueMutex);
}

int removeFromProductQueue(){
    lockMutex(modifyQueueMutex);
    if(queueSize==0){
        printf("Illegal remove state!\n");
        return -1;
    }
    int product = queue[queueIndex];
    deleteGhost(queueIndex);
    deletePacman(queueIndex);
    queueIndex = (queueIndex + 1) % MAX_PRODUCT_QUEUE_SIZE;
    drawAnimatedPacman(queueIndex,frame);
    queueSize --;
    unlockMutex(modifyQueueMutex);

    return product;
}

void animate(){
    while (1){

        frame = (frame+1);
        int i,index;
        lockMutex(modifyQueueMutex);
        for(i=0;i<queueSize;i++){
            index = (queueIndex + i)%MAX_PRODUCT_QUEUE_SIZE;
//            printf("animat %d", index);
//            drawFullGhost(index,frame);
            animateGhost(index,frame);
        }
        drawAnimatedPacman(queueIndex,frame);
        unlockMutex(modifyQueueMutex);
        sleep(500);
    }
}

int getXFromQueue(int index){
    return (index % MAX_PRODUCTS_PER_ROW)*(WIDTH/MAX_PRODUCTS_PER_ROW)+XOFFSET;
}
int getYFromQueue(int index){
    return ((index / MAX_PRODUCTS_PER_ROW)*(HEIGHT/MAX_PRODUCTS_PER_ROW))+YOFFSET;
}

void drawAnimatedPacman(int index, int status){
    int x = getXFromQueue(index)+PACMAN_XOFFSET;
    int y = getYFromQueue(index)+PACMAN_YOFFSET;
    int radius = PACMAN_SIZE;
    switch (status%4){
        case 0:
            drawPacman(x,y,radius,6);
            break;
        case 1:
            drawPacman(x,y,radius,12);
            break;
        case 2:
            drawClosePacman(x,y,radius);
            break;
        case 3:
            drawPacman(x,y,radius,12);
            break;
    }
}
void deletePacman(int index){
    int x = getXFromQueue(index)+PACMAN_XOFFSET;
    int y = getYFromQueue(index)+PACMAN_YOFFSET;
    qword color=0x000000;

    drawCFullCircle(x,y,PACMAN_SIZE,color);
}

void animateGhost(int index, int status){
    int x = getXFromQueue(index);
    int y = getYFromQueue(index);
    int size = GHOST_SIZE;
    qword color=0xFF0000;
    deleteGhostLegs(x,y,size);
    switch (status%2){
        case 0:
            drawGhostEyes(x,y,size,status%4);
            draw3GhostLegs(x,y,size,color);
            break;
        case 1:
            drawGhostEyes(x,y,size,status%2);
            draw4GhostLegs(x,y,size,color);
            break;
    }
}

void drawFullGhost(int index, int status){
    int x = getXFromQueue(index);
    int y = getYFromQueue(index);
    int size = GHOST_SIZE;
    qword color=0xFF0000;
    drawCFullCircle(x+size/2,y+size/2,size/2,color);
    drawCSquare(x,y+size/2,size/2,size,color);
    animateGhost(index,status);
}

void deleteGhost(int index){
    int x = getXFromQueue(index);
    int y = getYFromQueue(index);
    qword color=0x000000;

    int size = GHOST_SIZE;
    drawCSquare(x-GHOST_SIZE,y-GHOST_SIZE+1,size*2.5,size*2.7,color);
}

void deleteInstructions(){
    setCursorPos(0);
    drawCSquare(0,0,YOFFSET,1000,0x000000);
}
