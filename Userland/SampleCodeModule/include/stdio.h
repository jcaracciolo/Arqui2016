#ifndef STDIN_H
#define STDIN_H

#include <stdarg.h>

void putc(char c);

int getc();

void print(const char* arr, int length);

int printf(const char* format,...);

#endif
