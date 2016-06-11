
#include "types.h"
#include <naiveConsole.h>

#include "videoDriver.h"

static char *video = (char *) 0xb8000;

extern int read_key();

void irqDispatcher(int irq){	
	switch(irq) {
		case 0:
			//timerTick();
			break;
		case 1:
			onKeyPressed();
			break;
	}
	return;
}

static int counter = 0;
static int i = 0;
static char ascii = '0';

void timerTick(){
	if (counter++ == 18) {	// 2 sec transcurred
		counter = 0;
		video[i++] = ascii++;
		video[i++] = (char) 0x25;
	}
}

void onKeyPressed(){
	printKey();
}

