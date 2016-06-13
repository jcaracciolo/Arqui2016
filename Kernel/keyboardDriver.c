#include "include/types.h"
#include "include/types.h"
#include "include/videoDriver.h"
#include "include/scanCodes.h"

extern char read_key();

#define BUFFER_SIZE 100

static int buffer[BUFFER_SIZE];
static int actualIndex=0;
static int lastIndex=0;

static boolean lshift= false;
static boolean rshift=false;
static boolean lalt=false;
static boolean ralt=false;

void addToBuffer(){
    int key=read_key();

    if(key==0x36 || key==-74){
        rshift=!rshift;
    }else if(key==0x2A || key==-86){
        lshift=!lshift;
    }else if(key==0x38 || lalt==0xB8){
        lalt=!lalt;
    }

    if(lastIndex!=actualIndex-1){
        if(key<0x40 && key>0){
            if(ASCII_VALUE[key]!=0){
                if(lshift || rshift){
                    buffer[lastIndex] = SHIFT_ASCII_VALUE[key];
                }else{
                    buffer[lastIndex] = ASCII_VALUE[key];
                }
                lastIndex = (lastIndex + 1) % BUFFER_SIZE;
            }else{


            }
        }
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

