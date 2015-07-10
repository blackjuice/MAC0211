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
;
section .bss
        buffer     resb    12      	; reservamos 12 bytes
        I          resb    4        ; reservamos 32 Bits
        J          resb    4        ; reservamos 32 Bits

; declaracao das variaveis inicializadas
;
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
        ;
        xor eax,eax
        xor ebx,ebx
        xor ecx,ecx

        ; instrucao whilehundreds realiza o seguinte:
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
    	
        ; caso contrario, subtraimos 100 de buffer, e fazemos a contagem dessas subtracoes
    	sub eax, 100           ; subtrai 100 de eax, que possui o valor de buffer
    	mov [buffer], eax      ; copia o valor eax para buffer, ou seja, atualizamos o valor de buffer
    	inc ecx                ; incrementamos valor de ecx
    	mov [hundreds], ecx    ; copia do valor ecx incrementado para [hundreds],
                               ; [hundreds] sera contador que conta numero de vezes que subtraimos 100 de buffer
    	jmp whilehundreds      ; voltamos para whilehundreds

        ; instrucao donehundreds imprime [hundreds], que equivale ao digito mais significativo de [buffer]
        ;   Primeiro, verificamos se houve alguma subtracao de buffer com 100,
        ;   ou seja, verificamos se hundreds > 0
        ;       Caso sim, imprimimos hundreds na tela
        ;       Caso contrario, pulamos para instrucao end_of_hundreds
        ;
        donehundreds:
        	mov eax, [hundreds]     ; copia de valor de hundreds para eax
        	cmp eax, 0	            ; comparacao de eax com 0
        	je end_of_hundreds      ; Caso sim, pulamos para instrucao end_of_hundreds

            ; caso contrario, imprimimos hundreds
        	add eax, '0'               ; convertemos o valor contido em eax para a tabela ASCII
        	mov [hundreds], eax        ; copia de eax para hundreds
        	mov eax, 4                 ; teremos um StdOut
        	mov ebx, 1
        	mov ecx, hundreds          ; impressao do hundreds na tela
        	mov edx, 4                 ; tamanho de hundreds, que sao 32 bits
        	int 0x80           ; chamada ao kernel

        ; instrucao end_of_hundreds verifica se buffer < 10
        ; essa verificacao é necessaria pois podemos ter os seguintes casos:
        ;
        ; caso 1:
        ;   buffer = 123
        ;   Como buffer > 100, instrucao whilehundreds faz 123 - 100 = 23
        ;   note que subtraimos apenas uma vez, entao hundreds = 1
        ;
        ;   Logo apos, a instrucao donehundreds imprime hundreds na tela ("1")
        ;   temos o buffer = 23. Pulamos para a instrucao whiletens para verificar se 23 > 10
        ;   Como sim, subtraimos 10 de buffer 2 vezes, entao tens = 2
        ;   A instrucao donetens imprime tens na tela ("2")
        ;   Assim, a instrucao donefinal imprime os digitos restantes de buffer ("3")
        ;   saida: 123
        ;
        ; caso 2:
        ;   Nao tivemos em imprimir 123 na tela no caso 1, mas o caso 2 poderiamos ter:
        ;
        ;   buffer = 101
        ;   Como buffer = 101 > 100, a instrucao whilehundreds faz 101 - 100 = 1
        ;   note que subtraimos apenas uma vez, entao hundreds = 1
        ;    
        ;   Logo apos, a instrucao donehundreds imprime hundreds na tela ("1")
        ;   Entramos depois na na instrucao whiletens, que verifica se buffer < 10
        ;   Como buffer = 1 < 10, pulamos para instrucao donefinal que imprime o ultimo digito
        ;   saida: 11
        ;
        ;   Note que temos um problema em caso 2. Precisamos assim uma instrucao end_of_hundreds.
        ;
        ; caso 3 (correcao para caso 2):
        ;   buffer = 101
        ;   Como buffer = 101 > 100, a instrucao whilehundreds faz 101 - 100 = 1
        ;   note que subtraimos apenas uma vez, entao hundreds = 1
        ;
        ;   Logo apos, a instrucao donehundreds imprime hundreds na tela ("1").
        ;   verificamos se buffer < 10. Caso sim, verfique antes o seguinte: se hundreds > 0.
        ;   Essa verificacao é feita por checking.
        ;       Caso sim, precisamos imprimir um "0" extra na tela. A instrucao print0 faz isso.
        ;       Caso contrario, pulamos para instrucao whiletens.
        ;
        ;   A instrucao whiletens, que verifica se buffer < 10
        ;   Como buffer = 1 < 10, pulamos para instrucao donefinal que imprime o ultimo digito
        ;   saida: 101
        ;
        end_of_hundreds:
            mov eax, [buffer]           ; copia valor de buffer em eax
            cmp eax, 10                 ; compara eax com 10
            jl checking                 ; caso eax < 10, pulamos para instrucao checking
            jmp start_of_whiletens      ; caso contrario, pulamos para instrucao start_of_whiletens

        checking:                       ; instrucao que checa se hundreds > 0
            mov eax, [hundreds]         ; copia valor de hundreds em eax
            cmp eax, 0                  ; compara eax com 0
            jg print0                   ; Caso eax > 0, pula para instrucao print0
            jmp start_of_whiletens      ; Caso contrario, pula para start_of_whiletens

        print0:	                        ; instrucao que imprime um "0" extra na tela
        	mov eax, 0                  ; eax tera valor 0 (eax = 0)
        	add eax, '0'                ; converte eax para ASCII
        	mov [hundreds], eax         ; copia de eax para hundreds
        	mov eax, 4                  ; teremos um StdOut
        	mov ebx, 1
        	mov ecx, hundreds           ; impressao de hundreds na tela
        	mov edx, 4                  ; tamanho de hundreds
        	int 0x80                    ; chamada ao kernel
        	jmp donefinal               ; pula para instrucao donefinal

    ; instrucao start_of_whiletens fara a impressao do segundo digito de buffer
    ;
    start_of_whiletens:
    	mov ecx, 0                     ; fazemos aqui ecx = 0
    	mov [tens], ecx                ; copia do valor de ecx para tens

        whiletens:                     ; instrucao que compara se buffer > 10
        	mov eax, [buffer]          ; copia do valor de buffer para eax
        	cmp eax, 10                ; compara eax com 10
        	jl donetens                ; Caso valor de eax < 10, pula para instrucao donetens
        	
            ; Caso contrario, subtraimos buffer com 10 e fazemos a contagem dessa ocorrencia
            ; ate que o buffer atualizado seja < 10. Essa contagem é registrada em [tens]
            ;
        	sub eax, 10                ; subtraimos o valor de eax com 10
        	mov [buffer], eax          ; copia do valor de eax para buffer
        	inc ecx	                   ; incrementamos ecx
        	mov [tens], ecx            ; copia do valor de ecx para tens
        	jmp whiletens              ; pula instrucao para whiletens

            ; instrucao donetens imprime [tens]
            ;   Primeiro, verificamos se houve alguma subtracao de buffer com 10,
            ;   ou seja, verificamos se tens > 0
            ;       Caso sim, imprimimos tens na tela
            ;       Caso contrario, pulamos para instrucao donefinal
            ;
        	mov ecx, 0                 ; fazemos aqui ecx = 0
            donetens:
            	mov eax, [tens]        ; copia do valor de tens para eax
            	cmp eax, 0             ; comparamos eax com 0
            	je donefinal           ; caso eax == 0, pulamos para instrucao donefinal
            	
                ; Caso contrario, imprimimos tens na tela
            	add eax, '0'           ; converte eax para ASCII
            	mov [tens], eax        ; copia do valor eax para tens
            	mov eax, 4             ; teremos um StdOut
            	mov ebx, 1
            	mov ecx, tens          ; impressao de tens na tela
            	mov edx, 4             ; tamanho de tens
            	int 0x80               ; chamada ao kernel

    ; intrucao donefinal imprime os ultimos digitos restantes em buffer
    ;
    donefinal:
    	mov eax, [buffer]          ; copía do valor de buffer para eax
    	add eax, '0'               ; converte eax para ASCII
    	mov [buffer], eax          ; copia do valor de eax para buffer

    	mov eax, 4                 ; teremos um StdOut
    	mov ebx, 1
    	mov ecx, buffer            ; impressao do buffer na tela
    	mov edx, 12                ; tamanho do buffer
    	int 0x80                   ; chamada ao kernel

    	mov eax, 4                 ; teremos um StdOut
    	mov ebx, 1
    	mov ecx, println           ; impressao de um pula linha "\n"
    	mov edx, printsize         ; tamanho do pula linha
    	int 0x80                   ; chamada ao kernel
    	
    ;out:
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

            ;************************************
            ;   inicio da divisao [I] / [J] (faremos aqui if ((i % j) == 0) )
            ;************************************
            mov ecx, [J]        ; copia o valor da memoria [J] para eax
            mov eax, [I]        ; copia o valor da memoria [I] para ecx

            xor edx, edx        ; limpamos edx para garantir que nao exista resto de memoria em edx
            div ecx             ; fizemos aqui a operacao: [I] / [J]
            mov eax, edx        ; copiamos o resto da divisao para eax
            cmp eax, 0
            je not_a_prime      ; Caso sim, nao teremos um primo. Entao pula para not_a_prime
            jmp continue        ; Caso contrario, pulamos para a instrucao continue

            ; Como nao temos um primo, atualizaremos a variavel booleana prime para 0
            ;
            not_a_prime:        ;faremos [boolean_prime] = 0
                mov ecx, 0                  ; fazemos eax = 0
                mov [boolean_prime], ecx    ; copia o valor da memoria eax para boolean_prime
                jmp loop_J_End

            ; Incrementamos o valor de J, e voltamos para loop_J_Start
            ;
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
        int 0x80           ; chamada ao kernel
