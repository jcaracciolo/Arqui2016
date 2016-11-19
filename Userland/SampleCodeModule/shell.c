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
#include "include/prodCons.h"
#include "include/donPepito.h"
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
 newps               - create a useless process\n\
 philo               - simulation of philosophers problem\n\
 prod                - simulation of producer-consumer problem\n\
 pepito              - Don Pepito and Don Jose conversation\n\
 kill *pid* *msg*	- send a message to a process";

extern void int80(qword rdi, qword rsi, qword rdx, qword rcx, qword r8, qword r9);

char screen[ROWS * COLS] = {0};
int screenIndex = 0;

char sbuffer[COLS + 1] = {0};	// + 1 to add '\0' at the end
char * shellBuffer;
int shellIndex = 0;


void initShell() {
	//clearScreen();
	printf("-- WELCOME THE SHELL --\n\n  >>");
	setTimeZone(-3);
	setConsoleSize();
	shellBuffer = sbuffer[0];
	cleanBuffer();

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
		shellBuffer[shellIndex] = '\0';
;		putc('\b');
	}
}

void addToShellBuffer(char c) {
	if (shellIndex <= COLS) {
		shellBuffer[shellIndex++] = c;
	}
}

void drawFractal() {	
	drawCFractalEquilateral(150,768,768,10,readData());
}

void drawFractalc() {

	rand+=rand*3^getSeconds()*500;
	drawCFractalEquilateral(150,768,768,9,rand);
	printf("\n\n\n\n");
}

void func() {
    printf("execute!");
}

void printTime() {	
	printf("%d:%d\n", getHours(), getMinutes());
}

void printDate() {
	printf("%d/%d/%d\n", getDay(), getMonth(), getYear());
}

/* pargs[1] -> tz */
void callSetTimeZone(int cargs, void ** pargs) {
	int tz = pargs[1];
	if(tz>=-12 && tz<=12) {
		setTimeZone(tz);
		printf("\nTime set correctly to GTM %d \n",tz);
	} else{
		printf("\nInput error\n");
	}
}

void printInstructions() {
		printf("%s\n", instructions);
}

void callRunGedit() {
	lockMutex(createMutex("shell"));
	runGedit();
	unlockMutex(createMutex("shell"));
}

void callPaintLoop() {
	paintLoop();
}

/*
 *	parg[0]: process description
 *	parg[1]: color
 */
void paintBg(int carg, void ** pargs) {
	drawCSquare(0,0,768,1024, pargs[1]);
	setCursorPos(0);
}

/*
 *	parg[0]: process description
 *	parg[1]: font id
 */
void callChangeFont(int carg, void ** pargs) {
	changeFont(pargs[1]);
}

/*
 *	parg[0]: process description
 *	parg[1]: msg
 */
void callEcho(int carg, void ** pargs) {
	printf(pargs[1]);
	printf("\n");
}

/*
 *	pargs[0]: process description
 *	pargs[1]: pid
 *	pargs[2]: msg
 */
void callKill(int cargs, void ** pargs) {
	kill(pargs[1], pargs[2]);
}

void uslessPs(){
    printf("%d<--fd \n",openPipe("hola"));
    char buffa[6]={0};
    read(3,&buffa,5);
    printf("%s -- mypid: %d",buffa,getPID());
	while(1);
}

