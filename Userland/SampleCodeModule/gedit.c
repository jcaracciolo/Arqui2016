#include "include/gedit.h"
#include "../../Kernel/include/types.h"
#include "include/stdvid.h"
#include "include/stdio.h"
#include "../../../Kernel/include/videoDriver.h"

static boolean writeMode = false;

void runGedit() {
	clear();

	boolean ans;
	do {
		printf("\n    Do you want to load saved file?\n  -> ");
		ans = answer();
			sleep(200);
		if (ans == true) {
			clear();
			printSave();
		} else {
			clearText();
			clear();
		}
	} while (ans == EOF);

	while (1) {
		int c = getc();
		if (c != EOF) {
			if (c == '\t') {
				writeMode = !writeMode;				
			} else if (writeMode) {
				if (c == '\b' && getCursorPos() > 0) {
					text[getCursorPos() - 1] = '\0';
				} else if (getCursorPos() < ROWS * COLS) {
					text[getCursorPos()] = c;
				}
				putc(c);
			} else if (c == 'w' || c == 'a' || c == 's' || c == 'd'
						|| c == 'W' || c == 'A' || c == 'S' || c == 'D') {
				navigate(c);
			} else if (c == 'q') {	// quit
				exit();
				sleep(200);
				clear();
				return;
			} else if (c == 'c') {	 // clear
				clear();
			}
		}
	}
}

void navigate(int c) {
	int position = getCursorPos(); 
	switch(c) {
		case 'W':
		case 'w':
			position -= COLS;
			break;
		case 'A':
		case 'a':
			position--;
			break;
		case 'S':
		case 's':
			position += COLS;
			break;
		case 'D':
		case 'd':
			position++;
			break;
	};
	if (position < 0) position = 0;
	position %= COLS * ROWS;
	setCursorPos(position);
}

void exit() {
	clear();
	boolean ans;
	do {
		printf("\n    Do you want to save befor exit? (Y/N)\n  ->");
		ans = answer();
		if (ans == true) {
			saveState();
			return;
		} else {
			clearText();
			return;
		}
	} while(ans == EOF);
}

boolean answer() {
	int c = EOF;
	while (c == EOF) {
		c = getc();
	}
	if (c == 'y' || c == 'Y') {
		putc(c);
		return true;
	} else if (c == 'n' || c == 'N') {
		putc(c);
		return false;
	} else {
		putc(c);
		return EOF;
	}
}

void clearText() {
	for (int i = 0; i < ROWS * COLS; i++) {
		text[i] = '\0';
	}
}

void printSave() {
	setCursorPos(0);
	for (int i = 0; i < COLS * (MAX_LINE_TO_WRITE - 7); i++) {	// Why 7??
		putc(save[i]);
	}
	setCursorPos(0);
}

void saveState() {
	for (int i = 0; i < ROWS * COLS + 1; i++) {
		save[i] = text[i];
	}
}







