#include"include/stdvid.h"
#include "include/stdtime.h"
#include "include/types.h"
#include "include/stdlib.h"

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

void drawCFractalEquilateral(uint32 x,uint32 y, uint32 size, uint32 recursion, qword color){
	if(recursion==0) return;

	drawCEquilateral(x,y,size,color);


	
	int h=round(size*sqrt3/2.0);
	
	drawCFractalEquilateral(x,y,size/2,recursion-1,color);

	drawCFractalEquilateral(x+size/2,y,size/2,recursion-1,color);

	drawCFractalEquilateral(x+size/4,y-h/2,size/2,recursion-1,color);
}

//https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void drawCEmptyCircle(int x, int y, int radius, qword color) {
	int deltax = radius;
	int deltay = 0;
	int err = 0;

	while (deltax >= deltay)
	{
		drawCPixel(x + deltax, y + deltay, color);
		drawCPixel(x + deltay, y + deltax, color);
		drawCPixel(x - deltay, y + deltax, color);
		drawCPixel(x - deltax, y + deltay, color);
		drawCPixel(x - deltax, y - deltay, color);
		drawCPixel(x - deltay, y - deltax, color);
		drawCPixel(x + deltay, y - deltax, color);
		drawCPixel(x + deltax, y - deltay, color);

		deltay += 1;
		err += 1 + 2*deltay;
		if (2*(err-deltax) + 1 > 0)
		{
			deltax -= 1;
			err += 1 - 2*deltax;
		}
	}
}

//http://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
void drawCFullCircle(int x, int y, int radius, qword color) {
//	int i;
//	for(int tempY=-radius; tempY<=radius; tempY++)
//		for(int tempX=-radius; tempX<=radius; tempX++)
//			if(tempX*tempX+tempY*tempY <= radius*radius)
//				drawCPixel(x+tempX, y+tempY,color);
	int80(38,x,y,radius,color,0);
}

void drawPacman(int x,int y, int r,int mouthClosed){

	int mc=mouthClosed;
	qword color=0xFFEE00;
	drawCFullCircle(x,y,r,color);
	drawCSquare(x      ,y-r/mc  ,      r/mc*2 ,r/4+1,0000);
	drawCSquare(x+r/4  ,y-2*r/mc  ,  2*r/mc*2 ,r/4+1,0000);
	drawCSquare(x+r/2  ,y-3*r/mc,    3*r/mc*2 ,r/4+1,0000);
	drawCSquare(x+3*r/4,y-4*r/mc    ,4*r/mc*2 ,r/3,0000);
	drawCFullCircle(x+r/8,y-(r/2),r/8,0000);
}

void drawClosePacman(int x,int y,int r){
	qword color=0xFFEE00;
	drawCFullCircle(x,y,r,color);
	drawCFullCircle(x+r/8,y-(r/2),r/8,0000);
}


void animatePacman(int x,int y,int radius){
	while(1){
		drawPacman(x,y,radius,6);
		sleep(500);
		drawPacman(x,y,radius,12);
		sleep(500);
		drawClosePacman(x,y,radius);
		sleep(500);
		drawPacman(x,y,radius,12);
		sleep(500);
	}


}

void drawGhostEyes(int x,int y,int size,int pos){
	int vec[4][2]={{1,0},{0,1},{-1,0},{0,-1}};
	drawCFullCircle(x+size/3        ,y+size/3,size/8,0xFFFFFF);
	drawCFullCircle(x+2*size/3      ,y+size/3,size/8,0xFFFFFF);
	drawCFullCircle(x+size/3 + vec[pos%4][0]*size/15    ,y+size/3+ vec[pos%4][1]*size/15,size/16,0);
	drawCFullCircle(x+2*size/3+ vec[pos%4][0]*size/15  ,y+size/3 + vec[pos%4][1]*size/15,size/16,0);
}

void draw3GhostLegs(int x,int y, int size,qword color){
	drawCFullCircle(x+size/6  ,y+size,size/6,color);
	drawCFullCircle(x+size/2  ,y+size,size/6,color);
	drawCFullCircle(x+5*size/6,y+size,size/6,color);
}

void draw4GhostLegs(int x,int y, int size,qword color){
	drawCFullCircle(x+size/8  ,y+size,size/8,color);
	drawCFullCircle(x+3*size/8,y+size,size/8,color);
	drawCFullCircle(x+5*size/8,y+size,size/8,color);
	drawCFullCircle(x+7*size/8,y+size,size/8,color);
}

void deleteGhostLegs(int x,int y,int size){
	drawCSquare(x,y+size,size/6+10,size,0);
}

void drawGhost(int x,int y, int size){
	qword color=0xFF0000;
	drawCFullCircle(x+size/2,y+size/2,size/2,color);
	drawCSquare(x,y+size/2,size/2,size,color);
	for (int i = 0; i > -1; ++i) {
		drawGhostEyes(x,y,size,i++%2);
		draw3GhostLegs(x,y,size,color);
		sleep(700);
		deleteGhostLegs(x,y,size);
		drawGhostEyes(x,y,size,i%2);
		draw4GhostLegs(x,y,size,color);
		sleep(700);
		deleteGhostLegs(x,y,size);
	}


}

void pacmanClear(int cant) {
    int size=1024/4/cant;

    for (int j = 0; j < 760; j+=cant*size) {
        for (int i = 0; i < 1024; i+=size/4) {
                for (int k = 0; k < cant; ++k)
                drawPacman(i+size/2,j+size/2+k*size,size/2,6);
                sleep(100/cant);
                 for (int k = 0; k < cant; ++k)
                drawPacman(i+size/2,j+size/2+k*size,size/2,12);
                sleep(100/cant);
                for (int k = 0; k < cant; ++k)
                drawClosePacman(i+size/2,j+size/2+k*size,size/2);
                sleep(100/cant);
                for (int k = 0; k < cant; ++k)
                drawPacman(i+size/2,j+size/2+k*size,size/2,12);
                sleep(100/cant);
                for (int k = 0; k < cant; ++k)
                drawCSquare(i,j+k*size,size+1,size+1,0);
        }
    }

    clear();



}