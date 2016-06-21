#ifndef STDTIME_H
#define STDTIME_H
typedef void (*timerEventT)() ;


char getHours();
char getMinutes();
char getSeconds();
char getYear();
char getMonth();
char getDay();
void setTimeZone(int tz);
void sleep(unsigned int time);
void removeTimer(timerEventT t);
void addTimer(timerEventT t, int interval);
void sleep(unsigned int time);

#endif
