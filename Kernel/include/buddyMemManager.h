//
// Created by juanfra on 13/11/16.
//

#ifndef ARQUI2016_BUDDYMEMMANAGER_H
#define ARQUI2016_BUDDYMEMMANAGER_H

#include "lib.h"

/* Initializing heap*/
void initializeHeap();

/* Modifying Heap */
void* buddyAllocate(uint64_t amount);
void* buddyAllocatePages(uint64_t pages);
void* buddyReallocate(void* address,uint64_t amount);
int buddyFree(void* address);

/* Mark creators*/
uint16_t myBit(uint16_t n);
uint16_t myMask(uint16_t n);
int isPowerOfTwo (uint64_t x);
uint64_t roundUpPower2(uint64_t v);


#define MAXHEAPSIZE (MAXMEMORY/MINPAGE)*2-1

#endif //ARQUI2016_BUDDYMEMMANAGER_H
