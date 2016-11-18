#include "types.h"
#include "include/strings.h"
#include <stdarg.h>
#include "include/shell.h"
#include "include/stdlib.h"
extern void int80(qword rdi, qword rsi, qword rdx, qword rcx, qword r8, qword r9);


char* readLine() ;
char* readInt(char* string, int* num);


void putc(char c) {
	int80(4,1,&c,1,0,0);
}


int getc() {
	char a[2];
	int80(3,0,a,2,0,0);
	if(*a==0) return EOF;
	return *a;
}

void read(int fd,char* buffer,int size){
	int80(3,fd,buffer,size,0,0);
}

void printError(char *msg){
	write(2,msg,strlen(msg));
}

void write(int fd,char* msg,int size){
	int80(4,fd,msg,size,0,0);
}
// cambiar por printf
void printCharArray(char* arr, int length) {
	int80(4,1,arr,length,0,0);
}

void changeFont(int n) {
	int80(16,n,0,0,0,0);
}

int printf(const char* format,...){
	va_list args;
	va_start( args, format );

	int n;
	char strnum[10];
	char* str;

	while(*format!='\0'){
		if(*format!='%'){
			putc(*format);
		}else{
			switch(*(++format)){
				case '%': putc('%'); break;
				case 'd':
				case 'i':
					n=va_arg(args,int);
					int length=intToString(strnum,n);
					printCharArray(strnum,length);
					break;
				case 'c':
					n=(char)va_arg(args,int);
					putc((char)n);
					break;
				case 's':
					str=(char*)va_arg(args,char*);
					printCharArray(str, strlen(str));
					break;

			}
		}
		format++;

	}

	return 0;

}

int scanf(const char* format,...){
	va_list args;
	va_start( args, format );

	int n=0;

	char* str = readLine();
    char* character;


	while(*format!='\0'){
		if(*format!='%'){
            if((*format) != (*str)){
                return n;
            } else{
                format++;
                str++;
            }
		}else{
			switch(*(++format)){
				case '%':
                    if(*str != '%') return n;
                    else str++;
                    break;
				case 'd':
				case 'i':
                    str = readInt(str, va_arg(args,int));
					n++;
					break;
				case 'c':
                    character = va_arg(args, char*);
                    *character = *str++;
                    n++;
                    break;
				case 's':
                    character = va_arg(args,char*);
                    char temp;
                    while( (temp = *str) != '\0'){
                        *character = *str;
                        character++;
                        str++;
                    }
                    n++;
			}
			++format;
		}
	}

	return n;
}

int sscanf(const char* format, const char* str, ...){

		va_list args;
		va_start( args, format );


		int n=0;
		char strnum[10];
		char* character;

		while(*format!='\0'){
			if(*format!='%'){
				if((*format) != (*str)){
					return n;
				} else{
					format++;
					str++;
				}
			}else{
				switch(*(++format)){
					case '%':
						if(*str != '%') return n;
						else str++;
						break;
					case 'd':
					case 'i':
						character=str;
						str = readInt(str, va_arg(args,int));
						if(str!=character){
							n++;
						}
						break;
					case 'c':
						character = va_arg(args, char*);
						*character = *str++;
						n++;
						break;
					case 's':
						character = va_arg(args,char*);
						char temp;
						while( (temp = *str) != '\0'){
							*character = *str;
							character++;
							str++;
						}
						n++;

				}
				++format;
			}
		}

		return n;
	}


/**
 * Returns a null terminated string. It reads until it finds a \n.
 */
char* readLine() {
    int bufferIndex = 0;
    char *buff = malloc(COLS + 1);

    int c ;

    while ((c = getc()) != '\n') {
        if(c == '\b'){
            if (bufferIndex != 0) {
                bufferIndex--;
                putc('\b');
            }
        }
        else if(c != -1){
            if (bufferIndex <= COLS) {
                buff[bufferIndex++] = c;
            }
            putc(c);
        }

    }
    buff[bufferIndex] = '\0';
    return buff;
}
/**
 * Tries to parse a num from string. It stores the result in num and returns the
 * string that is left to read;
 */
char* readInt(char* string, int* num){
    *num = 0;
	boolean sign=1;

    if(*string == '-'){
		if (isNum(*(string + 1))) {
			sign = -1;
			*num = (*(string + 1) - '0') * sign;
			string++;
			string++;
		} else {
			return string;

		}
	}

	int c;

    while (isNum(c = *string)){
        *num = (*num)*10+(c-'0')*sign;
        string++;
    }
    return string;
}


