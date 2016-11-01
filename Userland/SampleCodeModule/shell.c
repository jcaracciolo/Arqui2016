#include "include/shell.h"
#include "types.h"
#include "include/stdio.h"
#include "include/strings.h"
#include "include/stdlib.h"
#include "include/stdvid.h"
#include "include/stdtime.h"
#include "include/sw.h"
#include "include/gedit.h"
#include "include/paint.h"

const char* instructions = " func                - print a simple message (completly useless)\n\
 echo *param*        - prints message on console\n\
 clear               - clears the screen\n\
 time                - get current system time\n\
 setTimeZone *param* - set computer's time zone\n\
 date                - get current system date\n\
 fractal Zelda       - draws a triforce inspirated fractal\n\
 star wars           - little star wars animation\n\
 gedit               - simple text editor\n\
 paint               - simple keyboard controlled paint\n\
 paintBg *param*     - paint the console background (only once)\n\
 setupFont *param*   - setup font to write";

extern void int80(qword rdi, qword rsi, qword rdx, qword rcx, qword r8, qword r9);

char screen[ROWS * COLS] = {0};
int screenIndex = 0;

char shellBuffer[COLS + 1] = {0};	// + 1 to add '\0' at the end
int shellIndex = 0;


void initShell() {
	//clearScreen();
	printf("-- WELCOME THE SHELL --\n\n  >>");
	setTimeZone(-3);
	setConsoleSize();

	while(1) {
		int c = getc();
		if (c != EOF) {
			if (c == '\n') {
				execute();
			} else if (c == '\b') {
				removeKey();
			} else {
				addToShellBuffer(c);
				putc(c);
			}
		}
	}
}

void clearScreen() {
	clear();
	shellIndex = 0;
}

void removeKey() {
	if (shellIndex != 0) {
		shellIndex--;
		putc('\b');
	}
}

void addToShellBuffer(char c) {
	if (shellIndex <= COLS) {
		shellBuffer[shellIndex++] = c;
	}
}

void execute() {
	char arr[100];
	int tz;
	shellBuffer[shellIndex] = '\0';
	int num;
	putc('\n');
	if (strcmp(shellBuffer, "func") == 0) {
		printf("execute!\n");
	} else if (strcmp(shellBuffer, "clear") == 0) {
		clearScreen();
	} else if(strcmp(shellBuffer, "time") == 0) {
		printf("%d:%d\n", getHours(), getMinutes());
	} else if(strcmp(shellBuffer, "date") == 0) {
		printf("%d/%d/%d\n", getDay(), getMonth(), getYear());
	} else if(sscanf("setTimeZone %d",shellBuffer,&tz)==1) {
		if(tz>=-12 && tz<=12) {
			setTimeZone(tz);
			printf("\nTime set correctly to GTM %d \n",tz);
		} else{
			printf("\nInput error\n");
		}
	} else if(strcmp(shellBuffer, "fractal Zelda") == 0) {
		clear();
		drawCFractalEquilateral(150,768,768,9,readData());
		sleep(1000);
		clear();
	} else if(strcmp(shellBuffer, "help") == 0) {
		printf("%s\n", instructions);
		int *mempos = 0x10000000;
		printf("mempos: %i\n", mempos);
		int80(18,&mempos,2,0,0,0);
		printf("\nmempos: %i\n", mempos);
		int80(18,&mempos,5,0,0,0);
		printf("\nmempos: %i\n", mempos);
		int80(18,&mempos,6,0,0,0);
		printf("\nmempos: %i\n", mempos);
		int80(18,&mempos,10,0,0,0);
		printf("\nmempos: %i\n", mempos);
		int80(18,0x10000000,2,0,0,0);
		printf("\nmempos: %i\n", mempos);
		int80(18,&mempos,11,0,0,0);
		printf("\nmempos: %i\n", mempos);

	} else if(strcmp(shellBuffer, "star wars") == 0) {
		swStart();
	} else if(strcmp(shellBuffer, "gedit") == 0) {
		runGedit();
	} else if(strcmp(shellBuffer, "paint") == 0) {
		paintLoop();
	} else if(sscanf("paintBg %d", shellBuffer, &num) == 1) {
			drawCSquare(0,0,768,1024,num);
			setCursorPos(0);
	} else if(sscanf("setupFont %d", shellBuffer, &num) == 1) {
		changeFont(num);
	}else if(sscanf("echo %s",shellBuffer,arr)==1) {
		printf(arr);
		printf("\n");
	}else {
		printf("Command not found.\n");
	}
	printf("  >>");
	cleanBuffer();
}
void cleanBuffer() {
	int i;
	for (i = 0; i < COLS + 1; i++) {
		shellBuffer[i] = '\0';
	}
	shellIndex = 0;
}



