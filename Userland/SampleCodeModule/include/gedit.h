#ifndef GEDIT_H
#define GEDIT_H

#include "../../../Kernel/include/videoDriver.h"

static char save[ROWS * COLS + 1] = " New to gedit? Here are some tips...\n\
    -> Tab to enter/exit writeMode\n\n\
        In writeMode\n\
         |\n\
         |-> wasd to navigate\n\
         |-> c to clear screen\n\
         |-> q to quit\n";

static char text[ROWS * COLS + 1] = " New to gedit? Here are some tips...\n\
    -> Tab to enter/exit writeMode\n\n\
        In writeMode\n\
         |\n\
         |-> wasd to navigate\n\
         |-> c to clear screen\n\
         |-> q to quit\n";

void runGedit();
void navigate(int c);
void exit();
void clearText();
void printSave();
void saveState();
boolean answer();

#endif
