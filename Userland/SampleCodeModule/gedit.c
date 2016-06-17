
#include "../../Kernel/include/types.h"
#include "../../Kernel/include/videoDriver.h"
#include "include/stdvid.h"
#include "include/stdio.h"


static boolean writeMode = false;
static char text[ROWS * COLS] = {0};

void runGedit() {
	clear();
	
	while (1) {
		int c = getc();
		if (c != EOF) {
			if (c == '|') {
				printf("changeMode\n");
			} else if (c == 'e') {
				clear();
				return;
			}
		}
	}
}
