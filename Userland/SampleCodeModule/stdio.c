#include "../../Kernel/include/types.h"

extern void int80(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

putc(char c) {
	int80(4,1,&c,1,0,0,0);
}


int getc() {
	char a[2];
	int80(3,1,a,2,0,0,0);
	if(*a==0) return EOF;
	return *a;
}

// cambiar por printf
void print(char* arr, int length) {
	int80(4,1,arr,length,0,0,0);
}
