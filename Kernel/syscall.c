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
#include "include/kernel.h"
#include "include/mutex.h"

#define SYSTEM_CALL_COUNT 22
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

qword sys_allocatePages(qword address, qword cantPages, qword rcx, qword r8, qword r9) {
    void** ad =(void**)address;
    if (*ad == 0) {
        *ad = allocatePages(cantPages);
    } else {
        *ad = reallocatePages(*ad, cantPages);
    }
    return 0;
}

qword sys_allocate(qword address, qword size, qword rcx, qword r8, qword r9) {
     void** ad =(void**)address;
    *ad=allocate((int)size);
    return 0;
}

qword sys_free(qword rsi, qword rdx, qword rcx, qword r8, qword r9) {
    return 0;
}

qword sys_read(qword file, qword buffer, qword size, qword r8, qword r9) {

    if (file == 0) {
        char *charbuffer = (char *) buffer;
        int i = 0;
        int c;
        while (i < size - 1 && (c = readBuffer()) != EOF) {
            charbuffer[i] = (char) c;
            i++;
        }
        charbuffer[i] = 0;
    }
    return 1;
}

qword sys_write(qword file, qword buffer, qword size, qword r8, qword r9){

    char* charbuffer=(char*)buffer;
    if(file==1){
        while(size--){
            printChar(*charbuffer++);
        }
    }else if(file==2){

        Color col={col.r=0xFF,col.b=00,col.g=00};
        Color aux=getColor();
        _setColor(col);
        while(size--){
            printChar(*charbuffer++);

        }
        _setColor(aux);
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
    return 0;
}

qword sys_sleep(qword time, qword rdx, qword rcx, qword r8, qword r9) {
	sleep(time);
    return 0;

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
    return 0;
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

/* set position if get=0, else get position */
qword sys_cursor(qword pos, qword get, qword rcx, qword r8, qword r9) {
	if (get) {
		int* p = (int*)pos;
		*p = getCursorPosition();
	} else setCursorPosition((int)pos);
	return 0;
}


qword sys_readData(qword data, qword rdx, qword rcx, qword r8, qword r9) {
	int* p = (int*) data;
	*p = *(int*)sampleDataModuleAddress;
	return 0;
}

qword sys_changeFont(qword font, qword rdx, qword rcx, qword r8, qword r9) {
	setupFonts(font);
    return 0;
}

qword sys_getConsoleSize(qword rows, qword cols, qword rcx, qword r8, qword r9) {
	int* r = (int*) rows;
	int* c = (int*) cols;

	*c = COLS;
	*r = ROWS;
    return 0;
}

qword sys_createMutex(qword name, qword mutex, qword rcx, qword r8, qword r9) {
    int * ret = (int *) mutex;
    *ret = createMutex((char*) name);
    return 0;
}

qword sys_releaseMutex(qword mutex, qword ret, qword rcx, qword r8, qword r9) {
    int * retVal = (int *) ret;
    int mutexCode = (int ) mutex;
    *retVal = releaseMutex(mutexCode);
    return 0;
}

qword sys_handleMutexLock(qword actionArg, qword mutexIDArg, qword returnArg, qword r8, qword r9) {
    lockAction_type action = (lockAction_type) actionArg;
    int * returnValue = (int *) returnArg;
    int mutexID = (int ) mutexIDArg;
    switch (action){
        case LOCK:
            *returnValue = lockMutex(mutexID);
            break;
        case TRY_LOCK:
            *returnValue = tryLockMutex(mutexID);
            break;
        case UNLOCK:
            *returnValue = unlockMutex(mutexID);
            break;
    }
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
	sysCalls[14] = &sys_cursor;
	sysCalls[15] = &sys_readData;
	sysCalls[16] = &sys_changeFont;
    sysCalls[17] = &sys_getConsoleSize;
	sysCalls[18] = &sys_allocatePages;
	sysCalls[19] = &sys_createMutex;
	sysCalls[20] = &sys_releaseMutex;
	sysCalls[21] = &sys_handleMutexLock;


    setup_IDT_entry (SYSTEM_CALL_START_INDEX, 0x08, (qword)&_irq80Handler, ACS_INT);
}

void syscallHandler(qword rdi,qword rsi, qword rdx, qword rcx, qword r8, qword r9){


    if(rdi < 0 || rdi >= SYSTEM_CALL_COUNT) {
        //TODO this should crash, snow crash.
        return;
    }
    sysCalls[rdi](rsi,rdx,rcx,r8,r9);
}



