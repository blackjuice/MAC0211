; Torre de Hanoi
global	main
	
; Segmento de dados
section .data

; msg e' a cadeia de caracteres impressa para indicar cada movimento 
; Por exemplo, para indicar um movimento de 1 para 3 a cadeia será "1 -> 3\n"
msg:	db	"d -> d",10,0

; algumas constantes, que indicam a posição dos parametros e
; variaveis locais na pilha
n:	equ	 8 
origem:	equ	 12 
dest:	equ	 16 
temp:	equ	 20 

; Segmento de código
section .text

main:
	; Faz a chamada a funcao Hanoi
	push	dword 3     ; pino "temporario"	
	push	dword 2     ; pino "destino"
	push	dword 1     ; pino "origem"
	push	dword 3     ; numero de discos

	call    hanoi
	add	esp,16	    ; "retira" parametros da pilha 

        movl    eax,1         ; numero da chamada ao sistema (sys_exit) 
        xorl    ebx,ebx       ; codigo de saida: 0
        int     0x80          ; executa a chamada

;-----------------------------------------------------------------------
; parte recursiva:
; prototipo: hanoi (int n, int origem, int dest, int temp)
; move n discos de origem para dest, usando temp como pino temporario

hanoi:	
	push	ebp		; cria o stack frame para a funcao
	mov     ebp, esp	

	mov     eax,[ebp+n]	; obtem parametro n
        cmp     eax,0	        ; chegamos na base da recursao?
	je	pronto		; se n e' zero, sai da funcao
          
	; chama recursivamente (n-1 discos de origem para temp)
	; hanoi(n-1,origem,temp,dest)
	dec     eax		; eax = n-1
        push	eax             ; salva eax: vamos usar novamente mais tarde
	push	[ebp+dest]	; quarto param: pino temporario
	push    [ebp+temp]	; terceiro param: pino destino
	push    [ebp+origem]	; segundo param: pino de origem
        push    eax		; primeiro param: num de fichas
	call    hanoi
        add	esp,16

	; move 1 disco de origem para dest
	; imprime uma mensagem do tipo "origem -> destino",
	; para indicar a movimentacao
	push    [ebp+dest]
	push    [ebp+origem]
	push	msg
        call	printf
	add	12,esp

	; n-1 discos de temp para dest
        ; hanoi(n-1,temp,dest,origem)
	pop	eax		; n-1 em eax
	push    [ebp+origem]
	push    [ebp+dest]
	push    [ebp+temp]
	push    eax
	call    hanoi
	add	esp,16

pronto:
	; desfaz o frame e retorna
	mov	 esp, ebp
	pop      ebp
	ret

