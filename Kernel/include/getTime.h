
#ifndef KERNELARQUI_GET_TIME_H
#define KERNELARQUI_GET_TIME_H

char _getSeconds();
char _getMinutes();
char _getHours();
char _getDayofWeek();
char _getDayofMonth();
char _getMonth();
char _getYear();

char _getAlarmSeconds();
void _setAlarmSeconds(char seconds);
char _getAlarmMinutes();
void _setAlarmMinutes(char minutes);
char _getAlarmHours();
void _setAlarmHours(char hours);

void _setTimeZone(int timezone);




#endif //KERNELARQUI_GET_TIME_H
