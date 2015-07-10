;*******************************************
; nome: LUCAS SUNG JUN HONG
; nUSP: 812 432 9
; 
; EP1 - Lab Prog (MAC 0211)
; 
; Compilacao:
;   Para rodar em uma maquina 64bit:
;      # nasm -f elf32 ep1_8124329.asm
;      # ld -m elf_i386 -s -o ep1_8124329 ep1_8124329.o
;
;   Para rodar em uma maquina 632bit:
;      # nasm -f elf ep1_8124329.asm
;      # ld ep1_8124329.o -o ep1_8124329
;
; Execucao:
;      # ./ep1_8124329
;
;*******************************************

; declaracao das variaveis nao-inicializadas
;   aqui temos os argumentos de 12 bytes e argumentos de 32 bits
section .bss
        buffer     resb    12      	; reservamos 12 bytes
        I          resb    4        ; reservamos 32 Bits
        J          resb    4        ; reservamos 32 Bits

; declaracao das variaveis inicializadas
section .data
        LF:             equ     0Ah         ; para pular linha na saida (equivalente a "\n")
        println:        db      LF, 0       ; chamar 'println' equivalente a printf("\n")
        printsize:      equ     $ - println ; tamanho para o comando acima
        tens:           dd      0           ; declaramos uma variavel tens = 0
        hundreds:       dd      0           ; variavel hundreds = 0
        boolean_prime:  dd      0           ; variavel booleana prime = 0

;-------------------------------------------------------------------------------------------------
;; Inicio do codigo
section .text
    ; subrotina int2str
    int2str:

        mov eax, ebp        ; copiamos o parametro recebido para eax
        mov [buffer], eax   ; copia do valor de eax para [buffer]

        ; faremos uma limpeza geral nos registradores eax, ebx e ecx para garantir
        ; que os registradores nao possuam resto de dado em suas memorias
        xor eax,eax
        xor ebx,ebx
        xor ecx,ecx

    ;; Printing buffers
    ; If (buffer > 100)
    ;   decrement by 100 until buffer < 100 and increment hundreds, then print hundreds
    ; If (buffer > 10)
    ;   decrement by 10 until buffer < 10 and increment tens, then pŕint tens
    ; Then buffer

    ; instrucao while hundreds realiza o seguinte:
    ;   Verifica se o valor contido em 'buffer' > 100:
    ;       - Caso sim, subtraimos 'buffer' de 100 N vezes ate que 'buffer' < 100,
    ;           tal que N nesse caso sera registrada em [hundreds], um contador.
    ;       - Caso contrario, pulamos para instrucao whiletens
    ;        
        mov ecx, 0             ; fazemos ecx = 0
        mov [hundreds], ecx    ; copia do valor de ecx para [hundreds], ou seja, [hundreds] = 0
    whilehundreds:
    	mov eax, [buffer]      ; copia de valor de buffer para eax
    	cmp eax, 100           ; comparamos eax com 100, ou seja, comparamos buffer com 100
    	jl donehundreds        ; caso buffer < 100, pulamos para instrucao donehundreds
    	
        ; caso contrario, subtraimos 100 de buffer 
    	sub eax, 100           ; subtrai 100 de eax, que possui o valor de buffer
    	mov [buffer], eax      ; copia o valor eax para buffer, ou seja, atualizamos o valor de buffer
    	inc ecx                ; incrementamos valor de ecx
    	mov [hundreds], ecx    ; copia do valor ecx incrementado para [hundreds],
                               ; [hundreds] sera contador que conta numero de vezes que subtraimos 100 de buffer
    	jmp whilehundreds      ; voltamos para whilehundreds
    	;mov ecx, 0

    donehundreds:			; printing hundreds
    	mov eax, [hundreds]
    	cmp eax, 0	
    	je second

    	add eax, '0'
    	mov [hundreds], eax
    	mov eax, 4
    	mov ebx, 1
    	mov ecx, hundreds
    	mov edx, 4
    	int 0x80
    	jmp second

    ;; e.g.: buffer = 101
    ;	 if (buffer > 101)
    ;		buffer = buffer - 100  (hundreds = 1)
    ;	 we will have buffer = 01
    ;	 if (buffer < 10 && hundreds > 0)
    ;		print extra '0'
    print0:	
    	mov eax, 0
    	add eax, '0'
    	mov [hundreds], eax
    	
    	mov eax, 4
    	mov ebx, 1
    	mov ecx, hundreds
    	mov edx, 4
    	int 0x80
    	jmp donefinal

    second:
    	mov eax, [buffer]
    	cmp eax, 10
    	jl checking
    	jmp secondreal

    checking:
    	mov eax, [hundreds]
    	cmp eax, 0
    	jg print0
    	jmp secondreal

    secondreal:
    	mov ecx, 0
    	mov [tens], ecx

    whiletens:
    	mov eax, [buffer]
    	cmp eax, 10
    	jl donetens
    	
    	sub eax, 10
    	mov [buffer], eax
    	inc ecx	
    	mov [tens], ecx
    	jmp whiletens

    	mov ecx, 0

    donetens:
    	mov eax, [tens]
    	cmp eax, 0
    	je donefinal
    	
    	add eax, '0'
    	mov [tens], eax
    	mov eax, 4
    	mov ebx, 1
    	mov ecx, tens
    	mov edx, 4
    	int 0x80

    ; IMPRIME buffer NA TELA
    donefinal:
    	mov eax, [buffer]          ; convertemos [buffer] para que
    	add eax, '0'               ; a impressao do numero apareca
    	mov [buffer], eax          ; em ASCII

    	mov eax, 4                 ; teremos um StdOut
    	mov ebx, 1
    	mov ecx, buffer            ; impressao do buffer na tela
    	mov edx, 12                ; tamanho do buffer
    	int 0x80                   ; chamamos kernel

    	mov eax, 4                 ; teremos um StdOut
    	mov ebx, 1
    	mov ecx, println           ; impressao de um pula linha "\n"
    	mov edx, printsize         ; tamanho do pula linha
    	int 0x80                   ; chamamos kernel
    	
    out:
    	ret                        ; retorna para main -> fim da subrotina

