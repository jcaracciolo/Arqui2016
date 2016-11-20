#include <unistd.h>
#include "include/types.h"
#include "include/videoDriver.h"
#include "include/condvar.h"
#include "include/keyboardDriver.h"
#include "include/interrupts.h"
#include "include/defs.h"
#include "include/graphicsDriver.h"
#include "include/getTime.h"
#include "include/syscall.h"
#include "include/lib.h"
#include "include/kernel.h"
#include "include/mutex.h"
#include "include/buddyMemManager.h"
#include "include/process.h"
#include "include/liballoc.h"
#include "include/scheduler.h"
#include "include/IPCstructs.h"

#define SYSTEM_CALL_COUNT 41
#define SYSTEM_CALL_START_INDEX 0x80

typedef qword (*sys)(qword rsi, qword rdx, qword rcx, qword r8, qword r9);

static sys sysCalls[SYSTEM_CALL_COUNT];
static int foregroundMutex;

qword sys_clear(qword rsi, qword rdx, qword rcx, qword r8, qword r9) {
    clearScreen();
    clearChars();
    return 0;
}

/*--------PAGING ALLOCATION  -----------------------------------*/
qword sys_allocatePages(qword address, qword cantPages, qword rcx, qword r8, qword r9) {
    void** ad =(void**)address;
    *ad = buddyAllocatePages(cantPages);
    return 0;
}

qword sys_reallocatePages(qword address, qword cantPages, qword rcx, qword r8, qword r9) {
    void** ad =(void**)address;
    *ad = buddyReallocate(*ad,cantPages);
    return 0;
}

qword sys_freePages(qword address, qword cantPages, qword rcx, qword r8, qword r9) {
    buddyFree((void*)address);
    return 0;
}

qword sys_allocateNewMemory(qword address, qword amountBytes, qword rcx, qword r8, qword r9) {
    void** ad =(void**)address;
    *ad = buddyAllocate(amountBytes);
    return 0;
}

qword sys_reallocateNewMemory(qword address, qword amountBytes, qword rcx, qword r8, qword r9) {
    void** ad =(void**)address;
    *ad = buddyReallocate(*ad,amountBytes);
    return 0;
}


//TODO fix this
/*--------------------- MEMORY ALLOCATING ----------------------------*/

qword sys_allocate(qword address, qword size, qword rcx, qword r8, qword r9) {
     void** ad =(void**)address;
    *ad=lib_malloc((int)size);
    return 0;
}

qword sys_free(qword address, qword rdx, qword rcx, qword r8, qword r9) {
    void* ad =(void*)address;
    lib_free(ad);
    return 0;
}

qword sys_reallocate(qword address, qword size, qword rcx, qword r8, qword r9) {
    void** ad =(void**)address;
    *ad=lib_realloc(*ad,(int)size);
    return 0;
}

/*------------------------------------------------------------------------*/

