//
// Created by julian on 6/13/16.
//http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html

#define inBound(x,y) (x)>=0 && (x)<1024 && (y)>=0 && (y)<768
#define outOfBound(x,y) !inBound((x),(y))
#define abs(n) (n)>=0?(n):-(n)
#define round(n) (int)((n) < 0 ? ((n) - 0.5) : ((n) + 0.5));

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

void drawLine(uint32 x1, uint32 y1,uint32 x2, uint32 y2){


    if(x1==x2){
        int y=y1>y2?y2:y1;
        drawVerticalLine(x1,y,abs(y1-y2));
    }else{

        if(x1>x2){
            int aux=x1;
            x1=x2;
            x2=aux;
            aux=y1;
            y1=y2;
            y2=aux;
        }

        double m=((double)y1-y1)/((double)x2-x1);
        double b=y1-m*x1;
        int y;
        for(int i=x1;i<x2;i++){
            y=round(m*i+b);
            if(outOfBound(i,y)) break;
            putPixel(i,y);
        }

    }




}

void drawStraightLine(uint32 x,uint32 y,uint32 length){
    while(inBound(x,y) && length-->0){
        putPixel(x++,y);
    }
}

void drawVerticalLine(uint32 x,uint32 y,uint32 length){
    while(inBound(x,y) && length-->0){
        putPixel(x,y++);
    }
}