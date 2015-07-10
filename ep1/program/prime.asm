int2str:
	push	ebp
	mov	ebp, esp
	sub	esp, 40
	mov	DWORD PTR [ebp-20], 2
	jmp	.L2
.L6:
	mov	eax, DWORD PTR [ebp-20]
	mov	edx, eax
	sar	edx, 31
	idiv	DWORD PTR [ebp-16]
	mov	eax, edx
	test	eax, eax
	jne	.L4
	mov	DWORD PTR [ebp-12], 0
	jmp	.L5
.L4:
	mov	DWORD PTR [ebp-12], 1
	add	DWORD PTR [ebp-16], 1
.L3:
	mov	eax, DWORD PTR [ebp-16]
	cmp	eax, DWORD PTR [ebp-20]
	jl	.L6
.L5:
	cmp	DWORD PTR [ebp-12], 0
	je	.L7
	mov	eax, OFFSET FLAT:.LC0
	mov	edx, DWORD PTR [ebp-20]
	mov	DWORD PTR [esp+4], edx
	mov	DWORD PTR [esp], eax
	call	printf
.L7:
	add	DWORD PTR [ebp-20], 1
.L2:
	mov	eax, DWORD PTR [ebp-20]
	cmp	eax, DWORD PTR [ebp+8]
	jl	.L8
.L8:
	mov	DWORD PTR [ebp+16], 2
	jmp 	.L3
	
	ret
	endp

global_start 
_start:
	push	ebp
	mov	ebp, esp
	and	esp, -16
	sub	esp, 32
	mov	DWORD PTR [esp+28], 1000	; putting 100 inside DWORD PTR [esp]
	mov	eax, DWORD PTR [esp+28]
	mov	DWORD PTR [esp], eax
	call	int2str
	mov	eax, 0				; return value
	leave
