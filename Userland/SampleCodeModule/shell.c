#include "include/shell.h"
#include "../../Kernel/include/types.h"
#include "include/stdio.h"
#include "include/strings.h"
#include "include/stdlib.h"
#include "include/stdvid.h"
#include "include/stdtime.h"
#include "include/sw.h"
#include "include/gedit.h"

const char* instructions = " func           - print a simple message (completly useless)\n\
 clear          - clears the screen\n\
 time           - get current system time\n\
 setTimeZone(t) - set computer's time zone\n\
 date           - get current system date\n\
 star wars      - little star wars animation";

extern void int80(qword rdi, qword rsi, qword rdx, qword rcx, qword r8, qword r9);

char screen[ROWS * COLS] = {0};
int screenIndex = 0;

char shellBuffer[COLS + 1] = {0};	// + 1 to add '\0' at the end
int shellIndex = 0;

void initShell() {
	//clearScreen();
	printf("-- WELCOME THE SHELL --\n\n  >>");
	setTimeZone(-3);
	
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
	shellBuffer[shellIndex] = '\0';
	putc('\n');
	if (strcmp(shellBuffer, "func") == 0) {
		printf("execute!\n");
	} else if (strcmp(shellBuffer, "clear") == 0) {
		clearScreen();
	} else if(strcmp(shellBuffer, "time") == 0) {
		printf("%d:%d\n", getHours(), getMinutes());
	} else if(strcmp(shellBuffer, "date") == 0) {
		printf("%d/%d/%d\n", (int)getDay(), (int)getMonth(), (int)getYear());
	} else if(strcmp(shellBuffer, "setTimeZone") == 0) {
		sleep(2000);
		printf("setting time zone...\n");
	} else if(strcmp(shellBuffer, "fractal --zelda") == 0) {
		clear();
		drawCFractalEquilateral(150,768,768,15,0x2A3B8D);
		sleep(1000);
		clear();
	} else if(strcmp(shellBuffer, "help") == 0) {
		printf("%s\n", instructions);
	} else if(strcmp(shellBuffer, "star wars") == 0) {
		printFrame();
	} else if(strcmp(shellBuffer, "gedit") == 0) {
		runGedit();
	} else {
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



