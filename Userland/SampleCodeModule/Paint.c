//
// Created by juanfra on 15/06/16.
//

#include "include/Paint.h"
#include "include/stdio.h"
#include "include/types.h"

static int array[YRES][XRES];
//static int Colors[3]={COLOR_BLUE,COLOR_RED,COLOR_GREEN}

//
//int main(){
//    boolean paint=false;
//    boolean erase=false;
//
//    drawSqare(xcursor,ycursor,4,4,WHITE);
//    while(1){
//        int c=getc();
//        if(c!=-1){
//            switch(c){
//                case ' ':
//                    erase=false;
//                    paint=!paint;
//                    break;
//                case 'd':
//                case 'D':
//                    move(1,0);
//                    break;
//                case 's':
//                case 'S':
//                    move(0,1);
//                    break;
//                case 'a':
//                case 'A':
//                    move(-1,0);
//                    break;
//                case 'w':
//                case 'W':
//                    move(0,-1);
//                    break;
//                case '\b':
//                    paint=false;
//                    erase=!erase;
//                    break;
//                case '\n':
//                    return;
//            }
//        }
//    }
//    return;
//}
//
//void move(int x,int y){
//    for(int i=0;i<4;i++){
//        putPixel()
//    }
//}
//
//



