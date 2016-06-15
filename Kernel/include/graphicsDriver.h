//
// Created by julian on 6/13/16.
//

#ifndef ARQUI2016_GRAPHICSDRIVER_H
#define ARQUI2016_GRAPHICSDRIVER_H

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8

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

void putPixel(int x, int y);
void putSquare(int x, int y, int height, int width);
void incPixel(int m) ;
void putCPixel(int x, int y,Color color);
void putCSquare(int x, int y, int height, int width, Color c);
void setColor(Color c);
void clearScreen();

void drawStraightLine(uint32 x,uint32 y,uint32 length);
void drawVerticalLine(uint32 x,uint32 y,uint32 length);
void drawLine(uint32 x1, uint32 y1,uint32 x2, uint32 y2);
void drawTriangle(uint32 x1, uint32 y1,uint32 x2, uint32 y2,uint32 x3,uint32 y3);
void fractalTriangle( uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 x3, uint32 y3 ,uint32 recursion);
void drawEquilateral(uint32 x,uint32 y,uint32 size);
void drawFractalEquilateral(uint32 x,uint32 y, uint32 size,uint32 recursion);


#endif //ARQUI2016_GRAPHICSDRIVER_H
