;
; interrups.asm
;
; Created on: Apr 18, 2010
;      Author: anizzomc
;

GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL _lidt
GLOBAL haltcpu

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _irq80Handler


EXTERN irqDispatcher
EXTERN syscallHandler

%macro irqHandlerMaster 1
	push rdi

	mov rdi, %1
	call irqDispatcher
	
	
	;signal pic
	mov al, 20h
	out 20h, al

	pop rdi
	iretq
%endmacro



SECTION .text

_cli:
	cli
	ret


_sti:
	sti
	ret


picMasterMask:
    push rbp
    mov rbp, rsp

    mov rax, rdi
    out	21h,al

    mov rsp, rbp
    pop rbp
    ret

picSlaveMask:
    push rbp
    mov rbp, rsp

    mov rax, rdi
    out	0A1h, al

    mov rsp, rbp
    pop rbp
    ret

_lidt:				; Carga el IDTR
    push    rbp
    mov     rbp, rsp

    push    rbx
    mov     rbx, [rbp + 6] ; ds:bx = puntero a IDTR
    rol	    rbx,16
    lidt    [rbx]          ; carga IDTR
    pop     rbx

    mov     rsp, rbp
    pop     rbp
    ret


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0
	
;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3
	
;Serial Port 1 and 3	
_irq04Handler:
	irqHandlerMaster 4
	
;USB
_irq05Handler:
	irqHandlerMaster 5

_irq80Handler:

    push r9
    mov r9,r8
    mov r8, rcx
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, rax
	call syscallHandler
	pop r9

	;signal pic
	mov al, 20h
	out 20h, al

	iretq

haltcpu:
	cli
	hlt
	ret