qword sys_read(qword file, qword buffer, qword size, qword r8, qword r9) {
    if (file == 0) {
        readFull((char*) buffer, (int) size);
    }else if(file>2 && file<8){
        pipe_t pipe=getMyProcessData()->fd[file-3];
        readPipe(pipe,buffer,size);
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
    }else if(file>2 && file<8){
        pipe_t pipe=(getMyProcessData())->fd[file-3];
        writePipe(pipe,buffer,size);
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

qword sys_circle(qword x, qword y, qword r, qword color, qword r9) {
    _drawCCircle(x, y,r, hexaToColor(color));
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


/*------------------------MUTEX ----------------------*/
qword sys_createMutex(qword name, qword mutex, qword rcx, qword r8, qword r9) {
    int * ret = (int *) mutex;
    *ret = getMutex((char*) name);
    return 0;
}

qword sys_releaseMutex(qword mutex, qword ret, qword rcx, qword r8, qword r9) {
    int * retVal = (int *) ret;
    int mutexCode = (int ) mutex;
    *retVal = releaseMutexFromPos(mutexCode);
    return 0;
}


qword sys_tryLock(qword mutex, qword ret, qword rcx, qword r8, qword r9) {
    int * retVal = (int *) ret;
    int mutexCode = (int ) mutex;
    *retVal = lockMutex(mutexCode);
    return 0;
}

qword sys_unlock(qword mutex, qword ret, qword rcx, qword r8, qword r9) {
    int * retVal = (int *) ret;
    int mutexCode = (int ) mutex;
    *retVal = unlockMutex(mutexCode);
    return 0;
}

/*----------------------- PROCCESS --------------------------*/

qword sys_exec(qword entry_point, qword pid, qword cargs, qword pargs, qword r9) {
    int * retPid = (int *) pid;
    *retPid = insertProcess((void *)entry_point, (int)cargs, (void **)pargs);
    return 0;
}

qword sys_ps(qword buff, qword num, qword rcx, qword r8, qword r9) {
    sprintAllProcesses(buff,num);
    //printAllProcesses();
    return 0;
}

qword sys_yield(qword rsi, qword rdx, qword rcx, qword r8, qword r9) {
    _yield();
    return 0;
}

//TODO fix this
qword sys_kill(qword pid, qword msg, qword rcx, qword r8, qword r9) {
    switch(msg) {
        case -1:
            // kill current process
            changeProcessState(getCurrentPid(), DEAD);
            break;
        case 0:
            // kill custom process
            changeProcessState(pid, DEAD);
            break;
        case 1:
            // sleep custom process
            changeProcessState(pid, SLEEPING);
            break;
        case 2:
            // wake up custom process
            changeProcessState(pid, READY);
            break;
        case 3:
            setForeground(pid);
            break;
    }
    return 0;
}

qword sys_leave(qword rsi, qword rdx, qword rcx, qword r8, qword r9) {
    if (getForegroundPid() == getCurrentPid()) {
        setForeground(1); // give forground to shell
    }
    changeProcessState(getCurrentPid(), DEAD);
    _yield();
    return 0;
}

qword sys_myPID(qword ans, qword rdx, qword rcx, qword r8, qword r9) {
    int * retPid = (int *) ans;
    *retPid = getCurrentPid();
    return 0;
}

/*------------------------- PIPES ------------------------------------*/

    qword sys_openPipe(qword name, qword ans, qword rcx, qword r8, qword r9) {
        int * a = (int *) ans;
        *a = addPipe(getPipe((char*)name)) +3;
    return 0;
    }

    qword sys_closePipe(qword name, qword rdx, qword rcx, qword r8, qword r9) {
        releasePipe(name);
        return;
    }


/*------------------------- CONDITIONAL VARIABLES ----------------------*/

    qword sys_waitCondVar(qword condVarArg, qword mutexArg, qword rcx, qword r8, qword r9) {
        condVar_t * condVar = (condVar_t *) condVarArg;
        int mutex = (int ) mutexArg;
        waitCondVar(condVar,mutex);
    return 0;
    }

    qword sys_signalCondVar(qword condVarArg, qword rdx, qword rcx, qword r8, qword r9) {
        condVar_t * condVar = (condVar_t *) condVarArg;
        signalCondVar(condVar);
        return;
    }

    qword sys_initCondVar(qword condVarArg, qword rdx, qword rcx, qword r8, qword r9) {
        condVar_t * condVar = (condVar_t *) condVarArg;
        initCondVar(condVar);
        return;
    }

    qword sys_broadcastCondVar(qword condVarArg, qword rdx, qword rcx, qword r8, qword r9) {
        condVar_t * condVar = (condVar_t *) condVarArg;
        broadcastCondVar(condVar);
        return;
    }

    qword sys_ipcs(qword pipcs, qword mutexes, qword pipes, qword ansMutexes, qword ansPipes) {
        int* ansM = ( int* )ansMutexes;
        int* ansP = ( int* )ansPipes;
        ipcs * ans = (ipcs*) pipcs;
        *ansM = getMuxeseNames(ans,mutexes);
        *ansP = getPipesNames(ans,pipes);
        return;
    }

/*--------------------------------------------------------------------*/

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
    sysCalls[19] = &sys_freePages;
    sysCalls[20] = &sys_reallocatePages;
    sysCalls[21] = &sys_allocateNewMemory;
    sysCalls[22] = &sys_reallocateNewMemory;

    sysCalls[23] = &sys_createMutex;
    sysCalls[24] = &sys_releaseMutex;
    sysCalls[25] = &sys_tryLock;
    sysCalls[26] = &sys_unlock;

    sysCalls[27] = &sys_exec;
    sysCalls[28] = &sys_ps;
    sysCalls[29] = &sys_kill;
    sysCalls[30] = &sys_leave;
    sysCalls[31] = &sys_yield;
    sysCalls[32] = &sys_myPID;

    sysCalls[33] = &sys_openPipe;
    sysCalls[34] = &sys_closePipe;

    sysCalls[35] = &sys_initCondVar;
    sysCalls[36] = &sys_signalCondVar;
    sysCalls[37] = &sys_waitCondVar;


    sysCalls[38] = &sys_circle;
    sysCalls[39] = &sys_broadcastCondVar;

    sysCalls[40] = &sys_ipcs;

    setup_IDT_entry (SYSTEM_CALL_START_INDEX, 0x08, (qword)&_irq80Handler, ACS_INT);
}

void syscallHandler(qword rdi,qword rsi, qword rdx, qword rcx, qword r8, qword r9){


    if(rdi < 0 || rdi >= SYSTEM_CALL_COUNT) {
        //TODO this should crash, snow crash.
        return;
    }
    sysCalls[rdi](rsi,rdx,rcx,r8,r9);
}



