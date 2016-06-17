//
// Created by julian on 6/16/16.
//
#include "include/stdio.h"
#include "include/sw_movie.h"
#include "include/stdvid.h"
#include "../../Kernel/include/graphicsDriver.h"

static int lastFrame = 400;
static int frameIndex =0;
void printFrame(){
    for (int j = 0; j < lastFrame; ++j) {
        //clear();
        setCursorPos(0);
		//drawCSquare(0,0,13 * CHAR_HEIGHT,1024, 0xFFFFFF);
        for (int i = 0; i < 13; ++i) {
            printf(sw_frames[frameIndex++]);
        }
		sleep(70);
    }
}
