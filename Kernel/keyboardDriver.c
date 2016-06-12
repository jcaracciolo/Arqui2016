#include "types.h"
#include "keyboardDriver.h"
#include "include/types.h"
#include "include/videoDriver.h"

extern char read_key();

static int buffer[100];
static int actualIndex=0;
static int lastIndex=0;

void preFillBuffer(){
    for(int i=0; i<10; i++){
        buffer[i] = 't';
    }
    lastIndex=10;
}

void addToBuffer(){
    if(lastIndex!=actualIndex-1){
        buffer[lastIndex]=2;
        printNum(lastIndex,2);
        printNum(actualIndex,2);
        lastIndex=(lastIndex+1)%100;
    }


}

int readBuffer(){


    if(actualIndex!=lastIndex){
        printNum(lastIndex,8);
        printNum(actualIndex,8);
        char aux=buffer[actualIndex];
        actualIndex=(actualIndex+1)%100;
        return  aux;
    }

    return EOF;
}
