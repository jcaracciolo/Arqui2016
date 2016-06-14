#ifndef STRINGS_H
#define STRINGS_H

/* Converts num into a '\0' ended string in str parameter*/
void intToString(char* str, int num);

/* Reverse str */
void reverseCharArray(char* str, int length);

/* returns length of a string */
int strlen(char* str);

/* Returns the length of num ('-' sign is counted whereas '+' sign isn't) */
int numLength(int num);

/* str1: destination, str2: origin */
void strcpy(char* str1, char* str2, int length);

/* convert str, '\0' ended string, to uppercase */
void toUpper(char* str);

/* compares twoe '\0' ended strings */
int strcmp(char* str1, char* str2);

#endif
