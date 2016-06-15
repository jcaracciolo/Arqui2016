#include "videoDriver.h"
#include "strings.h"
#include "include/strings.h"
#include "include/graphicsDriver.h"
// Colors: backColor_frontColor


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
#define cursorX(a) (((a)-((a)/COLS)*COLS)*CHAR_WIDTH)
#define cursorY(a) (((a)/COLS)*CHAR_HEIGHT)
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
	for (int j = 0; msg[j] != '\0' ; j++) {

//		video[cursor++] = msg[j++];
//		video[cursor++] = colourCode;
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

		screen[cursor/2] = (char)c;
//		video[cursor++] = (char)c;
//		video[cursor++] = 0x07;
        cursor++;
	}
	if (cursor/2 >= MAX_LINE_TO_WRITE * COLS) {
		clear();
		scroll();
		printScreenArray();
	}
	//cursor = cursor % (ROWS * COLS * 2);
}

void printScreenArray() {

//	for (int i = 0; i < COLS * ROWS *2; i += 2) {
//		drawChar(screen[i/2],cursorX(i),cursorY(i));
////		video[i+1] = cc(BLACK,WHITE);
//	}
    cursor = 0;
	for (int i = 0; i < COLS * ROWS ; i += 1) {
		printChar(screen[i]);
//		video[i+1] = cc(BLACK,WHITE);
	}
}

void printNum(int num, int colorCode) {
	char str[10];
	intToString(str, num);
	print(str, colorCode);
}
//TODO REDO
void blinkCursor() {
	if (video[cursor+1] == cc(BLACK,WHITE)) {
		video[cursor+1] = cc(WHITE,BLACK);
	} else {
		video[cursor+1] = cc(BLACK,WHITE);
	}
}

//TODO REDO
void removeCursorMark() {
	video[cursor + 1] = cc(BLACK,WHITE);
}

void printNewLine() {
	removeCursorMark();

	cursor = ((int)(cursor / (COLS * 2)) + 1) * COLS * 2;	// * 2 because video[]
															// reads char and color

	printCursor();
}
//TODO REDO
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
	if(cursor>= 2) cursor -=1;
	printChar(' ');
//	video[cursor - 2] = ' ';
//	video[cursor - 1] = cc(BLACK,BLACK);
	cursor -= 1;
	cursor = cursor < 0 ? 0 : cursor;

	printCursor();
}

//TODO
void supr() {
	removeCursorMark();

	printChar(' ');

//	video[cursor] = ' ';
//	video[cursor + 1] = cc(BLACK,BLACK);

	printCursor();
}

void printWithLength(const char* msg, int length, char colourCode) {
	int i = cursor;

	for (int j = 0; j < length ; j++) {
		//screen[screenIndex++] = msg[j];
//		video[cursor++] = msg[j++];
//		video[cursor++] = colourCode;
        printChar(msg[j]);
	}
	
	//printCursor();
}

void clear() {
	int i;
	removeCursorMark();
	clearScreen();
}


