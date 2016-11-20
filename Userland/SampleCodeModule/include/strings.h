#ifndef STRINGS_H
#define STRINGS_H

/* Converts num into a '\0' ended string in str parameter*/
int intToString(char* str, int num);

/* Reverse str */
void reverseCharArray(char* str, int length);

/* returns length of a string */
int strlen(const char* str);

/* Returns the length of num ('-' sign is counted whereas '+' sign isn't) */
int numLength(int num);

/* str1: destination, str2: origin */
int strcpy(char* str1, const char* str2, int length);

/* convert str, '\0' ended string, to uppercase */
void toUpper(char* str);

/* compares twoe '\0' ended strings */
int strcmp(const char* str1,const char* str2);

#endif
