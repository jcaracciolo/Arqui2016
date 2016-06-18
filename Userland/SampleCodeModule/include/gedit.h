#ifndef GEDIT_H
#define GEDIT_H

#include "../../../Kernel/include/videoDriver.h"
#include "../../../Kernel/include/types.h"

static boolean firstTime = true; 
static char save[ROWS * COLS + 1] = {0};
static char text[ROWS * COLS + 1] = {0};

void runGedit();
void navigate(int c);
void exit();
void clearText();
void printSave();
void saveState();
boolean answer();
int fillLines(char* str, int line);
void fillSpaces(int line, int begin);
boolean outOfBounds();

#endif
