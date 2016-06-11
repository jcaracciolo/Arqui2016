
#include "types.h"
#include <naiveConsole.h>

#include "videoDriver.h"

static char *video = (char *) 0xb8000;

extern int read_key();

void irqDispatcher(int irq){	
	switch(irq) {
		case 0:
			blink();
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

void blink(){
	if (counter++ == 6) {	// 1/3 sec transcurred
		counter = 0;
		blinkCursor();
	}
}

void onKeyPressed(){
	printKey();
}

