GLOBAL testAndSet

testAndSet:
    push rbp
    mov rbp, rsp

    xor edx, edx
    xor eax, eax
    xor ecx, ecx
    xor ebx, ebx

    inc ebx

    lock CMPXCHG8B [rdi]

    jnz lockNotAcquired

    mov rax, 1

    mov rsp, rbp
    pop rbp
    ret
  lockNotAcquired:
    mov rax, 0

    mov rsp, rbp
    pop rbp
    ret