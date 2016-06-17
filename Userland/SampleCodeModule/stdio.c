#include "../../Kernel/include/types.h"
#include "include/strings.h"
#include <stdarg.h>
#include "include/shell.h"

extern void int80(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

void putc(char c) {
	int80(4,1,&c,1,0,0,0);
}


int getc() {
	char a[2];
	int80(3,1,a,2,0,0,0);
	if(*a==0) return EOF;
	return *a;
}

// cambiar por printf
void printCharArray(char* arr, int length) {
	int80(4,1,arr,length,0,0,0);
}

int printf(const char* format,...){
	va_list args;
	va_start( args, format );

	int n;
	char strnum[10];

	while(*format!='\0'){
		if(*format!='%'){
			putc(*format++);
		}else{
			switch(*(++format)){
				case '%': putc('%'); break;
				case 'd':
				case 'i':
					n=va_arg(args,int);
					int length=intToString(strnum,n);
					printCharArray(strnum,length);
					break;
				case 'c':
					n=(char)va_arg(args,int);
					putc((char)n);
					break;
			}
		}

	}

	return 0;

}
