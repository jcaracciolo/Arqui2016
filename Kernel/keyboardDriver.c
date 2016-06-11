#include "types.h"
#include "keyboardDriver.h"

extern char read_key();

static char buffer[100];
static int actualIndex=0;
static int lastIndex=0;

void addToBuffer(){

    buffer[lastIndex]=read_key();
    lastIndex=(lastIndex+1)%100;
}

char readBuffer(){

    if(actualIndex!=lastIndex){
        char aux=buffer[actualIndex];
        actualIndex=(actualIndex+1)%100;
        return aux;
    }

    return EOF;
}
