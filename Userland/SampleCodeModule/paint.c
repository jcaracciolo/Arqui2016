//
// Created by juanfra on 15/06/16.
//

#include "paint.h"
#include "include/stdio.h"
#include "include/types.h"
#include "include/stdvid.h"

static int array[YRES/4][XRES/4]={0};
static int xcursor=XRES/8;
static int ycursor=YRES/8;

static boolean erase=false;
static boolean paint=false;

static int colors[10]={0xFFFFFF,0xFF0000,0x0000FF,0x00FF00,0xFFFF00,0x00FFFF,0xFF8000,0x0080FF,0xFF00BF,0x848484};
static int indexColor=0;


int paintLoop(){

    clear();
    paintArray();
    drawCSquare(xcursor*4,ycursor*4,4,4,0xFFFFFF);
    drawCSquare(1000,0,24,24,colors[indexColor]);
    while(1){
        int c=getc();
        if(c!=-1){
            switch(c){
                case ' ':
                    erase=false;
                    paint=!paint;
                    break;
                case 'd':
                case 'D':
                    move(1,0);
                    break;
                case 's':
                case 'S':
                    move(0,1);
                    break;
                case 'a':
                case 'A':
                    move(-1,0);
                    break;
                case 'w':
                case 'W':
                    move(0,-1);
                    break;
                case '\b':
                    paint=false;
                    erase=!erase;
                    break;
                case 'c':
                case 'C':
                    emptyArray();
                    clear();
                    drawCSquare(xcursor*4,ycursor*4,4,4,0xFFFFFF);
                    drawCSquare(1000,0,24,24,colors[indexColor]);
                    break;
                case 'n':
                case 'N':
                    indexColor=(indexColor+1)%10;
                    drawCSquare(1000,0,24,24,colors[indexColor]);
                    break;
                case '\n':
                    clear();
                    return;
            }
        }
    }
}

void move(int x,int y){
    if(xcursor+x<0 || xcursor+x>XRES/4-1 || ycursor+y<0 || ycursor+y>YRES/4-1)
        return;

    if(!paint){
        drawCSquare(xcursor*4,ycursor*4,4,4,array[xcursor][ycursor]);
    }else{
        drawCSquare(xcursor*4,ycursor*4,4,4,colors[indexColor]);
        array[xcursor][ycursor]=colors[indexColor];
    }

    if(erase){
        array[xcursor][ycursor]=0;
        drawCSquare(xcursor*4,ycursor*4,4,4,0);
    }



    drawCSquare((xcursor+x)*4,(ycursor+y)*4,4,4,0xFFFFFF);

    xcursor+=x;
    ycursor+=y;



}

void paintArray(){
    for(int i=0;i<XRES/4;i++){
        for(int j=0;j<YRES/4;j++){
                drawCSquare(i*4,j*4,4,4,array[i][j]);
        }
    }
}

void emptyArray(){
    for(int i=0;i<XRES/4;i++){
        for(int j=0;j<YRES/4;j++){
            array[i][j]=0;
        }
    }
}




