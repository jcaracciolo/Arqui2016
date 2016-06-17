GLOBAL softwareInterrupt
GLOBAL getChar
GLOBAL int80


%macro pushState 0  ; fuente: RowDaBoat/Proyect Wyrm
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

;
vesaControllerInfo:



softwareInterrupt:
    INT 80h
    ret


getChar:
    push rdi
    mov rax,3
    mov rdi,1
    mov rsi,char
    mov rdx,1
    INT 80h
    mov rax,[char]
    pop rdi
    ret


int80:

    pushState
    INT 80h
    popState
    ret

section .bss
char: resd 1