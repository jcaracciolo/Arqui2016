GLOBAL decreaseTimerTick

decreaseTimerTick:
push rax
cli
mov al, 0x36
out 0x43, al    ;tell the PIT which channel we're setting

mov rax, 19886    ;  x = 1193182 / f
out 0x40, al    ;send low byte
mov al,ah
out 0x40, al    ;send high byte
sti
pop rax
ret