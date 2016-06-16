//
// Created by julian on 6/13/16.
//http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html
#include "include/types.h"
#include "include/fonts.h"


#include "include/graphicsDriver.h"

Font fonts[MAX_FONTS];
int selectedFont = 4;


static Color color = {.r = 0xFF, .g = 0xFF , .b = 0xFF};
static qword hexColor=0;
modeInfo *inf = (modeInfo *)0x5c00;

void _setColor(Color c){
    color.r = c.r;
    color.g = c.g;
    color.b = c.b;

    hexColor=c.r<<16 | c.g<<8 | c.b;
}

void _drawSquare(int x, int y, int height, int width){
//    for (int i = x; i < x + width; ++i) {
//        for (int j = y; j < y + height; ++j) {
//            _drawPixel(i,j);
//        }
//    }

    int bpp=inf->bpp/8;
    int line=bpp*(--width);
    char * vi= inf->physbase + inf->pitch *y + bpp*x;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            *((qword*)vi)=hexColor;
            vi+=bpp;
        }
        vi[0] = color.b;
        vi[1] = color.g;
        vi[2] = color.r;

        vi-=line;
        vi+=inf->pitch;
    }

}

void _drawCSquare(int x, int y, int height, int width, Color c){
    Color temp = color;
    _setColor(c);
    _drawSquare(x,y,height,width);
    _setColor(temp);
}

void _drawPixel(int x, int y){

    char * vi =(char*) inf->physbase + inf->pitch *y + inf->bpp/8*x;
    vi[0] = color.b;
    vi[1] = color.g;
    vi[2] = color.r;
}

void _drawCPixel(int x, int y, Color c){

    char * vi =(char*) inf->physbase + inf->pitch *y + x* inf->bpp/8;
    vi[0] = c.b;
    vi[1] = c.g;
    vi[2] = c.r;
}

void incPixel(int m) {
    for (int i = 0; i < inf->Yres; ++i) {
        _drawPixel(i,5);
    }
}

void _drawLine(uint32 x1, uint32 y1, uint32 x2, uint32 y2){

    if(x1==x2){
        int y=y1>y2?y2:y1;
        int l=abs(y1-y2);
        _drawVerticalLine(x1,y,l);
    }else{

        double m=((double)y2-y1)/((double)x2-x1);
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
            _drawPixel(i,y);
        }

    }




}

void _drawStraightLine(uint32 x, uint32 y, uint32 length){
    while(inBound(x,y) && length-->0){
        _drawPixel(x++,y);
    }
}

void _drawVerticalLine(uint32 x, uint32 y, uint32 length){
    while(inBound(x,y) && length-->0){
        _drawPixel(x,y++);
    }
}

void _drawCLine(uint32 x1, uint32 y1, uint32 x2, uint32 y2, Color c) {
	Color temp = color;
    _setColor(c);
    _drawLine(x1,y1,x2,y2);
    _setColor(temp);
}

Color hexaToColor(qword color) {
	Color c = {.r = (color & RED_MASK) >>16, .g = (color & GREEN_MASK) >> 8, .b = color & BLUE_MASK};
	return c;
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
    const uint8* chr = fonts[selectedFont].font[c];
    // Draw pixels
        for (j=0; j<CHAR_WIDTH; j++) {
            for (i=0; i<CHAR_HEIGHT; i++) {

                if (chr[j] & (1<<i)) {
//                    drawPixel(x+j, y+i);
                    _drawSquare((x+j)*FONT_SCALE,(y+i)*FONT_SCALE,FONT_SCALE,FONT_SCALE);
                } else _drawCSquare((x+j)*FONT_SCALE,(y+i)*FONT_SCALE,FONT_SCALE,FONT_SCALE,black);
            }
        }
}

void  setupFonts(int fontNumber){
    fonts[0] = zxpix;
    fonts[1] = superdig;
    fonts[2] = sloth;
    fonts[3] = minimum;
    fonts[4] = hiskyf21;
    fonts[5] = haiku;
    fonts[6] = crackers;
    fonts[7] = bmplain;
    fonts[8] = blockus;
    fonts[9] = aztech;
    fonts[10] = fiveByFive;
    fonts[11] = uglyFont;
    if(selectedFont > MAX_FONTS) selectedFont = 0;
    selectedFont = fontNumber;
}

void clearScreen(){
//    Color black = {.r = 0x00, .g = 0x00 , .b = 0x0};
//    drawCSquare(0,0,inf->Yres,inf->Xres,black);
//    char * vi;
//    for (int i = 0; i < inf->Xres; ++i) {
//        for (int j = 0; j < inf->Yres; ++j) {
//
//            vi =(char*) inf->physbase + inf->pitch *j + inf->bpp/8*i;
//            vi[0] = 0x00;
//            vi[1] = 0x00;
//            vi[2] = 0x00;
//
//        }
//    }

    int size=inf->Yres*inf->Xres*3;
    char* vi =(char*) inf->physbase;
    for(int i=0;i<size;i+=3){
        vi[i]=0;
        vi[i+1]=0;
        vi[i+2]=0;
    }
}
