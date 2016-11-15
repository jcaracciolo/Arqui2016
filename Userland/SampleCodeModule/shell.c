#include "include/shell.h"
#include "types.h"
#include "include/stdio.h"
#include "include/strings.h"
#include "include/stdvid.h"
#include "include/stdtime.h"
#include "include/sw.h"
#include "include/gedit.h"
#include "include/paint.h"
#include "include/sync.h"
#include "include/philosophers.h"
#include "include/stdlib.h"

int rand=1;

const char* instructions = " func                - print a simple message (completly useless)\n\
 echo *param*        - prints message on console\n\
 clear               - clears the screen\n\
 time                - get current system time\n\
 setTimeZone *param* - set computer's time zone\n\
 date                - get current system date\n\
 fractal Zelda       - draws a triforce inspirated fractal\n\
 multifractal        - multipled colored fractals in a cool animation\n\
 star wars           - little star wars animation\n\
 gedit               - simple text editor\n\
 paint               - simple keyboard controlled paint\n\
 paintBg *param*     - paint the console background (only once)\n\
 setupFont *param*   - setup font to write\n\
 ps                  - list all processes\n\
 kill *pid* *msg*	- send a message to a process";

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
	leave();
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

	rand+=rand*3^getSeconds()*500;
	drawCFractalEquilateral(150,768,768,9,rand);
	leave();
}

void func(int cargs, void ** pargs) {
	printf("execute! cargs:%d arg:%s\n", cargs, *((int*)pargs));
	leave();
}

void printTime() {	
	printf("%d:%d\n", getHours(), getMinutes());
	leave();
}

void printDate() {
	printf("%d/%d/%d\n", getDay(), getMonth(), getYear());
	leave();
}

/* (pargs)[0] -> tz */
void callSetTimeZone(int cargs, void ** pargs) {
	int tz = *((int*)(pargs[1]));	// TODO revisar que funcione bien
	printf("changin tz to %d\n", tz);
	if(tz>=-12 && tz<=12) {
		setTimeZone(tz);
		printf("\nTime set correctly to GTM %d \n",tz);
	} else{
		printf("\nInput error\n");
	}
	leave();
}

void printInstructions() {
		printf("%s\n", instructions);
		leave();
}

void callRunGedit() {
	runGedit();
	leave();
}

void paintBg(int carg, void ** pargs) {
	drawCSquare(0,0,768,1024,*((int*)(args[1]))); // TODO revisar que ande
	setCursorPos(0);
	leave();
}

void callChangeFont(int carg, void ** pargs) {
	changeFont(*((int*)(args[1])));	// TODO revisar que ande
	leave();
}

void callEcho(int carg, void ** pargs) {
	printf(*((char*)(pargs[1])));	// TODO revisar que ande
	printf("\n");
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
		void* parg = malloc(sizeof(int));
		*parg = "func";
		exec(&func, 1, &parg);
	} else if (strcmp(shellBuffer, "clear") == 0) {
		void* parg = malloc(sizeof(int));
		*parg = "clear";
		exec(&clearScreen, 1, &parg);
	} else if(strcmp(shellBuffer, "time") == 0) {
		void* parg = malloc(sizeof(int));
		*parg = "time";
		exec(&printTime, 1, &parg);
	} else if(strcmp(shellBuffer, "date") == 0) {
		void* parg = malloc(sizeof(int));
		*parg = "date";	
		exec(&printDate, 1, &parg);
	} else if(sscanf("setTimeZone %d",shellBuffer,&tz)==1) {
		int * parg = malloc(sizeof(int) * 2);
		parg[0] = "setTimeZone";
		parg[1] = tz;
		exec(&callSetTimeZone, 2, &tz); // TODO revisar
	} else if(strcmp(shellBuffer, "fractal Zelda") == 0) {
		void* parg = malloc(sizeof(int));
		*parg = "fractal";
		clear();
		int pid = exec(&drawFractal, 1, &parg);

		//sleep(1000);
		//clear();
	} else if(strcmp(shellBuffer, "multifractal") == 0) {
		clear();
		for(int i=0;i<20;i++) {
			void* parg = malloc(sizeof(int));
			*parg = "colorfractal";
			int pid = exec(&drawFractalc, 1, &parg);
			//TODO sleep
			int n = 4000000;
			while (n--);
		}
		//clear();

	
	} else if(sscanf("kill %d %d",shellBuffer,&pidToKill, &msg)==1){
		void* parg = malloc(sizeof(int)*2);
		parg[0] = "kill";
		parg[2] = pidToKill;
		parg[3] = msg;
		exec(&kill, 3, &parg);	// TODO revisar que funcione
	} else if(strcmp(shellBuffer, "help") == 0) {
		void* parg = malloc(sizeof(int));
		parg[0] = "help";
		exec(&printInstructions, 1, &parg); // TODO revisar que funcione

	} else if(strcmp(shellBuffer, "star wars") == 0) {
		void* parg = malloc(sizeof(int));
		parg[0] = "starwars";
		exec(&playStarWars, 1, &parg);

	} else if(strcmp(shellBuffer, "gedit") == 0) {
		void* parg = malloc(sizeof(int));
		parg[0] = "gedit";
		exec(&callRunGedit, 1, &parg);	// TODO revisar si funciona
	} else if(strcmp(shellBuffer, "paint") == 0) {
		void* parg = malloc(sizeof(int));
		parg[0] = "paint";
		exec(&callPaintLoop, 1, &parg);	// TODO revisar que funcione
	} else if(sscanf("paintBg %d", shellBuffer, &num) == 1) {
		void* parg = malloc(sizeof(int) * 2);
		parg[0] = "paintBg";
		parg[1] = num;
		exec(&paintBg, 2, parg);	// TODO revisar que funcione
	} else if(sscanf("setupFont %d", shellBuffer, &num) == 1) {
		void* parg = malloc(sizeof(int) * 2);
		parg[0] = "setupFont";
		parg[1] = num;
		exec(callChangeFont, 2, &parg); // TODO revisar que funcione
	}else if(sscanf("echo %s",shellBuffer,arr)==1) {
		void* parg = malloc(sizeof(int) * 2);
		parg[0] = "echo";
		parg[1] = arr;
		exec(&callEcho, 2, parg);	// TODO revisar que funcione
	} else if(strcmp(shellBuffer, "ps") == 0) {
		void* parg = malloc(sizeof(int));
		parg[0] = "ps";
		exec(&ps, 1, parg);	// TODO revisar que funcione
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
