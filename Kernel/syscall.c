#include "videoDriver.h"
#include "types.h"
#include "interrupts.h"
#include "defs.h"
#include "keyboardDriver.h"


#define SYSTEM_CALL_COUNT 2
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
   while(i<size){
        *(charbuffer+i)=readBuffer();
        i++;

   }
     //printNum((int)(*(charbuffer)),8);
    //TODO FILL WITH CODE
    //printNum(5,8);
    return 1;
}

qword sys_write(qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9){
     printNum(5,8);
     return 1;
 }



void syscallHandler(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9){
    char* num [5];
    sysCalls[0](0,(qword)num,5,0,0,0);
//    sysCalls[rax](rdi,rsi,rdx,r10,r8,r9);

    if(rax < 0 || rax >= SYSTEM_CALL_COUNT) {
        //TODO this should crash, snow crash.
        return;
    }
}

void setUpSyscalls(){

    sysCalls[0] = &sys_read;
    sysCalls[1] = &sys_write;

    setup_IDT_entry (SYSTEM_CALL_START_INDEX, 0x08, (qword)&_irq80Handler, ACS_INT);
}




