#ifndef STRINGS_H
#define STRINGS_H

/* Converts num into a '\0' ended string in str parameter*/
void intToString(char* str, int num);

/* Reverse str */
void reverseCharArray(char* str, int length);

/* Returns the length of num ('-' sign is counted whereas '+' sign isn't) */
int numLength(int num);

/* str1: destination, str2: origin */
int strcpy(char* str1, const char* str2, int length);

/* convert str, '\0' ended string, to uppercase */
void toUpper(char* str);


int strcmp(const char* str1, const char* str2) ;

int strlen(const char* str) ;


#endif
