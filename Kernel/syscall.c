#include "include/types.h"
#include "include/videoDriver.h"
#include "include/keyboardDriver.h"
#include "include/interrupts.h"
#include "include/defs.h"
#include "include/graphicsDriver.h"


#define SYSTEM_CALL_COUNT 6
#define SYSTEM_CALL_START_INDEX 0x80

typedef qword (*sys)(qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

static sys sysCalls[SYSTEM_CALL_COUNT];
void setup_IDT_entry (int , byte , qword , byte );
/**
  *
 */
qword sys_read(qword file, qword buffer, qword size, qword r10, qword r8, qword r9){

    char* charbuffer=(char*)buffer;
    int i=0;
    int c;
    while(i<size-1 && (c=readBuffer())!=EOF){
	charbuffer[i]=(char)c;
        i++;
   }
	charbuffer[i]=0;
    //TODO FILL WITH CODE

    return 1;
}

qword sys_write(qword rdi, qword buffer, qword size, qword r10, qword r8, qword r9){

    char* charbuffer=(char*)buffer;
    while(size--){
        printChar(*charbuffer++);
     }
     return 1;
}


qword sys_clear(qword rdi, qword buffer, qword size, qword r10, qword r8, qword r9) {
	clearScreen();
	clearChars();
}

qword sys_line(qword origX, qword origY, qword destX, qword destY, qword color, qword r9) {
	drawCLine(origX,origY,destX,destY, hexaToColor(color));
}

qword sys_pixel(qword x, qword y, qword color, qword r10, qword r8, qword r9) {
	drawCPixel(x,y,hexaToColor(color));
}



void syscallHandler(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9){

    sysCalls[rax](rdi,rsi,rdx,r10,r8,r9);

    if(rax < 0 || rax >= SYSTEM_CALL_COUNT) {
        //TODO this should crash, snow crash.
        return;
    }
}

void _irq80Handler(void);

void setUpSyscalls(){
	sysCalls[0] = &sys_pixel;
	sysCalls[1] = &sys_line;
    sysCalls[3] = &sys_read;
    sysCalls[4] = &sys_write;
	sysCalls[5] = &sys_clear;

    setup_IDT_entry (SYSTEM_CALL_START_INDEX, 0x08, (qword)&_irq80Handler, ACS_INT);
}






