//
// Created by julian on 6/16/16.
//
#include "include/stdio.h"
#include "include/sw_movie.h"
#include "include/stdvid.h"

static int lastFrame = 400;
static int frameIndex =0;
void printFrame(){
    for (int j = 0; j < lastFrame; ++j) {
        //drawCSquare(0,0,)
        clear();

        for (int i = 0; i < 13; ++i) {
            printf(sw_frames[frameIndex++]);
        }
		sleep(50);
    }
}
