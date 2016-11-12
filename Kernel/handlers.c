
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


#define FPS 60

DESCR_INT *idt = 0x0;	// IDT de 11 entradas
IDTR idtr;			// IDTR

#define MAX_LISTENERS 10

static qword counter = 0;
static int timerListeners =0;

static timerEventT timerEvents[MAX_LISTENERS];
static int alarmEvents[MAX_LISTENERS];


static int* sleepCounter;
void blink(){
	if (counter++ == 6) {	// 1/3 sec transcurred
		counter = 0;
		blinkCursor();
	}
}
void timerTick(){
	counter++;
	for (int j = 0; j < timerListeners; j++) {
		if(counter % alarmEvents[j]==0) timerEvents[j]();
	}
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


void timerSleep(){
	(*sleepCounter) += 16;	// 1s/60fps = 16ms per frame
}

void sleep(unsigned int time){

	*sleepCounter=0;

	_cli();
	addTimerListener(&timerSleep,1);
	_sti();

	while(*sleepCounter<time);


	_cli();
	deleteTimerListener(&timerSleep);
	_sti();

	return;
}

void executeSchedule() {
	schedule();
}

void activateScheduler() {
	addTimerListener(&executeSchedule, 10);
}


void addTimerListener(timerEventT event, int interval){
	if(timerListeners >= MAX_LISTENERS) return;
	else{
		alarmEvents[timerListeners] = interval;
		timerEvents[timerListeners] = event;
		timerListeners++;
 	}
}

void deleteTimerListener(timerEventT event){
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

