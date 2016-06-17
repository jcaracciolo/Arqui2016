#include <unistd.h>
#include "include/types.h"
#include "include/videoDriver.h"
#include "include/keyboardDriver.h"
#include "include/interrupts.h"
#include "include/defs.h"
#include "include/graphicsDriver.h"
#include "include/getTime.h"
#include "include/syscall.h"
#include "include/lib.h"

#define SYSTEM_CALL_COUNT 14
#define SYSTEM_CALL_START_INDEX 0x80

typedef qword (*sys)(qword rsi, qword rdx, qword rcx, qword r8, qword r9);

static sys sysCalls[SYSTEM_CALL_COUNT];
/**
  *
 */

qword sys_clear(qword rsi, qword rdx, qword rcx, qword r8, qword r9) {
    clearScreen();
    clearChars();
    return 0;
}

qword sys_allocate(qword pointer, qword size, qword rcx, qword r8, qword r9) {
    pointer=allocate(size);
    return 0;
}

qword sys_free(qword rsi, qword rdx, qword rcx, qword r8, qword r9) {
    return 0;
}

qword sys_read(qword file, qword buffer, qword size, qword r8, qword r9){

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

qword sys_write(qword file, qword buffer, qword size, qword r8, qword r9){

    char* charbuffer=(char*)buffer;
    while(size--){
        printChar(*charbuffer++);
     }
     return 1;
}

qword sys_line(qword origX, qword origY, qword destX, qword destY, qword color) {
    _drawCLine(origX, origY, destX, destY, hexaToColor(color));
    return 0;
}

qword sys_square(qword origX, qword origY, qword height, qword width, qword color) {
    _drawCSquare(origX, origY, height, width, hexaToColor(color));
    return 0;
}

qword sys_pixel(qword x, qword y, qword color, qword r8, qword r9) {
    _drawCPixel(x, y, hexaToColor(color));
    return 0;
}

qword sys_timezone(qword tz, qword rdx, qword rcx, qword r8, qword r9) {
	_setTimeZone(tz);
}

qword sys_sleep(qword time, qword rdx, qword rcx, qword r8, qword r9) {
	sleep(time);
}

qword sys_time(qword hour, qword min, qword sec, qword r8, qword r9) {
    char* h=(char*)hour;
	char* mi=(char*)min;
	char* s=(char*)sec;

	*(h) =_getHours();
	*(mi) =_getMinutes();
	*(s) = _getSeconds();
    return 0;
}

qword sys_date(qword date,qword month,qword year,qword r8,qword r9){
    char* d=(char*)date;
    char* mo=(char*)month;
    char* y=(char*)year;

    *(d) =_getDayofMonth();
    *(mo) =_getMonth();
    *(y) = _getYear();
}

qword sys_removeTimerEvent(qword timerEventFunc, qword rdx, qword rcx, qword r8, qword r9) {
    _cli();
    deleteTimerListener((timerEventT) timerEventFunc);
    _sti();
    return 0;
}

qword sys_addTimerEvent(qword timerEventFunc, qword interval, qword rcx, qword r8, qword r9){
    _cli();
    addTimerListener((timerEventT) timerEventFunc,interval);
    _sti();
    return 0;
}



void setUpSyscalls(){
	sysCalls[0] = &sys_clear;
	sysCalls[1] = &sys_allocate;
	sysCalls[2] = &sys_free;
    sysCalls[3] = &sys_read;
    sysCalls[4] = &sys_write;
    sysCalls[5] = &sys_timezone;
    sysCalls[6] = &sys_time;
    sysCalls[7] = &sys_date;
    sysCalls[8] = &sys_addTimerEvent;
    sysCalls[9] = &sys_removeTimerEvent;
	sysCalls[10] = &sys_sleep;
    sysCalls[11] = &sys_pixel;
    sysCalls[12] = &sys_line;
    sysCalls[13] = &sys_square;


    setup_IDT_entry (SYSTEM_CALL_START_INDEX, 0x08, (qword)&_irq80Handler, ACS_INT);
}

void syscallHandler(qword rdi,qword rsi, qword rdx, qword rcx, qword r8, qword r9){


    if(rdi < 0 || rdi >= SYSTEM_CALL_COUNT) {
        //TODO this should crash, snow crash.
        return;
    }
    sysCalls[rdi](rsi,rdx,rcx,r8,r9);
}



