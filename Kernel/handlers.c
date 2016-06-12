
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

DESCR_INT *idt = 0x0;	// IDT de 11 entradas
IDTR idtr;			// IDTR

static int counter = 0;
static int i = 0;
static char ascii = '0';


void blink(){
	if (counter++ == 6) {	// 1/3 sec transcurred
		counter = 0;
		blinkCursor();
	}
}

void irqDispatcher(int irq){
	switch(irq) {
		case 0:
			blink();
           break;
		case 1:
			addToBuffer();
			break;
	}
	return;
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

