
#include "include/types.h"
#include "include/videoDriver.h"
#include "include/condvar.h"
#include "include/scanCodes.h"
#include "include/scheduler.h"
#include "include/process.h"
#include "include/mutex.h"
#include "include/getTime.h"
extern char read_key();

#define BUFFER_SIZE 100

static int buffer[BUFFER_SIZE];
static int actualIndex=0;
static int lastIndex=0;
static boolean bufferEmpty = true;

static condVar_t readSTDINcondVar;
static int readSTDINMutex;

static boolean lshift= false;
static boolean rshift=false;
static boolean lalt=false;
static boolean caps=false;
static boolean ctrl=false;

void addToBuffer(){
    int key=read_key();

    if (key == 0x1D) {
        ctrl = true; // ctrl pressed
    } else if (key == -99) {
        ctrl = false; // ctrl released
    } else if(key==0x36 || key==-74){
        rshift=!rshift;
    }else if(key==0x2A || key==-86){
        lshift=!lshift;
    }else if(key==0x38 || lalt==0xB8){
        lalt=!lalt;
    }else if(key==0x3a){
        caps=!caps;
    }

    int aux;
    if(lastIndex!=actualIndex-1){
        if(key<0x40 && key>0){
            if(ASCII_VALUE[key]!=0){
                if(lshift || rshift){
                   aux = SHIFT_ASCII_VALUE[key];
                } else if (ctrl) {
                    switch (key) {
                        case 0x2E: // C
                            changeProcessState(getForegroundPid(), DEAD);
                            return;
                        case 0x1F: // S
                            setForeground(1); //Le da el foreground a la shell
                            return;
                    }
                } else {
                    aux = ASCII_VALUE[key];
                }

                if(caps){
                    if(aux>='a' && aux<='z'){
                        aux=aux-'a'+'A';
                    }else if(aux>='A' && aux<='Z'){
                        aux=aux+'a'-'A';
                    }
                }
                buffer[lastIndex]=aux;
                lastIndex = (lastIndex + 1) % BUFFER_SIZE;
                bufferEmpty = false;
                broadcastCondVar(&readSTDINcondVar);
            }
        }
    }
}

int getCharsInBuffer(){
    if(lastIndex >= actualIndex){
        return lastIndex - actualIndex ;
    } else {
        return BUFFER_SIZE - actualIndex + lastIndex;
    }
}

int readBuffer(){
    if(actualIndex!=lastIndex){
        char aux=buffer[actualIndex];
        actualIndex=(actualIndex+1)%BUFFER_SIZE;
        return  aux;
    }
    bufferEmpty = true;
    return EOF;
}

void readFull(char * buffer, int size){
    int i = 0;
    int c;

    lockMutex(readSTDINMutex);
    while (getForegroundPid() != getCurrentPid() || bufferEmpty == true ){
        waitCondVar(&readSTDINcondVar, readSTDINMutex);
    }
    unlockMutex(readSTDINMutex);
    while (i < size - 1 && (c = readBuffer()) != EOF) {
        buffer[i] = (char) c;
        i++;

    }
    buffer[i] = 0;
}

void initKeyboardDriver(){
    initCondVar(&readSTDINcondVar);
    readSTDINMutex = getMutex("readSTDINMutex");
}

condVar_t * getSTDINCondVar(){
    return &readSTDINcondVar;
}