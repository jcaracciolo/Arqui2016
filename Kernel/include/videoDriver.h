#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

/* print msg ('\0' given string) with given coe color */
void print(const char* msg, char colourCode);

/* print any string whith given color */
void printWithLength(const char* msg, int length, char colourCode);

/* prints a number */
void printNum(int num, int colorCode);

/* prints a new line */
void printNewLine();

/* clear the screen */
void clear();

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
