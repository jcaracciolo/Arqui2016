//
// Created by julian on 6/13/16.
//http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html

#include "include/graphicsDriver.h"
static int color=0;
modeInfo *inf = (modeInfo *)0x5c00;

static int pixel = 0;

void putSquare(int x, int y, int height, int width){
    for (int i = x; i < x + width; ++i) {
        for (int j = y; j < y + height; ++j) {
            putColoredPixel(i,j,color);
        }
    }
    color+=0x02;
}


void putPixel(int x, int y){

    char * vi =(char*) inf->physbase + inf->pitch *x + y* inf->bpp/8;
    vi[0] = 0xFF;
    vi[1] = 0xFF;
    vi[2] = 0xFF;
}


void putColoredPixel(int x, int y,char color){

    char * vi =(char*) inf->physbase + inf->pitch *x + y* inf->bpp/8;
    vi[0] = color;
    vi[1] = color>>6;
    vi[2] = color<<6;
}
void incPixel(int m) {
    for (int i = 0; i < inf->Yres; ++i) {
        putPixel(i,5);
    }
}