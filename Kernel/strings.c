#include "strings.h"


/* Converts num into a '\0' ended string in str parameter*/
	void intToString(char* str, int num) {
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

void strcpy(char* str1, char* str2, int length) {
	int i;
	for (i = 0; i < length; i++) {
		str1[i] = str2[i];
	}
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
