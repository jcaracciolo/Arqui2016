//
// Created by julian on 6/16/16.
//
#include "include/stdio.h"
#include "include/sw_movie.h"
#include "include/stdvid.h"
#include "include/stdtime.h"

static int lastFrame;
static int frameIndex =0;
static boolean finished = false;

void printFrame(){
        if(frameIndex >= lastFrame){
            finished = true;
//            printf("ending %d,%d",frameIndex,lastFrame);
            return;
        }
        setCursorPos(0);

        for (int i = 0; i < 18; ++i) {
            printf(sw_frames[frameIndex++]);
        }
}

void swStart(){
    finished = false;
    setCursorPos(0);
    frameIndex = 0;
    clear();
    lastFrame = 9953;
    addTimer(&printFrame,2);
    while (!finished){
        if(getc() != -1) finished = true;
    }
    removeTimer(&printFrame);
    clear();
}
