#include "shell.h"
#include "../../Kernel/include/types.h"
#include "stdio.h"

#define ROWS 25
#define COLS 80

char arr[ROWS * COLS] = {0};
char shellBuffer[COLS + 1] = {0};	// + 1 to add '\0' at the end
int index = 0;

void initShell() {
//	printf("HOLa!!", 6);

	while(1) {
		int c = getc();
		if (c != EOF) {
			if (c == '\n') {
				execute();
			} else {
				addToShellBuffer(c);
				putc(c);
			}
		}
	}
}

void addToShellBuffer(char c) {
	shellBuffer[index] = c;
	index = (index + 1) % COLS;
}

void execute() {
	print("execute!",8);
}
