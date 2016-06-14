#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <defs.h>
#include <types.h>
#include <interrupts.h>
#include <syscall.h>
#include <videoDriver.h>
#include "include/videoDriver.h"
#include "include/naiveConsole.h"
#include "include/interrupts.h"
#include "include/syscall.h"
#include "include/graphicsDriver.h"
#include "include/getTime.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;


extern void int80(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

extern int getChar();

static const uint64_t PageSize = 0x1000;
static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
typedef int (*EntryPoint)();




int readBuffer();
void decreaseTimerTick();

int main()
{	
	ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();

	ncPrint("  Setting up IDT");
	setup_IDT();
	ncPrint("  Setting up System Calls");
	setUpSyscalls();


	ncPrint("  Calling the sample code module returned: ");
	//HERE IT SHOULD CALL SAMPLE MODULE BUT INSTEAD CALL A MODULE ONLY IF COMMAND
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();

	ncPrint("[Finished]");
	ncPrint("[Finished]");


    //preFillBuffer();

    decreaseTimerTick();

	_setTimeZone(-3);
//
//	printNum(_getHours(),3);
//	print(":",3);
//	printNum(_getMinutes(),3);
//
//	_setAlarmHours(10);
//	_setAlarmMinutes(51);
//	printNum(_getAlarmHours(),3);
//	print(":",3);
//	printNum(_getAlarmMinutes(),3);


//	char* a="HOLA MUNDO";
		int c;
	incPixel(0);

    while(1) {
        c=getChar();
        if (c!=-1){
			printChar(c);


		}
	}

	return 0;
}



void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
			(uint64_t)&endOfKernel
					   + PageSize * 8				//The size of the stack itself, 32KiB
					   - sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
			sampleCodeModuleAddress,
			sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}