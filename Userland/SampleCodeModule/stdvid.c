#include "stdvid.h"
#include "../../Kernel/include/types.h"

extern void int80(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);


void clear() {
	int80(5,0,0,0,0,0,0);
}

void drawLine(int x1, int y1, int x2, int y2, qword color) {	
	int80(1,x1,y1,x2,y2,color,0);
} 

void drawPixel(int x, int y, qword color) {	
	int80(0,x,y,color,0,0,0);
} 
