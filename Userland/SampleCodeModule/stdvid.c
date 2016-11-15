#include "include/stdvid.h"
#include "include/stdtime.h"
#include "include/types.h"

extern void int80(qword rax, qword rdi, qword rsi, qword rdx, qword r8, qword r9);

static qword stdColor=0xFFFFFF;

void clear() {
	int80(0,0,0,0,0,0);
}

void setColor(qword color) {
	stdColor = color;
}

void drawCLine(int x1, int y1, int x2, int y2, qword color) {
	int80(12,x1,y1,x2,y2,color);
}

void setCursorPos(int pos) {
	int80(14,pos,0,0,0,0);	
}

int getCursorPos() {
	int pos;
	int80(14,&pos,1,0,0,0);	
	return pos;
}

void drawLine(int x1, int y1, int x2, int y2) {
	int80(12,x1,y1,x2,y2,stdColor);
}

void drawPixel(int x, int y) {
	int80(11,x,y,stdColor,0,0);
}

void drawCPixel(int x, int y, qword color) {
	int80(11,x,y,color,0,0);
}

void drawCSquare(int x, int y, int height, int width,qword color){
	int80(13,x,y,height,width,color);
}

void drawSquare(int x, int y, int height, int width){
	int80(13,x,y,height,width,stdColor);
}

void drawTriangle(uint32 x1, uint32 y1,uint32 x2, uint32 y2,uint32 x3,uint32 y3){
	drawLine(x1,y1,x2,y2);
	drawLine(x2,y2,x3,y3);
	drawLine(x3,y3,x1,y2);
}

void drawCTriangle(uint32 x1, uint32 y1,uint32 x2, uint32 y2,uint32 x3,uint32 y3,qword color){
	drawCLine(x1,y1,x2,y2,color);
	drawCLine(x2,y2,x3,y3,color);
	drawCLine(x3,y3,x1,y2,color);
}

void drawEquilateral(uint32 x,uint32 y,uint32 size){

	if(size<2) return;

	int h=round(size*sqrt3/2.0);
	drawTriangle(x,y,x+size,y,x+size/2,y-h);
}

void drawCEquilateral(uint32 x,uint32 y,uint32 size,qword color){

	if(size<2) return;

	int h=round(size*sqrt3/2.0);
	drawCTriangle(x,y,x+size,y,x+size/2,y-h,color);
}

void drawFractalEquilateral(uint32 x,uint32 y, uint32 size,uint32 recursion){
	if(recursion==0) return;

	drawEquilateral(x,y,size);

	int h=round(size*sqrt3/2.0);
	drawFractalEquilateral(x,y,size/2,recursion-1);
	drawFractalEquilateral(x+size/2,y,size/2,recursion-1);
	drawFractalEquilateral(x+size/4,y-h/2,size/2,recursion-1);

}

void drawFractal() {	
	drawCFractalEquilateral(150,768,768,9,readData());
	leave();
}

void drawCFractalEquilateral(uint32 x,uint32 y, uint32 size, uint32 recursion, qword color){
	if(recursion==0) return;

	drawCEquilateral(x,y,size,color);


	
	int h=round(size*sqrt3/2.0);

	if(recursion%3==0)sleep(1);

	
	drawCFractalEquilateral(x,y,size/2,recursion-1,color);

	drawCFractalEquilateral(x+size/2,y,size/2,recursion-1,color);

	drawCFractalEquilateral(x+size/4,y-h/2,size/2,recursion-1,color);
}

