#include "include/gedit.h"
#include "types.h"
#include "include/stdvid.h"
#include "include/stdio.h"
#include "../../../Kernel/include/videoDriver.h"

static boolean writeMode = false;

void initInstructions();

void runGedit() {
	clear();

	if (firstTime) {
		initInstructions();
		firstTime = false;
	}

	boolean ans;
	do {
		printf("\n    Do you want to load saved file? (Y/N)\n  -> ");
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
			} else if (writeMode && !outOfBounds()) {
				if (c == '\b' && getCursorPos() > 0) {
					text[getCursorPos() - 1] = '\0';
				} else if (getCursorPos() < ROWS * COLS && c != '\n') {
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
				clearText();
			}
		}
	}
}

boolean outOfBounds() {
	int currentLine = (getCursorPos() / COLS) * COLS;
	if (currentLine < (MAX_LINE_TO_WRITE - 1) * COLS) return false;
	return true;
}

void initInstructions() {
	char* lines[7] = {" New to gedit? Here are some tips...",
                  "    -> Tab to enter/exit writeMode",
                  "        In writeMode",
                  "         |",
                  "         |-> wasd to navigate",
                  "         |-> c to clear screen",
                  "         |-> q to quit"};

	char* line1 = " New to gedit? Here are some tips...";
    char* line2 = "    -> Tab to enter/exit writeMode";
    char* line3 = "        In writeMode";
    char* line4 = "         |";
    char* line5 = "         |-> wasd to navigate";
    char* line6 = "         |-> c to clear screen";
    char* line7 = "         |-> q to quit";

	for (int i = 0; i < 7; i++) {
		int cantChars = fillLines(lines[i], i);
		fillSpaces(i, cantChars);
	}
	
}

int fillLines(char* str, int line) {
	int i = 0;
	while(*(str++) != '\0') {
		save[line * COLS + i] = *(str);
		text[line * COLS + i] = *(str);
		i++;
	}
	return i;
}

void fillSpaces(int line, int begin) {
	int from = line * COLS + begin;
	int to = (line + 1) * COLS;
	for (int i = from; i < to; i++) {
		save[i] = ' ';
		text[i] = ' ';
	}
} 

void navigate(int c) {
	int position = getCursorPos();
	int mov;
	switch(c) {
		case 'W':
		case 'w':
			mov = -COLS;
			break;
		case 'A':
		case 'a':
			mov = -1;;
			break;
		case 'S':
		case 's':
			mov = COLS;
			break;
		case 'D':
		case 'd':
			mov = 1;
			break;
	};
	if (position + mov >= (MAX_LINE_TO_WRITE - 1) * COLS) return;
	position += mov;
	setCursorPos(position);

	//if (position < 0) position = 0;
	//if (position >= ROWS * COLS) position = COLS * ROWS - 1;
	//setCursorPos(position);
}

void exit() {
	clear();
	boolean ans;
	do {
		printf("\n    Do you want to save before exit? (Y/N)\n  ->");
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
	clear();
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







