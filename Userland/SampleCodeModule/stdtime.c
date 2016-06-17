
#include "../../Kernel/include/types.h"
#include "stdtime.h"
#include "stdio.h"

extern void int80(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

char getHours() {
    char h;
	char mi;
	char s;
	int80(6, &h, &mi, &s, 0, 0,0 );
	return h;
}


char getMinutes() {
	char h;
	char mi;
	char s;
	int80(6, &h, &mi, &s, 0, 0,0 );
	return mi;
}


char getSeconds() {
	char h;
	char mi;
	char s;
	int80(6, &h, &mi, &s, 0, 0,0 );
	return s;
}


char getYear() {
	char y;
	char mo;
	char d;
	int80(7, &d, &mo, &y, 0, 0, 0);
	return y;
}


char getMonth() {
	char y;
	char mo;
	char d;
	int80(7, &d, &mo, &y, 0, 0, 0);
	return mo;
}


char getDay() {
	char y;
	char mo;
	char d;
	int80(7, &d, &mo, &y, 0, 0, 0);
	return d;
}

void setTimeZone(int tz) {
	int80(5, tz, 0, 0, 0, 0, 0);
}

void sleep(unsigned int time){
	int80(10, time, 0, 0, 0, 0, 0);
}





