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

#define MAX_PRODUCT_QUEUE_SIZE 30

void producer();
void consumer();
void addToProductQueue(int productNum);
int removeFromProductQueue();

static int queue[MAX_PRODUCT_QUEUE_SIZE];
static int queueSize;
static int queueIndex;
static condVar_t condVar;
static int mutex;




void producerConsumer(){
    sleep(200);
    printf("hi there");

    queueSize = 0;
    queueIndex =0;
    mutex = createMutex("prodConsumer");
    initCondVar(&condVar);

    void** parg = (void**)malloc(sizeof(void*));
    parg[0] = (void*)"producer";
    exec(&producer,1,parg, 1);
    parg[0] = (void*)"consumer";
    exec(&consumer,1,parg, 1);

    drawCFullCircle(100,100,30,0x00EEEEEE);
    drawCFullCircle(150,100,70,0x00EEEEEE);
}

void producer(){
    printf("hi, i am a producer\n");
    int i = 0;
    while (i < 50){
        i++;
        sleep(500);
        lockMutex(mutex);
        addToProductQueue(i);
        printf("prod %d\n",i);
        signalCondVar(&condVar);
        unlockMutex(mutex);
    }
    printf("releasing size: %d, index: %d\n",queueSize,queueIndex);
}

void consumer(){
    sleep(randBound(5000,15000));

    lockMutex(mutex);
    printf("hi, i am a consumer\n");
    while (1){
        while (queueSize <= 0)waitCondVar(&condVar,mutex);

        printf("cons %d\n", removeFromProductQueue());
    }
    unlockMutex(mutex);
}


void addToProductQueue(int productNum){
    if(queueSize == MAX_PRODUCT_QUEUE_SIZE) {
        printf("Illegal add state!\n");
        return;
    }
    int index = (queueIndex + queueSize)%MAX_PRODUCT_QUEUE_SIZE;
    queue[index] = productNum;
    queueSize ++;
}

int removeFromProductQueue(){
    if(queueSize==0){
        printf("Illegal remove state!\n");
        return -1;
    }
    int product = queue[queueIndex];
    queueIndex = (queueIndex + 1) % MAX_PRODUCT_QUEUE_SIZE;
    queueSize --;
    return product;
}