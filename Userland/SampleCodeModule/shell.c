#include "include/shell.h"
#include "../../Kernel/include/types.h"
#include "include/stdio.h"
#include "include/strings.h"
#include "include/stdlib.h"
#include "include/stdvid.h"
#include "include/sw.h"

#define ROWS 48
#define COLS 85

extern void int80(qword rax, qword rdi, qword rsi, qword rdx, qword r10, qword r8, qword r9);

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
		printf("%d/%d/%d\n", getDay(), getMonth(), getYear());
	} else if(strcmp(shellBuffer, "setTimeZone") == 0) {
		sleep(2);
		printf("setting time zone...\n");
	} else if(strcmp(shellBuffer, "star wars") == 0) {
		printFrame();
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



