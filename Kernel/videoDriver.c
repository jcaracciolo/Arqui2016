#include "videoDriver.h"
#include "strings.h"
#include "include/strings.h"

// Colors: backColor_frontColor

static char *video = (char *) 0xB8000;
const int ROWS = 25;
const int COLS = 80;
static int cursor = 0;

#define BLACK 0
#define BLUE 1
#define GREEB 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_AQUA 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

#define cc(a,b) (a)*0x10+(b)


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

void printChar(int c){
	if(c=='\b'){
		backspace();
	}else if(c=='\n'){
		printNewLine();
	}else{
	video[cursor++] = (char)c;
	video[cursor++] = 0x07;
	}
}

void printNum(int num, int colorCode) {
	char str[10];
	intToString(str, num);
	print(str, colorCode);
}

void blinkCursor() {
	if (video[cursor+1] == cc(BLACK,WHITE)) {
		video[cursor+1] = cc(WHITE,BLACK);
	} else {
		video[cursor+1] = cc(BLACK,WHITE);
	}
}

void removeCursorMark() {
	video[cursor + 1] = cc(BLACK,WHITE);
}

void printNewLine() {
	removeCursorMark();

	cursor = ((int)(cursor / (COLS * 2)) + 1) * COLS * 2;	// * 2 because video[]
															// reads char and color

	printCursor();
}

void printCursor() {
	video[cursor + 1] = cc(WHITE,BLACK);
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
	video[cursor - 1] = cc(BLACK,BLACK);
	cursor -= 2;
	cursor = cursor < 0 ? 0 : cursor;

	printCursor();
}

void supr() {
	removeCursorMark();

	cursor += 2;
	video[cursor] = ' ';
	video[cursor + 1] = cc(BLACK,BLACK);

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
		print(" ", cc(BLACK,BLACK));
	}
	cursor = 0;
}


