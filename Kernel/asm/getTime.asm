global _getSeconds
global _getAlarmSeconds
global _getMinutes
global _getAlarmMinutes
global _getHoursASM
global _getAlarmHoursASM
global _getDayofWeek
global _getDayofMonth
global _getMonth
global _getYear


global _setAlarmSeconds
global _setAlarmMinutes
global _setAlarmHoursASM




section .text

%macro _setState 1

            mov al,0Bh
            out 70h,al
            in al,71h
            or al,4
            out 71h,al
            mov al, %1
            out 70h,al
%endmacro

_getSeconds:
            push rbp
            mov rbp,rsp
            _setState 0
            in al,71h
            leave
            ret

_getAlarmSeconds:
            push rbp
            mov rbp,rsp
            _setState 1
            in al,71h
            leave
            ret

_setAlarmSeconds:
            push rbp
            mov rbp,rsp
            _setState 1
            mov rax,rdi
            out 71h,al
            leave
            ret

_getMinutes:
            push rbp
            mov rbp,rsp
            _setState 2
            in al,71h
            leave
            ret

_getAlarmMinutes:
            push rbp
            mov rbp,rsp
            _setState 3
            in al,71h
            leave
            ret

_setAlarmMinutes:
            push rbp
            mov rbp,rsp
             _setState 3
            mov rax,rdi
            out 71h,al
            leave
            ret


_getHoursASM:
            push rbp
            mov rbp,rsp
            _setState 4
            in al,71h
            leave
            ret

_getAlarmHoursASM:
            push rbp
            mov rbp,rsp
            _setState 5
            in al,71h
            leave
            ret

_setAlarmHoursASM:
            push rbp
            mov rbp,rsp
            _setState 5
            mov rax,rdi
            out 71h,al
            leave
            ret

_getDayofWeek:
            push rbp
            mov rbp,rsp
            _setState 6
            in al,71h
            leave
            ret

_getDayofMonth:
            push rbp
            mov rbp,rsp
            _setState 7
            in al,71h
            leave
            ret

_getMonth:
            push rbp
            mov rbp,rsp
            _setState 8
            in al,71h
            leave
            ret

_getYear:
            push rbp
            mov rbp,rsp
            _setState 9
            in al,71h
            leave
            ret

