#include <stdio.h>
#include "../../Kernel/include/types.h"
#include "include/shell.h"
#include "include/stdvid.h"

/* sampleCodeModule.c */


//char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;


int main() {


	//All the following code may be removed
//	*v = 'D';
//	*(v+1) = 0x74;
	//drawCSquare(0,0,768,1024,color)


	initShell();
	//char* a="HOLA MUNDO!!!!!!!!!!!!!!!!!";
	//int80(4,1,a,20,0,0,0);

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}
