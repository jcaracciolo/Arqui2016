#include "include/kernel.h"
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
#include "include/interrupts.h"
#include "include/videoDriver.h"
#include "include/naiveConsole.h"
#include "include/syscall.h"
#include "include/graphicsDriver.h"
#include "include/getTime.h"
#include "include/logo.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;


extern void int80(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

extern int getChar();


static const uint64_t PageSize = 0x1000;

typedef int (*EntryPoint)();




int readBuffer();
void decreaseTimerTick();
void setupEverything();

void onlyPrints() {
	print("another process!!");
	while(1) print(".");
}

void onlyPrints2(){
	while(1) print("o");
}

int main()
{
	setupEverything();

 	insertProcess(&onlyPrints);
 	print("currentPID: "); printNum(getCurrentPid()); print("\n");
 	insertProcess(&onlyPrints2);
 	print("currentPID: "); printNum(getCurrentPid()); print("\n");
 	insertProcess(sampleCodeModuleAddress);
 	//beginScheduler();

 	((EntryPoint)sampleCodeModuleAddress)();
	while(1) {
	}


	return 0;
}

void printSmt(){
	printNum(4);
}

void printSmt2(){
	printNum(8);
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

void setupEverything(){
    //Color color = {.r = 0xBF, .g = 0x0D , .b = 0x0D};
	
	Color color = {.r = 0xFF, .g = 0xFF , .b = 0x00};

	setupFonts(1);
	setupFonts(7);
	print("  Setting up Fonts ....\n");
	print("  Giving life some color....\n");
	_setColor(color);
	print("  Setting up IDT....\n");
	setup_IDT();
	print("  Setting up System Calls.....\n");
	setUpSyscalls();
	print("  Giving listeners ears.....\n");
	addTimerListener(&blinkCursor,10);
	print("  Overclocking to 60fps.....\n     (we cant see more than 24fps anyways)/s.....\n");
	decreaseTimerTick();
	activateScheduler();
	print("  Activating scheduler...\n\n");
	
	print(logo);
	print("\n\n\n");

}
