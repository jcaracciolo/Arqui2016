#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include "graphicsDriver.h"

#define ROWS ((WIN_HEIGHT / CHAR_HEIGHT) / FONT_SCALE)
#define COLS ((WIN_WIDTH / CHAR_WIDTH) / FONT_SCALE)

/* print msg ('\0' given string) with given coe color */
void print(const char* msg);

/* print any string whith given color */
void printWithLength(const char* msg, int length);

/* prints a number */
void printNum(int num);

/* prints a new line */
void printNewLine();

/* clear the screen */
void clear();

/* clears the matrix of chars */
void clearChars();

void printScreenArray();

/* Makes cursor blink */
void blinkCursor();

void printCursor();

void backspace();

void supr();

/* Move Cursor*/
void moveCursorRight();

void moveCursorUp();
void moveCursorLeft();
void moveCursorDown();
void moveCursorRight();

void printChar(int c);

#endif
