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
#define MAX_PRODUCTS_ROWS 3
#define MAX_PRODUCT_QUEUE_SIZE (MAX_PRODUCTS_ROWS * MAX_PRODUCTS_PER_ROW)
#define WIDTH 700
#define HEIGHT 700
#define XOFFSET 150
#define YOFFSET 150
#define PACMAN_SIZE 30
#define PACMAN_XOFFSET -10
#define PACMAN_YOFFSET 30
#define GHOST_SIZE 40

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



static int queue[MAX_PRODUCT_QUEUE_SIZE];
static int queueSize;
static int queueIndex;
static condVar_t readCondVar;
static condVar_t writeCondVar;
static int readMutex;
static int writeMutex;
static int modifyQueueMutex;
static int frame;



void producerConsumer(){
    sleep(200);
    printf("hi there");

    queueSize = 0;
    queueIndex =0;
    readMutex = createMutex("prodConsRead");
    writeMutex= createMutex("prodConsWrite");
    modifyQueueMutex= createMutex("prodConsModify");
    initCondVar(&readCondVar);
    initCondVar(&writeCondVar);
    clear();
    drawAnimatedPacman(queueIndex,frame);

    void** parg = (void**)malloc(sizeof(void*));
    parg[0] = (void*)"producer";
    exec(&producer,1,parg, 1);
    parg[0] = (void*)"consumer";
    exec(&consumer,1,parg, 1);
    parg[0] = (void*)"pacmanTick";
    exec(&animate,1,parg, 1);
//
//    int i=0;
//    for(i=0;i < MAX_PRODUCT_QUEUE_SIZE;i++){
//        drawCFullCircle(getXFromQueue(i),getYFromQueue(i),20,0x00B22222);
//    }
//    drawCFullCircle(100,100,30,0x00EEEEEE);
//    drawCFullCircle(150,100,70,0x00EEEEEE);
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

        i++;
        if(i < maxProduced/2){
            sleep(randBound(1000,2000));
        } else {
            sleep(randBound(4000,6000));
        }

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
        sleep(randBound(2000,4000));

        int prod = removeFromProductQueue();
//        printf("cons %d\n", prod);
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
        for(i=0;i<queueSize;i++){
            index = (queueIndex + i)%MAX_PRODUCT_QUEUE_SIZE;
//            printf("animat %d", index);
            animateGhost(index,frame);
        }
        drawAnimatedPacman(queueIndex,frame);
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