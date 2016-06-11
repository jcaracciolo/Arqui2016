#include "types.h"
#include "keyboardDriver.h"


char buffer[100];
char actualIndex=0;
char lastIndex=0;

void addToBuffer(){
    buffer[lastIndex]=read_key();
    lastIndex=(lastIndex++)%100;
}

char readBuffer(){

    if(actualIndex!=lastIndex){
        char aux=buffer[actualIndex];
        actualIndex=(actualIndex++)%100;
        return aux;
    }

    return EOF;
}