;; main
global _start
_start:
	; sys_write

    ;**************************************************
    ;   inicio do loop_i (i comeca de 2 e vai ate 999)
    ;**************************************************
    	mov ecx, 2          ; inicia contador = I = 2
        mov [I], ecx        ; copia valor de [I] para ecx
    loop_I_Start:           ; inicio do loop_i
        mov eax, [I]        ; copia o valor da memoria [I] para eax
        cmp eax, 1000         ; compara contador com 1000
        jge loop_I_End      ; pula para loop_I_End se contador >= 1000

        ; caso contrario, definimos boolean_prime = 1 e iniciamos o loop_j
        mov ecx, 1                  ; fazemos eax = 1
        mov [boolean_prime], ecx    ; copia o valor da memoria eax para boolean_prime
        mov ecx, 2                  ; inicia contador = 2
        mov [J], ecx                ; copia o valor de ecx para [J]

        ;**************************************************
        ;   inicio do loop_J (J comeca de 2 e vai ate [I])
        ;**************************************************
        loop_J_Start:
            mov eax, [J]        ; copia o valor da memoria [J] para eax
            mov ecx, [I]        ; copia o valor da memoria [I] para ecx
            cmp eax, ecx        ; comparamos [J] com [I]
            jge loop_J_End      ; se [J] > [I], pula para loop_J_End




            mov ecx, [J]        ; copia o valor da memoria [J] para eax
            mov eax, [I]        ; copia o valor da memoria [I] para ecx

            xor edx, edx        ; limpamos edx para garantir que nao exista resto de memoria em edx
            div ecx             ; fizemos aqui a operacao: [I] / [J]
            mov eax, edx        ; copiamos o resto da divisao para eax
            cmp eax, 0
            je not_a_prime      ; Caso sim, nao teremos um primo. Entao pula para not_a_prime
            jmp continue

            not_a_prime:        ;faremos [boolean_prime] = 0


                mov ecx, 0                  ; fazemos eax = 0
                mov [boolean_prime], ecx    ; copia o valor da memoria eax para boolean_prime
                jmp loop_J_End



            ; caso contrario, checaremos se [I] % [J] == 0
            ;   Se [I] % [J] != 0, incrementamos [J] ate encontramos um [I] % [J] == 0
            ;       se a condicao acima nao for satisfeita, teremos um primo
            ;       caso contrario, NAO temos um primo: entao saimos do loop_J,
            ;       e voltamos ao loop_I

            continue:
            mov eax, [J]        ; copia o valor da memoria [J] para eax
            add eax, 1          ; incrementamos eax
            mov [J], eax        ; copiamos o valor incrementado de eax para [J]
            jmp loop_J_Start    ; volta para loop_J_Start

        ;**************************************************
        ;   fim do loop_J (volta para o loop_I)
        ;**************************************************
        loop_J_End:
            mov eax, [boolean_prime]    ; copia o valor da memoria [boolean_prime] para eax
            cmp eax, 1                  ; verificamos se boolean_prime == 1
            je then_print               ; Caso sim, pula para then_print, onde faremos a impressao de [I]
            jmp dont_print              ; Caso contrario, nao imprimimos [I] e incrementamos [I]++
            
            then_print:             ; impressao do [I], que classifica-se como numero primo
                mov eax, [I]        ; copia o valor da memoria [I] para eax            
                mov ebp, eax        ; valor de [I] entrando como parametro para subrotina
                call int2str        ; chama subrotina int2str

                mov eax, [I]        ; copia o valor da memoria [I] para eax
                add eax, 1          ; incrementamos eax
                mov [I], eax        ; copiamos o valor incrementado de eax para [I]
                jmp loop_I_Start    ; volta para o loop_I_Start

            dont_print:             ; nao imprimimos [I], apenas incrementamos [I]++
                mov eax, [I]        ; copia o valor da memoria [I] para eax
                add eax, 1          ; incrementamos eax
                mov [I], eax        ; copiamos o valor incrementado de eax para [I]
                jmp loop_I_Start    ; volta para o loop_I_Start

    ;**************************************************
    ;   fim do loop_I (saida do programa)
    ;**************************************************
    loop_I_End:             ; fim do loop
        ; sys_exit(return_code)
        mov eax, 1
        mov ebx, 0
        int 0x80
