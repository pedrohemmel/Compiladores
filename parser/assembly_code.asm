section .data
	resultado db 'Resultado: ', 0
section .text
	global _start
_start:
	finit
	fld 5.300000
	fld 5.400000
	fsub
	fstp 5.400000
	mov eax, 4
	mov ebx, 1
	mov ecx, resultado
	mov edx, 13
	int 0x80
	mov eax, 1
	xor ebx, ebx
	int 0x80
