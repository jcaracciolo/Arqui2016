#include "getTime.h"

static int tz = 0;

char _getAlarmHoursASM();

char _setAlarmHoursASM();

char _getHoursASM();

#define abs(n) (n)>=0?(n):24+(n)

void _setTimeZone(int timezone) {
    if (timezone >= -12 && timezone <= 12)
        tz = timezone;
}

char _getAlarmHours() {
    char h = (_getAlarmHoursASM() + tz) % 24;
    return abs(h);
}

char _getHours() {
    char h = (_getHoursASM() + tz) % 24;
    return abs(h);
}

void _setAlarmHours(char hours) {
    if (hours >= 0 && hours < 24) {
        char h = (hours - tz) % 24;
        _setAlarmHoursASM(abs(h));
    }
}

void _get
