//
// Created by julian on 6/13/16.
//http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html

#include "include/graphicsDriver.h"

modeInfo *inf = (modeInfo *)0x5c00;

static int pixel = 0;

void putSquare(int x, int y, int height, int width){
    for (int i = x; i < x + width; ++i) {
        for (int j = y; j < y + height; ++j) {
            putPixel(i,j);
        }
    }
}

void putPixel(int x, int y){

    char * vi =(char*) inf->physbase + inf->pitch *y + x* inf->bpp;
    vi[0] = 0xff;
    vi[1] = 0xff;
    vi[2] = 0x00;
}

void incPixel(int m) {
    for (int i = 0; i < inf->Yres; ++i) {
        putPixel(i,5);
    }
}