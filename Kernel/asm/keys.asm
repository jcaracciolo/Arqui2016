
GLOBAL read_key

read_key:
	in al, 60h
	movzx rax, al
	ret
