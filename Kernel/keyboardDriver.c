#include "types.h"
#include "keyboardDriver.h"

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
        buffer[lastIndex]=read_key();
        lastIndex=(lastIndex+1)%100;
    }
}

int readBuffer(){


    if(actualIndex!=lastIndex){
        char aux=buffer[actualIndex];
        actualIndex=(actualIndex+1)%100;
        return  aux;
    }

    return EOF;
}
