#include "videoDriver.h"
#include "strings.h"

static char *video = (char *) 0xB8000;
const int ROWS = 25;
const int COLS = 80;
static int cursor = 0;


/* print '/0' ended string */
void print(const char* msg, char colourCode) {
	int j = 0;
	for (; msg[j] != '\0' ; cursor++) {
		if (cursor >= ROWS * COLS * 2) {
			cursor = 0;
		}

		video[cursor] = msg[j++];
		video[++cursor] = colourCode;
	}

	//printCursor();
}

void printNum(int num, int colorCode) {
	char str[10];
	intToString(str, num);
	print(str, colorCode);
}

void blinkCursor() {
	if (video[cursor+1] == 0x70) {
		video[cursor+1] = 0x07;
	} else {
		video[cursor+1] = 0x70;
	}
}

void removeCursorMark() {
	video[cursor + 1] = 0x07;
}

void printNewLine() {
	removeCursorMark();

	cursor = ((int)(cursor / (COLS * 2)) + 1) * COLS * 2;	// * 2 because video[]
															// reads char and color

	printCursor();
}

void printCursor() {
	video[cursor + 1] = 0x70;
}


void moveCursorUp() {
	removeCursorMark();

	cursor -= COLS * 2;
	cursor = cursor < 0 ? cursor % (COLS*2) + ROWS * COLS * 2: cursor;

	printCursor();
}

void moveCursorDown() {
	removeCursorMark();

	cursor += COLS * 2;
	cursor = cursor > ROWS * COLS * 2 ? cursor % (COLS*2) : cursor;

	printCursor();
}

void moveCursorLeft() {
	removeCursorMark();

	cursor -= 2;
	cursor = cursor < 0 ? 0: cursor;

	printCursor();
}

void moveCursorRight() {
	removeCursorMark();
	cursor += 2;
	cursor = cursor > ROWS * COLS * 2 ? cursor % COLS : cursor;

	printCursor();
}

void backspace() {
	removeCursorMark();

	video[cursor - 2] = ' ';
	video[cursor - 1] = 0x00;
	cursor -= 2;
	cursor = cursor < 0 ? 0 : cursor;

	printCursor();
}

void supr() {
	removeCursorMark();

	cursor += 2;
	video[cursor] = ' ';
	video[cursor + 1] = 0x00;

	printCursor();
}

void printWithLength(const char* msg, int length, char colourCode) {
	int i = cursor;
	int j = 0;
	for (; j < length ; cursor++) {
		video[cursor] = msg[j++];
		video[++cursor] = colourCode;
		if (cursor == ROWS * COLS - 1) {
			cursor = 0;
		}
	}
	
	//printCursor();
}

void clear() {
	int i;
	removeCursorMark();
	for (i = 0; i < ROWS * COLS; i++) {
		print(" ", 0x00);
	}
	cursor = 0;
}


