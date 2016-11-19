//
// Created by julian on 6/13/16.
//http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html
#include "include/types.h"
#include "include/fonts.h"
#include "include/graphicsDriver.h"
#define INBOUND(x,y) ((x)>0 && (x)<inf->Xres && (y)>0 && (y)<inf->Yres)

#define SQRT2 1.41421356237

Font fonts[MAX_FONTS];
int selectedFont = 4;

void cos(float x, float * res){
    if( x < 0.0f )
        x = -x;
    while( M_PI < x )
        x -= M_2_PI;
    *res = 1.0f - (x*x/2.0f)*( 1.0f - (x*x/12.0f) * ( 1.0f - (x*x/30.0f) * (1.0f - x*x/56.0f )));
    return;
}

void sin(float x, float * res){
    cos(x-M_PI_2, res);
    return ;
}

static Color color = {.r = 0xFF, .g = 0xFF , .b = 0xFF};
static int hexColor=0;
modeInfo *inf = (modeInfo *)0x5c00;

Color getColor(){
    return color;
}

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

    width=(x+width>inf->Xres)?inf->Xres-x:width;
    height=(y+height>inf->Yres)?inf->Yres-y:height;
    --width;
    int bpp=inf->bpp/8;
    int line=bpp*(width);
    char * vi= inf->physbase + inf->pitch *y + bpp*x;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            *((int*)vi)=hexColor;
            vi+=bpp;
        }

        vi[0] = color.b;
        vi[1] = color.g;
        vi[2] = color.r;


        vi-=line;
        vi+=inf->pitch;
    }

}

void graphicScroll(){
    int width=inf->Xres;
    int height=inf->Yres-(3*CURSOR_HEIGHT);
    int bpp=inf->bpp/8;
    int line=bpp*(width);
    char * vi= inf->physbase;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            *((char*)vi)=*((char*)(vi+line*3*CURSOR_HEIGHT));
            vi++;
            *((char*)vi)=*((char*)(vi+line*3*CURSOR_HEIGHT));
            vi++;
            *((char*)vi)=*((char*)(vi+line*3*CURSOR_HEIGHT));
            vi++;
        }
        vi-=line;
        vi+=inf->pitch;
    }

    Color black={.b=0,.g=0,.r=0};
    _drawCSquare(0,(inf->Yres-(3*CURSOR_HEIGHT)),3*CURSOR_HEIGHT,width,black);





}

void graphicScrollWeird(){
    int width=inf->Xres;
    int height=inf->Yres;
    --width;
    int bpp=inf->bpp/8;
    int line=bpp*(width);
    char * vi= inf->physbase;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            *((int*)vi)=*((int*)(vi+line*CURSOR_HEIGHT));
            vi+=bpp;
        }
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
    if(fontNumber <MAX_FONTS)
    selectedFont = fontNumber;
}

void clearScreen(){

    int size=inf->Yres*inf->Xres*3;
    char* vi =(char*) inf->physbase;
    for(int i=0;i<size;i+=3){
        vi[i]=0;
        vi[i+1]=0;
        vi[i+2]=0;
    }


}

// Quake Inverse Sqrt by John Carmak
void Sqrt(float x,float* res){
    float xhalf = 0.5f * x;
    int i = *(int*)&x;            // store floating-point bits in integer
    i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
    x = *(float*)&i;              // convert new bits into float
    x = x*(1.5f - xhalf*x*x);     // One round of Newton's method
    if(x>0){

        *res =1/x;
    }else{
        *res=0;
    }
    return;
}

//https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void _drawCEmptyCircle(int x, int y, int radius) {
    int deltax = radius;
    int deltay = 0;
    int err = 0;

    while (deltax >= deltay)
    {
        _drawPixel(x + deltax, y + deltay);
        _drawPixel(x + deltay, y + deltax);
        _drawPixel(x - deltay, y + deltax);
        _drawPixel(x - deltax, y + deltay);
        _drawPixel(x - deltax, y - deltay);
        _drawPixel(x - deltay, y - deltax);
        _drawPixel(x + deltay, y - deltax);
        _drawPixel(x + deltax, y - deltay);

        deltay += 1;
        err += 1 + 2*deltay;
        if (2*(err-deltax) + 1 > 0)
        {
            deltax -= 1;
            err += 1 - 2*deltax;
        }
    }
}



void _drawCircle(uint32 x, uint32 y, uint32 r){
    int radius=(int) r;
    int side=(2*radius)/SQRT2;
    int hside=side/2;
    int corte= (radius-side);
    int aux;
    int auxX;
    int auxY;
    int r2=radius*radius;
    //top hat
    for(int tempY=-radius; tempY<=-hside; tempY++) {
        for (int tempX = -radius; tempX <= radius; tempX++) {
            aux=tempX * tempX + tempY * tempY;
            if ( aux<=r2 ) {
                auxX = x + tempX;
                auxY = y + tempY;
                if (inBound(auxX, auxY)) {
                    _drawPixel(auxX, auxY);
                }
            }
        }
    }


    //bot hat
    for(int tempY=hside; tempY<=radius; tempY++) {
        for (int tempX = -radius; tempX <= radius; tempX++) {
            aux=tempX * tempX + tempY * tempY;
            if ( aux<=r2 ) {
                auxX = x + tempX;
                auxY = y + tempY;
                if (inBound(auxX, auxY)) {
                    _drawPixel(auxX, auxY);
                }
            }
        }
    }

    //top hat
    for(int tempY=-radius; tempY<=radius; tempY++) {
        for (int tempX = -radius; tempX <= -hside; tempX++) {
            aux=tempX * tempX + tempY * tempY;
            if ( aux<=r2 ) {
                auxX = x + tempX;
                auxY = y + tempY;
                if (inBound(auxX, auxY)) {
                    _drawPixel(auxX, auxY);
                }
            }
        }
    }

    //top hat
    for(int tempY=-radius; tempY<=radius; tempY++) {
        for (int tempX = hside; tempX <= radius; tempX++) {
            aux=tempX * tempX + tempY * tempY;
            if ( aux<=r2 ) {
                auxX = x + tempX;
                auxY = y + tempY;
                if (inBound(auxX, auxY)) {
                    _drawPixel(auxX, auxY);
                }
            }
        }
    }

    _drawSquare(x-side/2,y-side/2,side,side);






//    //Bot hat
//    for(int tempY=hside; tempY<=radius; tempY++)
//        for(int tempX=-radius; tempX<=radius; tempX++)
//            if(tempX*tempX+tempY*tempY <= radius*radius)
//                _drawPixel(x+tempX, y+tempY);
//
//    //Left hat
//    for(int tempY=-radius; tempY<=radius; tempY++)
//        for(int tempX=-radius; tempX<=-hside; tempX++)
//            if(tempX*tempX+tempY*tempY <= radius*radius)
//                _drawPixel(x+tempX, y+tempY);
//
//    //Right hat
//    for(int tempY=-radius; tempY<=radius; tempY++)
//        for(int tempX=hside; tempX<=radius; tempX++)
//            if(tempX*tempX+tempY*tempY <= radius*radius && )
//                _drawPixel(x+tempX, y+tempY);


//    for (int i = side; i < r; ++i) {
//        _drawCEmptyCircle(x,y,i/2);
//    }
}


void _drawCCircle(uint32 x, uint32 y, uint32 r, Color c) {
    Color temp = color;
    _setColor(c);
    _drawCircle(x,y,r);
    _setColor(temp);
}

