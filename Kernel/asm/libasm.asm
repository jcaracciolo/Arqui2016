GLOBAL cpuVendor

section .text

GLOBAL _get_rsp
GLOBAL _set_rsp
GLOBAL _get_rip
GLOBAL _popAll
GLOBAL _pushAll
GLOBAL _change_process
GLOBAL _yield

EXTERN next_process

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
	push fs
	push gs
%endmacro

%macro	popState 0
	pop gs
	pop fs
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

%macro deleteInterr 0
	pop rax
	pop rax
	pop rax
	pop rax
	pop rax
    pop rax
   	pop rax
   	pop rax
   	pop rax
%endmacro



cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

_yield:
	mov rax,rsp

	push QWORD 0
	push QWORD 0
	push rax
	pushfq
	push QWORD 0x008
	push .ret

	pushState

	jmp to_next

.ret:
	ret

_change_process:
    deleteInterr

to_next:
	mov rdi, rsp
	call next_process

	mov rsp, rax

    ;signal pic
    mov al, 20h
    out 20h, al

    popState
    sti
    iretq

_get_rsp:
	mov rax, rsp
	ret

_set_rsp:
	mov rsp, rdi
	ret

_get_rip:
	mov rax, $
	ret

_pushAll:
	push rax
	ret

_popAll:
	popState
	ret