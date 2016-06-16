#include <unistd.h>
#include "include/types.h"
#include "include/videoDriver.h"
#include "include/keyboardDriver.h"
#include "include/interrupts.h"
#include "include/defs.h"
#include "include/graphicsDriver.h"
#include "include/getTime.h"
#include "include/syscall.h"

#define SYSTEM_CALL_COUNT 10
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
    return 0;
}

qword sys_line(qword origX, qword origY, qword destX, qword destY, qword color, qword r9) {
    _drawCLine(origX, origY, destX, destY, hexaToColor(color));
    return 0;
}

qword sys_pixel(qword x, qword y, qword color, qword r10, qword r8, qword r9) {
    _drawCPixel(x, y, hexaToColor(color));
    return 0;
}

qword sys_timezone(qword tz, qword rsi, qword rdx, qword r10, qword r8, qword r9) {
	_setTimeZone(tz);
}

qword sys_sleep(qword time, qword rsi, qword rdx, qword r10, qword r8, qword r9) {
	sleep(time);
}

qword sys_time(qword hour, qword min, qword sec, qword year, qword month, qword day) {
    char* h=(char*)hour;
	char* mi=(char*)min;
	char* s=(char*)sec;
	char* y=(char*)year;
	char* mo=(char*)month;
	char* d=(char*)day;

	*(h) =_getHours();
	*(mi) =_getMinutes();
	*(s) = _getSeconds();
	*(y) = _getYear();
	*(mo) = _getMonth();
	*(d) = _getDayofMonth();
    return 0;
}


void syscallHandler(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9){

    sysCalls[rax](rdi,rsi,rdx,r10,r8,r9);

    if(rax < 0 || rax >= SYSTEM_CALL_COUNT) {
        //TODO this should crash, snow crash.
        return;
    }
}

void _irq80Handler(void);

qword sys_removeTimerEvent(qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

qword sys_addTimerEvent(qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);


qword sys_removeTimerEvent(qword timerEventFunc, qword rsi, qword rdx, qword r10, qword r8, qword r9) {
    _cli();
    deleteTimerListener((timerEventT) timerEventFunc);
    _sti();
    return 0;
}

qword sys_addTimerEvent(qword timerEventFunc, qword interval, qword rdx, qword r10, qword r8, qword r9) {
    _cli();
    addTimerListener((timerEventT) timerEventFunc,interval);
    _sti();
    return 0;
}


void setUpSyscalls(){
	sysCalls[0] = &sys_clear;
	sysCalls[1] = &sys_pixel;
	sysCalls[2] = &sys_line;
    sysCalls[3] = &sys_read;
    sysCalls[4] = &sys_write;
    sysCalls[5] = &sys_time;
	sysCalls[6] = &sys_timezone;
    sysCalls[7] = &sys_addTimerEvent;
    sysCalls[8] = &sys_removeTimerEvent;
	sysCalls[9] = &sys_sleep;

    setup_IDT_entry (SYSTEM_CALL_START_INDEX, 0x08, (qword)&_irq80Handler, ACS_INT);
}



