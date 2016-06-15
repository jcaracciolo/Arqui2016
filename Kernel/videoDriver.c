#include "videoDriver.h"
#include "strings.h"
#include "include/strings.h"


#define ROWS 25
#define COLS 80

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

#define MAX_LINE_TO_WRITE 23
#define LINES_TO_SCROLL 1	// works weird if any other number

static char *video = (char *) 0xB8000;
static int cursor = 0;
static char screen[COLS * ROWS] = {2};

void scroll() {
	int begincpy = LINES_TO_SCROLL * COLS;
	int length = COLS * ROWS - begincpy;
	strcpy(screen, &(screen[begincpy]), length);
	cursor -= begincpy * 2;
}

/* print '/0' ended string */
void print(const char* msg, char colourCode) {
	int j = 0;
	for (; msg[j] != '\0' ; cursor++) {
		if (cursor >= ROWS * COLS * 2) {
			cursor = 0;
		}
		screen[cursor/2] = msg[j];
		video[cursor++] = msg[j++];
		video[cursor++] = colourCode;
		if (cursor/2 >= MAX_LINE_TO_WRITE * COLS) {
		clear();
		scroll();
		printScreenArray();
	}
		cursor = cursor % (ROWS * COLS * 2);
	}

	//printCursor();
}

void printChar(int c){
	if(c=='\b'){
		backspace();
	}else if(c=='\n'){
		printNewLine();
	}else{
		screen[cursor/2] = (char)c;
		video[cursor++] = (char)c;
		video[cursor++] = 0x07;
		//screenIndex++;
	}
	if (cursor/2 >= MAX_LINE_TO_WRITE * COLS) {
		clear();
		scroll();
		printScreenArray();
	}
	//cursor = cursor % (ROWS * COLS * 2);
}

void printScreenArray() {
	for (int i = 0; i < COLS * ROWS * 2; i += 2) {
		video[i] = screen[i/2];
		video[i+1] = cc(BLACK,WHITE);
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
		//screen[screenIndex++] = msg[j];
		video[cursor++] = msg[j++];
		video[cursor++] = colourCode;
		if (cursor == ROWS * COLS - 1) {
			cursor = 0;
		}
	}
	
	//printCursor();
}

void clear() {
	int i;
	removeCursorMark();
	for (i = 0; i < ROWS * COLS * 2; i+=2) {
		video[i] = ' ';
		video[i+1] = cc(BLACK,WHITE);
	}
}


