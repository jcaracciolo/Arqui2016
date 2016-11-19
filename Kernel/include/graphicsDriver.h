//
// Created by julian on 6/13/16.
//

#ifndef GRAPHICSDRIVER_H
#define GRAPHICSDRIVER_H

#include "types.h"
#include "videoDriver.h"


#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8

#define FONT_SCALE 2

#define WIN_WIDTH 1024
#define WIN_HEIGHT 768

#define RED_MASK 0x00FF0000
#define GREEN_MASK 0x00FF00
#define BLUE_MASK 0x0000FF


#define inBound(x,y) ((x)>=0 && (x)<1024 && (y)>=0 && (y)<768)
#define abs(n) ((n)>=0?(n):-(n))
#define round(n) (int)((n) < 0 ? ((n) - 0.5) : ((n) + 0.5));
#define sqrt3 1.73205080757

#define M_2_PI 6.28318530718f
#define M_PI_2 1.57079632679f
#define M_PI 3.14159265359f

#define CURSOR_HEIGHT (CHAR_HEIGHT*FONT_SCALE)


typedef  unsigned short uint16;
typedef  unsigned int uint32;
typedef  unsigned char uint8;

typedef struct {
	uint8 r;
	uint8 g;
	uint8 b;
} Color;

struct ModeInfoBlock {
    uint16 attributes;
    uint8 winA,winB;
    uint16 granularity;
    uint16 winsize;
    uint16 segmentA, segmentB;
    uint32 realFctPtr;
    uint16 pitch; // bytes per scanline

    uint16 Xres, Yres;
    uint8 Wchar, Ychar, planes, bpp, banks; //bpp = bits per pixel
    uint8 memory_model, bank_size, image_pages;
    uint8 reserved0;

    uint8 red_mask, red_position;
    uint8 green_mask, green_position;
    uint8 blue_mask, blue_position;
    uint8 rsv_mask, rsv_position;
    uint8 directcolor_attributes;

    uint32 physbase;  // your LFB (Linear Framebuffer) address ;)
    uint32 reserved1;
    uint16 reserved2;
} __attribute__((packed));


typedef struct ModeInfoBlock modeInfo;

void drawChar(char c, int x, int y);

Color hexaToColor(qword color);
void _drawPixel(int x, int y);
void _drawSquare(int x, int y, int height, int width);
void incPixel(int m) ;
void _drawCPixel(int x, int y, Color color);
void _drawCSquare(int x, int y, int height, int width, Color c);
void _setColor(Color c);
Color getColor();
void clearScreen();
void _drawStraightLine(uint32 x, uint32 y, uint32 length);
void _drawVerticalLine(uint32 x, uint32 y, uint32 length);
void _drawLine(uint32 x1, uint32 y1, uint32 x2, uint32 y2);
void _drawCLine(uint32 x1, uint32 y1, uint32 x2, uint32 y2, Color c);
void graphicScroll();
void graphicScrollWeird();
void _drawCCircle(uint32 x, uint32 y, uint32 r, Color c);
//void _drawCCircle(uint32 x1, uint32 y1, uint32 radius, Color c);


#endif //GRAPHICSDRIVER_H
