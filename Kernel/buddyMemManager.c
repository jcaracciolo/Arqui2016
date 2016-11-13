//
// Created by juanfra on 12/11/16.
//


#include "include/buddyMemManager.h"
#include "include/lib.h"
#define NULL ((void*)0)

/* Initializing heap*/
unsigned short recursiveMark(int index);

/* Modifying Heap */
void* addNblocks(unsigned char  n);
void* recursiveAdd(int i , unsigned short n,size_t address,size_t innerSize);

void releaseUp(int i,int level);
int searchUp(int i,unsigned short level);


/* Moving and marking the Heap */
#define PARENT(i) ((i)>>1)
#define LCHILD(i) ((i)<<1)
#define RCHILD(i) (((i)<<1)+1)
#define AMILEFT(i) !((i)%2)
#define SIBLING(i) ((i)%2?((i)-1):((i)+1))
#define ISNAVAILABLE(i,n) ((i)&(myBit(n)))



static unsigned short heap[MAXHEAPSIZE];
static size_t heapSize;
static size_t block;
static void* beginning=(void*)MEMBEGIN;


size_t roundUpPower2(size_t v){

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

size_t pow2(size_t n){
    return (size_t)1 << n;
}


void* buddyAllocate(size_t amount){
    if(amount<=0) return NULL;
    size_t pages=roundUpPower2( amount/block );
    return addNblocks(pages);
}

void* buddyReallocate(void* address,size_t amount){

    if( buddyFree(address) != -1){

        void* ans = buddyAllocate(amount);
        if(ans!=NULL && ans!=address){
            memcpy(ans,address,amount);
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

unsigned short myBit(unsigned short n){
    int i=0;
    while(n){
        n=n>>1;
        i++;
    }
    return 1<<(i-1);
}
unsigned short myMask(unsigned short n){
    unsigned short i=0;
    while(n){
        n--;
        i=(i<<1) +1;
    }
    return i;
}



int isPowerOfTwo (unsigned int x){
    return ((x != 0) && !(x & (x - 1)));
}

void initializeHeap(){
    block=MINPAGE;
    heapSize=MAXHEAPSIZE;
    recursiveMark(1);
}

unsigned short recursiveMark(int index){

    if(index>heapSize/2){
        heap[index]=1;
        return (1<<1) + 1;
    }

    recursiveMark(RCHILD(index));
    unsigned short mark=recursiveMark(LCHILD(index));
    heap[index]=mark;
    return (mark<<1) + 1;
}

void* addNblocks(unsigned char  n){
    if(n> ((1+heapSize)/2)) return NULL;

    return recursiveAdd(1,n,(size_t)beginning,(1+heapSize)/2);
}

void* recursiveAdd(int i , unsigned short n,size_t address,size_t innerSize){

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

    if(!isInt((long)address/(block*1.0f))){
        return -1;
    } else{
        int position=((long)address)/block;
        return searchUp(heapSize/2 + 1 + position,1);
    }

}

int searchUp(int i, unsigned short level){
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

void releaseUp(int i,int level){
    if(i<1) return;

    if(heap[RCHILD(i)] == myMask(level-1) && heap[LCHILD(i)] == myMask(level-1)){
        heap[i]=myMask(level);
    }else{
        heap[i]=heap[RCHILD(i)] | heap[LCHILD(i)];
    }
    releaseUp(PARENT(i),level+1);
}

