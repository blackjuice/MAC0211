	.file	"plusCheck.c"
	.intel_syntax noprefix
	.section	.rodata
.LC0:
	.string	"higher"
.LC1:
	.string	"here we are"
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
	mov	DWORD PTR [esp+24], 3
	mov	eax, DWORD PTR [esp+24]
	mov	edx, DWORD PTR [esp+20]
	add	eax, edx
	mov	DWORD PTR [esp+28], eax
	cmp	DWORD PTR [esp+28], 5
	jle	.L2
	mov	eax, OFFSET FLAT:.LC0
	mov	DWORD PTR [esp], eax
	call	printf
	jmp	.L3
.L2:
	mov	eax, OFFSET FLAT:.LC1
	mov	DWORD PTR [esp], eax
	call	printf
.L3:
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
