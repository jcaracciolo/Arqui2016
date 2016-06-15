#include "videoDriver.h"
#include "strings.h"
#include "include/strings.h"
#include "include/graphicsDriver.h"
// Colors: backColor_frontColor


#define ROWS 48
#define COLS 85

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

#define MAX_LINE_TO_WRITE 47
#define LINES_TO_SCROLL 3	// works weird if any other number

static char *video = (char *) 0xB8000;
static int cursor = 0;
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
	
	//int i;
	//for (i = 0; i < cursor; i ++) {
	//	drawChar(screen[i],cursorX(i),cursorY(i));
	//}	
	//for (i = (MAX_LINE_TO_WRITE - 1) * COLS; i < COLS * ROWS; i++) {
	//	drawChar(' ',cursorX(i),cursorY(i));
	//}


	cursor -= begincpy;

	//cursor = 0;
	//int i;
	//int j = 0;
	//for (i = LINES_TO_SCROLL * COLS; i < cursor; i++) {
	//	screen[j] = screen[i];
		//printChar(screen[j]);
	//	printChar('h');
	//}
	//for(; i < COLS * ROWS; i++) {
	//	printChar(' ');
	//}
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
		clear();
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
	int i = cursor;

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


