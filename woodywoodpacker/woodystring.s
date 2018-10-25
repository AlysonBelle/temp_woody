



section .text
global _start


_start:

	jmp 	_springboard

_func2:
	mov	rax, 1
	mov	rdi, 1
	pop	rsi
	mov	rdx, 12
	syscall

	jmp	leaving	

_springboard:
	push	rdi
	push	rsi
	push	rdx	
	call 	_func2
	db	'...WOODY...', 10
	
leaving:
	pop 	rdx
	pop	rsi
	pop	rdi
