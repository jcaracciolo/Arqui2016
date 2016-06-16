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
#include "../Userland/SampleCodeModule/include/sw.h"

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
void setupEverything();

int main()
{
	setupEverything();
//	ncPrint("[Kernel Main]");
//	ncNewline();
//	ncPrint("  Sample code module at 0x");
//	ncPrintHex((uint64_t)sampleCodeModuleAddress);
//	ncNewline();

	ncPrint("  Setting up IDT");
	setup_IDT();
	ncPrint("  Setting up System Calls");
	setUpSyscalls();
	addTimerListener(&blinkCursor,10);

//	ncPrint("  Calling the sample code module returned: ");
//	//HERE IT SHOULD CALL SAMPLE MODULE BUT INSTEAD CALL A MODULE ONLY IF COMMAND
//	ncNewline();
//	ncNewline();
//
//	ncPrint("  Sample data module at 0x");
//	ncPrintHex((uint64_t)sampleDataModuleAddress);
//	ncNewline();
//	ncPrint("  Sample data module contents: ");
//	ncPrint((char*)sampleDataModuleAddress);
//	ncNewline();
//
//	ncPrint("[Finished]");
//	ncPrint("[Finished]");


//	drawFractalEquilateral(0,768,768,15);
	//Call shell
	((EntryPoint)sampleCodeModuleAddress)();

	//for (int u = 1; u < 20; u++) {
	//	printNum(u, 0x05);
	//	print("Linea de prueba   \n", 65);
	//}
	//print("Linea de pruebaLinea de pruebaLinea de pruebaLinea de pruebaLinea de pruebaLinea de pruebaLinea de pruebaLinea de pruebaLinea de pruebaLinea de prueba", 65);
	
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

	setupFonts(10);
	print("  Setting up Fonts ....\n");
	print("  Giving life some color....\n");
	_setColor(color);
	print("  Setting up IDT....\n");
	setup_IDT();
	print("  Setting up System Calls.....\n");
	setUpSyscalls();
	print("  Giving listeners ears.....\n");
	addTimerListener(&blinkCursor,10);
	print("  Overclocking to 60fps.....\n     (we cant see more than 24fps anyways)/s.....\n\n");
	decreaseTimerTick();

}
