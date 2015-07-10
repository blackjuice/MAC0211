	.file	"prime.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	and	esp, -16
	sub	esp, 32
	mov	DWORD PTR [esp+20], 2
	jmp	.L2
.L8:
	mov	DWORD PTR [esp+24], 2
	jmp	.L3
.L6:
	mov	eax, DWORD PTR [esp+20]
	mov	edx, eax
	sar	edx, 31
	idiv	DWORD PTR [esp+24]
	mov	eax, edx
	test	eax, eax
	jne	.L4
	mov	DWORD PTR [esp+28], 0
	jmp	.L5
.L4:
	mov	DWORD PTR [esp+28], 1
	add	DWORD PTR [esp+24], 1
.L3:
	mov	eax, DWORD PTR [esp+24]
	cmp	eax, DWORD PTR [esp+20]
	jl	.L6
.L5:
	cmp	DWORD PTR [esp+28], 0
	je	.L7
	mov	eax, OFFSET FLAT:.LC0
	mov	edx, DWORD PTR [esp+20]
	mov	DWORD PTR [esp+4], edx
	mov	DWORD PTR [esp], eax
	call	printf
.L7:
	add	DWORD PTR [esp+20], 1
.L2:
	cmp	DWORD PTR [esp+20], 999
	jle	.L8
	mov	DWORD PTR [esp], 10
	call	putchar
	mov	eax, 0
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
