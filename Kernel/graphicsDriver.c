//
// Created by julian on 6/13/16.
//http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html

#define inBound(x,y) ((x)>=0 && (x)<1024 && (y)>=0 && (y)<768)
#define abs(n) ((n)>=0?(n):-(n))
#define round(n) (int)((n) < 0 ? ((n) - 0.5) : ((n) + 0.5));
#include "fonts.h"
#define sqrt3 1.73205080757
#define FONT_SCALE 2
#include "include/graphicsDriver.h"



static Color color = {.r = 0xFF, .g = 0xFF , .b = 0xFF};
modeInfo *inf = (modeInfo *)0x5c00;

void setColor(Color c){
    color.r = c.r;
    color.g = c.g;
    color.b = c.b;
}

void putSquare(int x, int y, int height, int width){
    for (int i = x; i < x + width; ++i) {
        for (int j = y; j < y + height; ++j) {
            putPixel(i,j);
        }
    }
}
void putCSquare(int x, int y, int height, int width, Color c){
    Color temp = color;
    setColor(c);
    putSquare(x,y,height,width);
    setColor(temp);
}

void putPixel(int x, int y){

    char * vi =(char*) inf->physbase + inf->pitch *y + x* inf->bpp/8;
    vi[0] = color.b;
    vi[1] = color.g;
    vi[2] = color.r;
}

void putCPixel(int x, int y,Color c){

    char * vi =(char*) inf->physbase + inf->pitch *y + x* inf->bpp/8;
    vi[0] = c.b;
    vi[1] = c.g;
    vi[2] = c.r;
}

void incPixel(int m) {
    for (int i = 0; i < inf->Yres; ++i) {
        putPixel(i,5);
    }
}

void drawLine(uint32 x1, uint32 y1,uint32 x2, uint32 y2){


    if(0 && x1==x2){
        int y=y1>y2?y2:y1;
        int l=abs(y1-y2);
        drawVerticalLine(x1,y,l);
    }else{

        double m=((double)y2-y1)/((double)x2-x1+0.01);
        double b=y1-m*x1;

        int y;

        if(x1>x2){
            int aux=x1;
            x1=x2;
            x2=aux;
        }

        for(int i=x1;i<x2;i++){
            y=round(m*i+b);

            if(!inBound(i,y)) break;
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

void drawTriangle(uint32 x1, uint32 y1,uint32 x2, uint32 y2,uint32 x3,uint32 y3){
    drawLine(x1,y1,x2,y2);
    drawLine(x2,y2,x3,y3);
    drawLine(x3,y3,x1,y2);
}

void fractalTriangle( uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 x3, uint32 y3,uint32 recursion )
{
    /* Base case. */
    if( recursion==0 ) return;

    /* Drawing step. */
    drawTriangle(x1,y1,x2,y2,x3,y3);

    /* Recursive step. */



   //         .                 x3 , y3
  //      .       .             (x1+x2)/4 , (y1+y3)/2 | (x1+x2)*3/4 , (y1+y3)/2
 //   .       .       .         x1 , y1 | (x1+x2)/2 y1 | x2,y2

    int x1x24=round((x1+x2)/4.0);
    int ym=round((y3+y1)/2.0);

//    fractalTriangle(x1,y1,x1+x2)/2,y1,(x1+x2)/4,(y1+y3)/2,recursion-1);
//    fractalTriangle((x1+x2)/2,y1, x2, y2, (x1+x2)*3/4, (y1+y3)/2,recursion-1 );
//    fractalTriangle((x1+x2)/4,(y1+y3)/2,(x1+x2)*3/4,(y1+y3)/2,x3,y3,recursion-1);

    fractalTriangle(x1,y1,x1x24 * 2, y1,x1x24,ym,recursion-1);
    fractalTriangle(x1x24*2,y1, x2, y2, x1x24*3, ym,recursion-1 );
    fractalTriangle(x1x24,ym,x1x24*3,ym,x3,y3,recursion-1);
}

void drawEquilateral(uint32 x,uint32 y,uint32 size){

    int h=round(size*sqrt3/2.0);
    drawTriangle(x,y,x+size,y,x+size/2,y-h);
}

void drawFractalEquilateral(uint32 x,uint32 y, uint32 size,uint32 recursion){
    if(recursion==0) return;

    drawEquilateral(x,y,size);


    //.      .      .

    int h=round(size*sqrt3/2.0);
    drawFractalEquilateral(x,y,size/2,recursion-1);
    drawFractalEquilateral(x+size/2,y,size/2,recursion-1);
    drawFractalEquilateral(x+size/4,y-h/2,size/2,recursion-1);

}

void drawChar(char c, int x, int y) {
    uint8 i,j;
    Color black = {.r = 0x00, .g = 0x00 , .b = 0x0};

    // Convert the character to an index
    c = c & 0x7F;
    if (c < ' ') {
        c = 0;
    } else {
        c -= ' ';
    }
    // 'font' is a multidimensional array of [96][char_width]
    // which is really just a 1D array of size 96*char_width.
    const uint8* chr = fonts[c];
    // Draw pixels
        for (j=0; j<CHAR_WIDTH; j++) {
            for (i=0; i<CHAR_HEIGHT; i++) {

                if (chr[j] & (1<<i)) {
//                    putPixel(x+j, y+i);
                    putSquare((x+j)*FONT_SCALE,(y+i)*FONT_SCALE,FONT_SCALE,FONT_SCALE);
                } else putCSquare((x+j)*FONT_SCALE,(y+i)*FONT_SCALE,FONT_SCALE,FONT_SCALE,black);
            }
        }
}

void clearScreen(){
    Color temp = {.r = 0x00, .g = 0x00 , .b = 0x0};
    putCSquare(0,0,inf->Yres,inf->Xres,temp);
}
