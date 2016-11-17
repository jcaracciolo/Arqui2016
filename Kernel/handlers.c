
#include "types.h"
#include <naiveConsole.h>
#include "defs.h"
#include "interrupts.h"
#include "videoDriver.h"
#include "keyboardDriver.h"
#include "include/videoDriver.h"
#include "include/interrupts.h"
#include "include/defs.h"
#include "include/keyboardDriver.h"
#include "include/scheduler.h"
#include "include/mutex.h"
#include "include/strings.h"


#define FPS 60

DESCR_INT *idt = 0x0;	// IDT de 11 entradas
IDTR idtr;			// IDTR

#define MAX_LISTENERS 50

extern void _change_process();
void deleteSleep(int index);

static qword counter = 0;
static int timerListeners =0;
static int sleepListeners =0;
static int auxj=0;

static timerEventT timerEvents[MAX_LISTENERS];
static int alarmEvents[MAX_LISTENERS];

static int sleepPIDS[MAX_LISTENERS];
static int sleepCounter[MAX_LISTENERS];
static int alarmSleep[MAX_LISTENERS];

void blink(){
	if (counter++ == 6) {	// 1/3 sec transcurred
		counter = 0;
		blinkCursor();
	}
}

void timerTick(){
	counter++;
	for (auxj = 0; auxj < timerListeners; auxj++) {
		if(counter % alarmEvents[auxj]==0) timerEvents[auxj]();
	}
	for(auxj=0;auxj < sleepListeners; auxj++){
		sleepCounter[auxj]+=1;
		if(sleepCounter[auxj] == alarmSleep[auxj]){
			doneSleeping(auxj);
            auxj--;
		}
	}
	executeSchedule();
}

void irqDispatcher(int irq){
	switch(irq) {
		case 0:
			timerTick();
			break;
		case 1:
			addToBuffer();
            break;
    }

	return;
}


//void timerSleep(){
//	(*sleepCounter) += 16;	// 1s/60fps = 16ms per frame
//}

void deleteSleep(int index){
	if(index >= sleepListeners) return;

    lockScheduler();

	alarmSleep[index] = alarmSleep[sleepListeners-1];
	sleepPIDS[index] = sleepPIDS[sleepListeners-1];
	sleepCounter[index] = sleepCounter[sleepListeners-1];
    sleepListeners--;

    unlockScheduler();

}

void addSleep(int pid,int interval){
	if(sleepListeners >= MAX_LISTENERS) return;

	lockScheduler();

	alarmSleep[sleepListeners] = interval;
	sleepPIDS[sleepListeners] = pid;
	sleepCounter[sleepListeners] = 0;
	sleepListeners++;

	unlockScheduler();
}

void doneSleeping(int index){

	changeProcessState(sleepPIDS[index],READY);
	deleteSleep(index);

}

void sleep(unsigned int time){
	char myPid=getCurrentPid();
    addSleep(myPid,time);
	changeProcessState(myPid,SLEEPING);
    _yield();
	return;
}







void executeSchedule() {
	_change_process();

}

void activateScheduler() {
	addTimerListener(&executeSchedule, 1);
}


void addTimerListener(timerEventT event, int interval){
	_cli();
	if(timerListeners >= MAX_LISTENERS) return;
	else{
		alarmEvents[timerListeners] = interval;
		timerEvents[timerListeners] = event;
		timerListeners++;
 	}
	_sti();
}

void deleteTimerListener(timerEventT event){
	_cli();
	for (int j = 0; j < timerListeners; j++) {
		if(timerEvents[j]==event){
			for(int k=j;k<timerListeners-1;k++){
				timerEvents[k]=timerEvents[k+1];
				alarmEvents[k]=alarmEvents[k+1];
			}
			timerListeners--;
			break;
		}
	}
	_sti();
}

void setup_IDT_entry (int index, byte selector, qword offset, byte access) {
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].selector = selector;
  idt[index].zero_byte = 0;
  idt[index].access = access;
  idt[index].offset_m = (offset >> 16) & 0xFFFF;
  idt[index].offset_h = (offset >> 32) & 0xFFFF;
  idt[index].zero_dword = 0;
}


void setup_IDT()
{
	setup_IDT_entry (0x20, 0x08, (qword)&_irq00Handler, ACS_INT);
	setup_IDT_entry (0x21, 0x08, (qword)&_irq01Handler, ACS_INT);

	//idtr.base = 0;
	//idtr.base +=(dword) &idt;
	//idtr.limit = sizeof(idt)-1;
	//_lidt (&idtr);


	//Todas las interrupciones desabilidas.
	picMasterMask(0xFC);
	//picSlaveMask(0xFF);

	_sti();
}

