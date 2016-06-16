//
// Created by julian on 6/16/16.
//
#include "include/stdio.h"
#include "include/sw_movie.h"
#include "include/stdvid.h"

static int lastFrame = 50;
static int frameIndex =0;
void printFrame(){
    for (int j = 0; j < lastFrame; ++j) {
        clear();


        for (int i = 0; i < 14; ++i) {
            printf(sw_frames[frameIndex++]);
        }
    }
}
