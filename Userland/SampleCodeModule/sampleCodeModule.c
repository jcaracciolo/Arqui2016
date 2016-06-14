#include "../../Kernel/include/types.h"
#include "shell.h"

/* sampleCodeModule.c */

extern void int80(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main() {
	//All the following code may be removed 
	*v = 'D';
	*(v+1) = 0x74;

	initShell();

	//char* a="HOLA MUNDO!!!!!!!!!!!!!!!!!";
	//int80(4,1,a,20,0,0,0);

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}
