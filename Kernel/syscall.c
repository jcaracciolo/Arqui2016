#include "include/types.h"
#include "include/videoDriver.h"
#include "include/keyboardDriver.h"
#include "include/interrupts.h"
#include "include/defs.h"


#define SYSTEM_CALL_COUNT 2
#define SYSTEM_CALL_START_INDEX 0x80

typedef qword (*sys)(qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

static sys sysCalls[SYSTEM_CALL_COUNT];
void setup_IDT_entry (int , byte , qword , byte );
/**
  *
 */
qword sys_read(qword file, qword buffer, qword size, qword r10, qword r8, qword r9){

//    printNum(file,7);
//    printNum(0,4);
//    printNum(buffer,7);
//    printNum(0,4);
//    printNum(size,7);
//    printNum(0,4);
    int* charbuffer=(int*)buffer;
    int i=0;
    int c=readBuffer();
    while(i<size){
        *(charbuffer+i)=c;
        i++;
   }
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



void syscallHandler(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9){

    sysCalls[rax](rdi,rsi,rdx,r10,r8,r9);

    if(rax < 0 || rax >= SYSTEM_CALL_COUNT) {
        //TODO this should crash, snow crash.
        return;
    }
}
void _irq80Handler(void);

void setUpSyscalls(){

    sysCalls[3] = &sys_read;
    sysCalls[4] = &sys_write;

    setup_IDT_entry (SYSTEM_CALL_START_INDEX, 0x08, (qword)&_irq80Handler, ACS_INT);
}




