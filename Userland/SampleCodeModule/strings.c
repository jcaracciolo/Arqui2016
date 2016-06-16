#include "include/strings.h"


/* Converts num into a '\0' ended string in str parameter*/
	int intToString(char* str, int num) {
	int length = numLength(num);
	int numLength = length;
	int i;

	if (num < 0) {
		str[length - 1] = '-';
		num *= -1;
		numLength--;
	}

	for (i = 0; i < numLength; i++) {
		str[i] = (char)(num % 10 + '0');
		num /= 10;
	}

	reverseCharArray(str, length);
	str[length] = '\0';
	return length;
}

int strlen(const char* str) {
	int length = 0;
	while (*(str++) != '\0') {
		length++;
	}
	return length;
}

void reverseCharArray(char* str, int length) {
	char aux[length];
	int i;
	strcpy(aux, str, length);

	for (i = 0; i < length; i++) {
		str[length - 1 - i] = aux[i];
	}
}

void toUpper(char* str) {
	while(*str) {
		if (*str >= 'a' && *str <= 'z') {
			*str += 'A' - 'a';
		}
		str++;
	}
}

void strcpy(char* str1,const char* str2, int length) {
	int i;
	for (i = 0; i < length; i++) {
		str1[i] = str2[i];
	}
}

int strcmp(const char* str1, const char* str2) {
	while (*str1 != '\0') {
		if (*str2 == '\0') return -1;
		if (*str1 < *str2) return -1;
		if (*str2 < *str1) return 1;
		str1++; str2++;
	}
	if (*str2 == '\0') return 0;
	return -1;	
}

int numLength(int num) {
	int length = 1;

	if (num < 0) {
		length += 1;
		num *= -1;
	}

	while(num / 10 != 0) {
		length += 1;
		num /= 10;
	}
	return length;
}
