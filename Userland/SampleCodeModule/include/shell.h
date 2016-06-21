#ifndef SHELL_H
#define SHELL_H


const static int ROWS, COLS;

void initShell();

/* adds char c to the buffer */
void addKey(char c);

/* removes from the buffer the last char added to it */
void removeKey();

void execute();

void cleanBuffer();

/* clears the screen */
void clearScreen();

void printScreen();

/* adds a line to the screen */
void addStringToScreen(char* str);

/* adds a char tho the screen */
void addToBuffer(char c);

void addToShellBuffer(char c);

#endif
