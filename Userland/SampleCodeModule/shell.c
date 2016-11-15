#include "include/shell.h"
#include "types.h"
#include "include/stdio.h"
#include "include/strings.h"
#include "include/stdlib.h"
#include "include/stdvid.h"
#include "include/stdtime.h"
#include "include/sw.h"
#include "include/gedit.h"
#include "include/paint.h"
#include "include/sync.h"
#include "include/philosophers.h"

int rand=1;

const char* instructions = " func                - print a simple message (completly useless)\n\
 echo *param*        - prints message on console\n\
 clear               - clears the screen\n\
 time                - get current system time\n\
 setTimeZone *param* - set computer's time zone\n\
 date                - get current system date\n\
 fractal Zelda       - draws a triforce inspirated fractal\n\
 star wars           - little star wars animation\n\
 gedit               - simple text editor\n\
 paint               - simple keyboard controlled paint\n\
 paintBg *param*     - paint the console background (only once)\n\
 setupFont *param*   - setup font to write\n\
 kill *pid* *msg*	 - send a message to a process";

extern void int80(qword rdi, qword rsi, qword rdx, qword rcx, qword r8, qword r9);

char screen[ROWS * COLS] = {0};
int screenIndex = 0;

char shellBuffer[COLS + 1] = {0};	// + 1 to add '\0' at the end
int shellIndex = 0;


void initShell() {
	//clearScreen();
	printf("-- WELCOME THE SHELL --\n\n  >>");
	setTimeZone(-3);
	setConsoleSize();

	while(1) {
		int c = getc();
		if (c != EOF) {
			if (c == '\n') {
				execute();
			} else if (c == '\b') {
				removeKey();
			} else {
				addToShellBuffer(c);
				putc(c);
			}
		}
	}
}

void clearScreen() {
	clear();
	shellIndex = 0;
}

void removeKey() {
	if (shellIndex != 0) {
		shellIndex--;
		putc('\b');
	}
}

void addToShellBuffer(char c) {
	if (shellIndex <= COLS) {
		shellBuffer[shellIndex++] = c;
	}
}

void drawFractal() {	
	drawCFractalEquilateral(150,768,768,9,readData());

	leave();
}

void drawFractalc() {

	rand=rand*3+getSeconds()*500;
	drawCFractalEquilateral(150,768,768,9,rand);

	leave();
}

void execute() {
	char arr[100];
	int number = 0;

	int tz;
	shellBuffer[shellIndex] = '\0';
	int num, pidToKill, msg;
	putc('\n');
	if (strcmp(shellBuffer, "func") == 0) {
		printf("execute!\n");
	} else if (strcmp(shellBuffer, "clear") == 0) {
		clearScreen();
	} else if(strcmp(shellBuffer, "time") == 0) {
		printf("%d:%d\n", getHours(), getMinutes());
	} else if(strcmp(shellBuffer, "date") == 0) {
		printf("%d/%d/%d\n", getDay(), getMonth(), getYear());
	} else if(sscanf("setTimeZone %d",shellBuffer,&tz)==1) {
		if(tz>=-12 && tz<=12) {
			setTimeZone(tz);
			printf("\nTime set correctly to GTM %d \n",tz);
		} else{
			printf("\nInput error\n");
		}
	} else if(strcmp(shellBuffer, "fractal Zelda") == 0) {
		clear();
		int pid = exec(&drawFractal);

		//sleep(1000);
		//clear();
	} else if(strcmp(shellBuffer, "multifractal") == 0) {
		clear();
		for(int i=0;i<20;i++){
			int pid = exec(&drawFractalc);
			//TODO sleep
			int n=4000000;
			while(n--);
		}

	} else if(sscanf("kill %d %d",shellBuffer,&pidToKill)==1){
		printf("killing %d %d\n", pidToKill, msg);
		kill(pidToKill, msg);
	} else if(strcmp(shellBuffer, "help") == 0) {
		printf("%s\n", instructions);

	} else if(strcmp(shellBuffer, "star wars") == 0) {
    philosphers();
//		swStart();
	} else if(strcmp(shellBuffer, "gedit") == 0) {
		number = createMutex("sad");

		printf("number: %d",number);
//		runGedit();
	} else if(strcmp(shellBuffer, "paint") == 0) {
		paintLoop();
	} else if(sscanf("paintBg %d", shellBuffer, &num) == 1) {
			drawCSquare(0,0,768,1024,num);
			setCursorPos(0);
	} else if(sscanf("setupFont %d", shellBuffer, &num) == 1) {
		changeFont(num);
	}else if(sscanf("echo %s",shellBuffer,arr)==1) {
		printf(arr);
		printf("\n");
	} else if(strcmp(shellBuffer, "ps") == 0) {
		ps();
	}else {
		printf("Command not found.\n");
	}
	printf("  >>");
	cleanBuffer();
}
void cleanBuffer() {
	int i;
	for (i = 0; i < COLS + 1; i++) {
		shellBuffer[i] = '\0';
	}
	shellIndex = 0;
}
