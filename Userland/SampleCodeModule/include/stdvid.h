#ifndef STDVID_H
#define STDVID_H

#include "types.h"

#define abs(n) ((n)>=0?(n):-(n))
#define round(n) (int)((n) < 0 ? ((n) - 0.5) : ((n) + 0.5));
#define sqrt3 1.73205080757



void clear();
void setColor(qword color);
void drawCLine(int x1, int y1, int x2, int y2, qword color);
void drawLine(int x1, int y1, int x2, int y2);
void drawPixel(int x, int y);
void drawCPixel(int x, int y, qword color);
void drawCSquare(int x, int y, int size, int width,qword color);
void drawSquare(int x, int y, int size, int width);
void drawTriangle(uint32 x1, uint32 y1,uint32 x2, uint32 y2,uint32 x3,uint32 y3);
void drawCTriangle(uint32 x1, uint32 y1,uint32 x2, uint32 y2,uint32 x3,uint32 y3,qword color);
void drawEquilateral(uint32 x,uint32 y,uint32 size);
void drawCEquilateral(uint32 x,uint32 y,uint32 size,qword color);
void drawFractalEquilateral(uint32 x,uint32 y, uint32 size,uint32 recursion);
void drawCFractalEquilateral(uint32 x,uint32 y, uint32 size,uint32 recursion,qword color);

#endif
