global _start		; exporta para o ligador (ld) o ponto de entrada
section .text
_start:
	; sys_write(stdout, mensagem, tamanho)
	mov eax, 4		; chamada de sistema sys_write
	mov ebx, 1		; stdout 
	mov ecx, mensagem	; endereco da mensagem
	mov edx, tamanho	; tamanho da string da mensagem
	int 80h			; chamada ao nucleo (kernel)

	; sys_exit(return_code)

	mov eax, 1		; chamada de sistema sys_exit
	mov ebx, 0		; return 0 (sucesso)
	int 80h			; chamada ao nucleo (kernel)

section .data
mensagem: db 'Hello, World!',0x0A	; mensagem e quebra de linha
tamanho: equ $ - mensagem		; tamanho da mensagem
