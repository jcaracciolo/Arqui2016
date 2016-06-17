#include "videoDriver.h"
#include "include/videoDriver.h"
#include "strings.h"
#include "include/strings.h"
#include "include/graphicsDriver.h"
// Colors: backColor_frontColor

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
#define cursorX(a) (((a)-((a)/COLS)*COLS)*CHAR_WIDTH)
#define cursorY(a) (((a)/COLS)*CHAR_HEIGHT)
#define cc(a,b) (a)*0x10+(b)

#define CURSOR_WIDTH (CHAR_WIDTH*FONT_SCALE)
#define CURSOR_HEIGHT (CHAR_HEIGHT*FONT_SCALE)

static char *video = (char *) 0xB8000;
static int cursor = 0;
static boolean on = false;
static char screen[COLS * ROWS] = {0};



void scroll() {
	int begincpy = LINES_TO_SCROLL * COLS;
	int length = COLS * ROWS - begincpy;
	//strcpy(screen, &(screen[begincpy]), length);
	 int j;
	for (int i = begincpy,j = 0; j < length; i++, j++) {
		screen[j] = screen[i];
	}
	for (j = length; j < ROWS * COLS; j++) {
		screen[j] = ' ';
	}
	cursor -= begincpy;
}

void setCursorPosition(int pos) {
	cursor = pos;
}

int getCursorPosition() {
	return cursor;
}

/* print '/0' ended string */
void print(const char* msg) {
	for (int j = 0; msg[j] != '\0' ; j++) {

        printChar(msg[j]);
	}

	//printCursor();
}

void printChar(int c){
	if(c=='\b'){
		backspace();
	}else if(c=='\n'){
		printNewLine();
	}else{
		drawChar(c,cursorX(cursor),cursorY(cursor));

		screen[cursor++] = (char)c;
//		video[cursor++] = (char)c;
//		video[cursor++] = 0x07;
	}
	if (cursor >= MAX_LINE_TO_WRITE * COLS) {
		//clear();
		scroll();
		printScreenArray();
	}
	//cursor = cursor % (ROWS * COLS * 2);
}

void printScreenArray() {

	for (int i = 0; i < COLS * ROWS; i ++) {
		drawChar(screen[i],cursorX(i),cursorY(i));
//		video[i+1] = cc(BLACK,WHITE);
	}

    //cursor = 0;
	//for (int i = 0; i < COLS * ROWS ; i += 1) {
	//	printChar(screen[i]);
//		video[i+1] = cc(BLACK,WHITE);
	//}
}

void printNum(int num) {
	char str[10];
	intToString(str, num);
	print(str);
}

void blinkCursor() {
	Color black = {.r = 0x00, .g = 0x00 , .b = 0x0};

	on = !on;
	if(on)
		_drawSquare(cursorX(cursor) * FONT_SCALE, cursorY(cursor) * FONT_SCALE,
					CURSOR_HEIGHT, CURSOR_WIDTH);
	else
		_drawCSquare(cursorX(cursor) * FONT_SCALE, cursorY(cursor) * FONT_SCALE,
					 CURSOR_HEIGHT, CURSOR_WIDTH, black);
}

//TODO REDO
void removeCursorMark() {
	Color black = {.r = 0x00, .g = 0x00 , .b = 0x0};
	_drawCSquare(cursorX(cursor) * FONT_SCALE, cursorY(cursor) * FONT_SCALE,
				 CURSOR_HEIGHT, CURSOR_WIDTH, black);
}

void printNewLine() {
	removeCursorMark();

	cursor = ((int)(cursor / COLS) + 1) * COLS;	

	printCursor();
}
//TODO REDO
void printCursor() {
	video[cursor + 1] = cc(WHITE,BLACK);
}


void moveCursorUp() {
	removeCursorMark();

	cursor -= COLS;
	cursor = cursor < 0 ? cursor % COLS + ROWS * COLS : cursor;

	printCursor();
}

void moveCursorDown() {
	removeCursorMark();

	cursor += COLS;
	cursor = cursor > ROWS * COLS ? cursor % COLS : cursor;

	printCursor();
}

void moveCursorLeft() {
	removeCursorMark();

	cursor--;
	cursor = cursor < 0 ? 0: cursor;

	printCursor();
}

void moveCursorRight() {
	removeCursorMark();
	cursor++;
	cursor = cursor > ROWS * COLS ? cursor % COLS : cursor;

	printCursor();
}

void backspace() {
	removeCursorMark();
	if(cursor>= 1) cursor--;
	printChar(' ');
	cursor--;
	cursor = cursor < 0 ? 0 : cursor;

	printCursor();
}

//TODO
void supr() {
	removeCursorMark();

	printChar(' ');

	printCursor();
}

void printWithLength(const char* msg, int length) {

	for (int j = 0; j < length ; j++) {
        printChar(msg[j]);
	}
}

void clear() {
	int i;
	removeCursorMark();
	clearScreen();
}

void clearChars() {
	for (int i = 0; i < COLS * ROWS; i++) {
		screen[i] = 0;
	}
	cursor = 0;
}