void execute() {
	char arr[100];
	int number = 0;


	int tz;
	sbuffer[shellIndex] = '\0';
	int num, pidToKill, msg = 0, psToFg = 1;
	putc('\n');
	if(shellBuffer[0] == '&') {
		psToFg = 0;
		shellBuffer++;
	}
	if (strcmp(shellBuffer, "func") == 0) {
		void** parg = (void**)malloc(sizeof(void*) * 2);
		parg[0] = (void*)"func";
		exec(&func, 1, parg, psToFg);

	} else if (strcmp(shellBuffer, "clear") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"clear";
		exec(&clearScreen, 1, parg, psToFg);

	} else if(strcmp(shellBuffer, "time") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"time";
		exec(&printTime, 1, parg, psToFg);

	} else if(strcmp(shellBuffer, "date") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"date";	
		exec(&printDate, 1, parg, psToFg);

	} else if(sscanf("setTimeZone %d",shellBuffer,&tz)==1) {
		void** parg = (void**)malloc(sizeof(void*) * 2);
		parg[0] = (void*)"setTimeZone";
		parg[1] = (void*)tz;
		exec(&callSetTimeZone, 2, parg, psToFg);

	} else if(strcmp(shellBuffer, "fractal Zelda") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"fractal";
		clear();
		int pid = exec(&drawFractal, 1, parg, psToFg);
		//sleep(1000);
		//clear();

	} else if(strcmp(shellBuffer, "multifractal") == 0) {
		clear();
		for(int i=0;i<50;i++) {
			void** parg = (void**)malloc(sizeof(void*));
			parg[0] = (void*)"colorfractal";
			int pid = exec(&drawFractalc, 1, parg, psToFg);
			sleep(500);
		}

		//clear();
	
	} else if(sscanf("kill %d %d",shellBuffer,&pidToKill, &msg)==2){
		void** parg = (void**)malloc(sizeof(void*) * 3);
		parg[0] = (void*)"kill";
		parg[1] = (void*)pidToKill;
		parg[2] = (void*)msg;
		exec(&callKill, 3, parg, psToFg);

	} else if(strcmp(shellBuffer, "help") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"help";
		exec(&printInstructions, 1, parg, psToFg);

	} else if(strcmp(shellBuffer, "star wars") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"starwars";
		exec(&playStarWars, 1, parg, psToFg);

	} else if(strcmp(shellBuffer, "philo") == 0) {
        void** parg = (void**)malloc(sizeof(void*));
        parg[0] = (void*)"philoManager";
        exec(&philosphers,1,parg, psToFg);

	}else if(strcmp(shellBuffer, "prod") == 0) {
        exec(&producerConsumer,0,0, psToFg);

	} else if(strcmp(shellBuffer, "gedit") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"gedit";
		exec(&callRunGedit, 1, parg, psToFg);
		int n = 400000;
		while (n--);
		lockMutex(createMutex("shell"));
		unlockMutex(createMutex("shell"));

	} else if(strcmp(shellBuffer, "paint") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"paint";
		exec(&callPaintLoop, 1, parg, psToFg);

	} else if(sscanf("paintBg %d", shellBuffer, &num) == 1) {
		void** parg = (void**)malloc(sizeof(void*) * 2);
		parg[0] = (void*)"paintBg";
		parg[1] = (void*)num;
		exec(&paintBg, 2, parg, psToFg);

	} else if(sscanf("setupFont %d", shellBuffer, &num) == 1) {
		void** parg = (void**)malloc(sizeof(void*) * 2);
		parg[0] = (void*)"setupFont";
		parg[1] = (void*)num;
		exec(callChangeFont, 2, parg, psToFg);

	}else if(sscanf("echo %s",shellBuffer,arr)==1) {
		void** parg = (void**)malloc(sizeof(void*) * 2);
		parg[0] = (void*)"echo";
		parg[1] = (void*)arr;
		exec(&callEcho, 2, parg, psToFg);

	} else if(strcmp(shellBuffer, "ps") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"ps";
		exec(&ps, 1, parg, psToFg);

	} else if(strcmp(shellBuffer, "newps") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"uslessPs";
		exec(&uslessPs, 1,parg, psToFg);

	} else if(strcmp(shellBuffer, "pepito") == 0) {
		void** parg = (void**)malloc(sizeof(void*));
		parg[0] = (void*)"PepitoJoseCreator";
		exec(&pepitoJose, 1,parg, psToFg);

	} else if(strcmp(shellBuffer, "mt") == 0) {
		printf("%d<--fd \n",openPipe("hola"));
		printf("%d<--fd \n",openPipe("hola2"));
		printf("%d<--fd \n",openPipe("hola3"));
		write(3,"AAAAA",5);
        void** parg = (void**)malloc(sizeof(void*));
        parg[0] = (void*)"uslessPs";
        exec(&uslessPs, 1,parg, psToFg);

	}else if(sscanf("pacman clear %d", shellBuffer, &num) == 1) {
        if(num<1 || num>20)
            printf("Number muste be between 1 and 20\n");
        else
            pacmanClear(num);
	} else {
		printf("Command not found.\n");
	}

	if (psToFg == 0) {
		shellBuffer--;
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
