#include "types.h"
#include "keyboardDriver.h"
#include "include/types.h"
#include "include/videoDriver.h"

extern char read_key();

#define BUFFER_SIZE 100

static int buffer[BUFFER_SIZE];
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
        lastIndex=(lastIndex+1)%BUFFER_SIZE;
    }


}

int readBuffer(){


    if(actualIndex!=lastIndex){
        char aux=buffer[actualIndex];
        actualIndex=(actualIndex+1)%BUFFER_SIZE;
        return  aux;
    }

    return EOF;
}
