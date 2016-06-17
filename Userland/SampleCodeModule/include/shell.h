#ifndef SHELL_H
#define SHELL_H

#include "../../../Kernel/include/graphicsDriver.h"

#define ROWS ((WIN_HEIGHT / CHAR_HEIGHT) / FONT_SCALE)
#define COLS ((WIN_WIDTH / CHAR_WIDTH) / FONT_SCALE)

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
