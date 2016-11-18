//
// Created by julian on 11/18/16.
//

#include "include/prodCons.h"
#include "include/sync.h"
#include "include/stdio.h"
#include "include/stdlib.h"
#include "include/stdtime.h"

#define MAX_PRODUCT_QUEUE_SIZE 30

void producer();
void consumer();

static int queue[MAX_PRODUCT_QUEUE_SIZE];
static int queueSize;
static int queueIndex;

void producerConsumer(){
    sleep(200);
    printf("hi there");
    queueSize = 0;
    queueIndex =0;
    void** parg = (void**)malloc(sizeof(void*));
    parg[0] = (void*)"producer";
    exec(&producer,1,parg, 1);
    parg[0] = (void*)"consumer";
    exec(&consumer,1,parg, 1);
}

void producer(){
    int mut = createMutex("asd");
    lockMutex(mut);
    printf("hi, i am a producer\n");
    addToProductQueue(26);
    addToProductQueue(275);
    addToProductQueue(21);
    addToProductQueue(45);
    addToProductQueue(77);
    printf("releasing size: %d, index: %d\n",queueSize,queueIndex);
    unlockMutex(mut);
}

void consumer(){
    int mut = createMutex("asd");
    lockMutex(mut);
    printf("hi, i am a consumer\n");
    while (queueSize > 0){
        printf("i got %d\n", removeFromProductQueue());
    }
    unlockMutex(mut);
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