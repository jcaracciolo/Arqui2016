
#include "../../Kernel/include/types.h"
#include "stdtime.h"

extern void int80(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

char getHours() {
    char h;
	char mi;
	char s;
	char y;
	char mo;
	char d;
	int80(2, &h, &mi, &s, &y, &mo, &d);
	return h;
}


char getMinutes() {
    char h;
	char mi;
	char s;
	char y;
	char mo;
	char d;
	int80(2, &h, &mi, &s, &y, &mo, &d);
	return mi;
}


char getSeconds() {
    char h;
	char mi;
	char s;
	char y;
	char mo;
	char d;
	int80(2, &h, &mi, &s, &y, &mo, &d);
	return s;
}


char getYear() {
    char h;
	char mi;
	char s;
	char y;
	char mo;
	char d;
	int80(2, &h, &mi, &s, &y, &mo, &d);
	return y;
}


char getMonth() {
    char h;
	char mi;
	char s;
	char y;
	char mo;
	char d;
	int80(2, &h, &mi, &s, &y, &mo, &d);
	return mo;
}


char getDay() {
    char h;
	char mi;
	char s;
	char y;
	char mo;
	char d;
	int80(2, &h, &mi, &s, &y, &mo, &d);
	return d;
}
