//
// Created by juanfra on 12/11/16.
//


#include "include/buddyMemManager.h"
#include "include/videoDriver.h"
#include "include/lib.h"
#define NULL ((void*)0)

/* Initializing heap*/
uint16_t recursiveMark(int index);

/* Modifying Heap */
void* addNblocks(uint8_t  n);
void* recursiveAdd(int i , uint16_t n,uint64_t address,uint64_t innerSize);

void releaseUp(int i,uint16_t level);
int searchUp(int i,uint16_t level);


/* Moving and marking the Heap */
#define PARENT(i) ((i)>>1)
#define LCHILD(i) ((i)<<1)
#define RCHILD(i) (((i)<<1)+1)
#define AMILEFT(i) !((i)%2)
#define SIBLING(i) ((i)%2?((i)-1):((i)+1))
#define ISNAVAILABLE(i,n) ((i)&(myBit(n)))



static uint16_t heap[MAXHEAPSIZE];
static uint64_t heapSize;
static uint64_t block;
static void* beginning=(void*)MEMBEGIN;

uint64_t roundUpPower2(uint64_t v){

    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    v++;
    return v;

}

uint64_t pow2(uint64_t n){
    return (uint64_t)1 << n;
}


void* buddyAllocate(uint64_t amount){
    if(amount<=0) return NULL;

    uint64_t pages=roundUpPower2( amount/block );
    pages=pages==0?1:pages;
    return addNblocks(pages);
}

void* buddyAllocatePages(uint64_t pages){
    if (pages==0) return NULL;
    return addNblocks(pages);
}

void* buddyReallocate(void* address,uint64_t amount){

    if( buddyFree(address) != -1){

        void* ans = buddyAllocate(amount);
        if(ans!=NULL && ans!=address){
            //memcpy(ans,address,amount);
        }
        return ans;
    }else{
        return NULL;
    }
}



int lowerBound2Pow(int n){
    int i=0;
    while(n){
        n=n>>1;
        i++;
    }
    return 1<<i;
}

uint16_t myBit(uint16_t n){
    int i=0;
    while(n){
        n=n>>1;
        i++;
    }
    return 1<<(i-1);
}
uint16_t myMask(uint16_t n){
    uint16_t i=0;
    while(n){
        n--;
        i=(i<<1) +1;
    }
    return i;
}



int isPowerOfTwo (uint64_t x){
    return ((x != 0) && !(x & (x - 1)));
}

void initializeHeap(){
    block=MINPAGE;
    heapSize=MAXHEAPSIZE;
    recursiveMark(1);
}

uint16_t recursiveMark(int index){

    if(index>heapSize/2){
        heap[index]=1;
        return (1<<1) + 1;
    }

    recursiveMark(RCHILD(index));
    uint16_t mark=recursiveMark(LCHILD(index));
    heap[index]=mark;
    return (mark<<1) + 1;
}

void* addNblocks(unsigned char  n){
    int a=((1+heapSize)/2);

    if(n> ((1+heapSize)/2)) return NULL;

    return recursiveAdd(1,n,(uint64_t)beginning,MAXMEMORY);
}

void* recursiveAdd(int i , uint16_t n,uint64_t address,uint64_t innerSize){

    void*ans=NULL;

    if(!ISNAVAILABLE(heap[i],n))
        return NULL;
    else{
        if(i<=heapSize/2 && ISNAVAILABLE(heap[RCHILD(i)],n)){

            ans= recursiveAdd(RCHILD(i),n,address+(innerSize/2),innerSize/2);
            heap[i]=heap[RCHILD(i)] | heap[LCHILD(i)];

        }else if(i<=heapSize/2 && ISNAVAILABLE(heap[LCHILD(i)],n)){

            ans= recursiveAdd(LCHILD(i),n,address,innerSize/2);
            heap[i]=heap[RCHILD(i)] | heap[LCHILD(i)];

        }else{

            heap[i]= 0;
            return (void*)address;

        }
    }

    return ans;
}

int isInt(float f){
    return (f-(int)f == 0);
}

int buddyFree(void* address){
    address= (void*)((char*)address -  (char*)beginning);

    if(!isInt((uint64_t)address/(block*1.0f))){
        return -1;
    } else{
        int position=((uint64_t)address)/block;
        return searchUp(heapSize/2 + 1 + position,1);
    }

}

int searchUp(int i, uint16_t level){
    if(i<1) return -1;
    if(heap[i]==0){
        heap[i]=myMask(level);
        releaseUp(PARENT(i),level+1);
        return 0;
    }else if(AMILEFT(i)){
        return searchUp(PARENT(i),level+1);
    }else{
        return -1;
    }
}

void releaseUp(int i,uint16_t level){
    if(i<1) return;

    if(heap[RCHILD(i)] == myMask(level-1) && heap[LCHILD(i)] == myMask(level-1)){
        heap[i]=myMask(level);
    }else{
        heap[i]=heap[RCHILD(i)] | heap[LCHILD(i)];
    }
    releaseUp(PARENT(i),level+1);
}