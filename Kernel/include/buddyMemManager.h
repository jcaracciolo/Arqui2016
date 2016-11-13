//
// Created by juanfra on 13/11/16.
//

#ifndef ARQUI2016_BUDDYMEMMANAGER_H
#define ARQUI2016_BUDDYMEMMANAGER_H

#include "lib.h"
typedef unsigned int size_t;

/* Initializing heap*/
void initializeHeap();

/* Modifying Heap */
void* buddyAllocate(size_t amount);
void buddyFree(void* address);

/* Mark creators*/
unsigned short myBit(unsigned short n);
unsigned short myMask(unsigned short n);
int isPowerOfTwo (unsigned int x);
size_t roundUpPower2(size_t v);

#define MAXMEMORY 2048000 //in b//
#define MINPAGE 4000 //in b//
#define MAXHEAPSIZE (MAXMEMORY/MINPAGE)*2-1



#endif //ARQUI2016_BUDDYMEMMANAGER_H
