#include "include/shell.h"
#include "../../Kernel/include/types.h"
#include "include/stdio.h"
#include "include/strings.h"
#include "include/stdlib.h"

#define ROWS 25
#define COLS 80
#define MAX_LINE_TO_WRITE 20
#define LINES_TO_SCROLL 2

char screen[ROWS * COLS] = {0};
int screenIndex = 0;

char shellBuffer[COLS + 1] = {0};	// + 1 to add '\0' at the end
int shellIndex = 0;

void initShell() {
	clearScreen();
	//print("WELCOME THE SHELL!!\n  >>", 24);
	addStringToScreen("WELCOME THE SHELL!!\n  >>");

	while(1) {
		int c = getc();
		if (c != EOF) {
			if (c == '\n') {
				execute();
			} else if (c == '\b') {
				removeKey();
			} else {
				addKey(c);
			}
		}
		printScreen();
	}
}

void clearScreen() {
	int i;	
	for (i = 0; i < ROWS * COLS; i++) {
		screen[i] = ' ';
	}
	shellIndex = 0;
	screenIndex = 0;
}

void removeKey() {
	if (shellIndex != 0) {
		shellIndex--;
	}
	if (screenIndex != 0) {
		screen[--screenIndex] = ' ';
	}
	putc('\b');
}

void addKey(char c) {
	if (shellIndex <= COLS) {
		shellBuffer[shellIndex++] = c;
		screen[screenIndex++] = c;
	}
}

void addToScreen(char c) {
	screen[screenIndex++] = c;
	screenIndex %= COLS * ROWS;
}

void addStringToScreen(char* str) {
	while (*str != '\0') {
		if (*str == '\n') {
			screenIndex = ((screenIndex / COLS) * COLS + COLS);
		} else {
			screen[screenIndex++] = *str;
		}
		screenIndex %=ROWS * COLS;
		str++;
	}
}

void execute() {
	shellBuffer[shellIndex] = '\0';
	addStringToScreen("\n");
	//putc('\n');
	if (strcmp(shellBuffer, "func") == 0) {
		addStringToScreen("execute!\n");
		//print("execute!\n",9);
		printf("hola %d",10);
	} else if (strcmp(shellBuffer, "clear") == 0) {
		clearScreen();
	} else {
		addStringToScreen("Command not found.\n");
		//print("Command not found.\n", 19);
	}
	addStringToScreen("  >>");
	//print("  >>", 4);
	cleanBuffer();
}

void cleanBuffer() {
	int i;
	for (i = 0; i < COLS + 1; i++) {
		shellBuffer[i] = '\0';
	}
	shellIndex = 0;
}


void printScreen() {
	int line = screenIndex / COLS;	
	if (line >= MAX_LINE_TO_WRITE) {
		scroll();
	}
	for (int i = 0; i < ROWS * COLS; i++) {
		putc(screen[i]);
	}
}

void scroll() {
	int begincpy = LINES_TO_SCROLL * COLS;
	int length = COLS * ROWS - begincpy;
	strcpy(screen, &(screen[begincpy]), length);
	screenIndex -= begincpy;
}



