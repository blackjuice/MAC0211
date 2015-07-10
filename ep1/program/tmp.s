main:
	jmp	.L2
.L3:
	mov	eax, OFFSET FLAT:.LC0
	mov	DWORD PTR [esp], eax
	call	printf
	add	DWORD PTR [esp+28], 1
.L2:
	cmp	DWORD PTR [esp+28], 9
	jle	.L3

;return
	mov	eax, 0
	leave
	ret
