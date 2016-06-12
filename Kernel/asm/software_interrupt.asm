GLOBAL softwareInterrupt
GLOBAL getChar

softwareInterrupt:
    INT 80h
    ret


getChar:
    push rdi
    mov rax,0
    mov rdi,1
    mov rsi,char
    mov rdx,1
    INT 80h
    mov rbx,[rax]
    mov rax,rbx
    pop rdi
    ret

section .bss
char: resb 1