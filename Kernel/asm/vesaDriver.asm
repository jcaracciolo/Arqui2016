
GLOBAL getVesaBiosInfo
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

;returns 0x004f if successfull
;
getVesaBiosInfo:
    pushState
    mov ah, 0x0c
    mov bh, 0x00
    mov al, 4
    mov cx, 4
    mov dx, 5
    int 0x10
    popState

