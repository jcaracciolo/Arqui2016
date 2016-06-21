#ifndef STDIN_H
#define STDIN_H

#include <stdarg.h>

void putc(char c);

int getc();

void printCharArray(const char* arr, int length);

int printf(const char* format,...);

char* readLine() ;

char* readInt(char* string, int* num);

int scanf(const char* format,...);

int sscanf(const char* format, const char* str, ...);
#endif
