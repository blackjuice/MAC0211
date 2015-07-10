;*******************************************
; nome: LUCAS SUNG JUN HONG
; nUSP: 812 432 9
; 
; EP1 - Lab Prog (MAC 0211)
; 
; Compilacao:
;   Em caso de rodar o programa em uma maquina 64 bit:
;      # nasm -f elf32 ep1_8124329.asm
;      # ld -m elf_i386 -s -o ep1_8124329 ep1_8124329.o
;   Em caso de rodar o programa em uma maquina 32 bit:
;      # nasm -f elf ep1_8124329.asm
;      # ld ep1_8124329.o -o ep1_8124329
;
; Execucao:
;      # ./ep1_8124329
;
;*******************************************

; declaracao das variaveis nao-inicializadas
section .bss
        buffer     resb    12       ; reservamos 12 bytes
        num:       resb    4        ; reservamos 32 bits
        unit:      resb    4        ; reservamos 32 bits
        j:         resb    4        ; reservamos 32 bits
        limit:     resb    4        ; reservamos 32 bits

; declaracao das variaveis inicializadas
section .data
        LF:             equ     0Ah         ; para pular linha na saida (equivalente a "\n")
        println:        db      LF, 0       ; chamar 'println' equivalente a printf("\n")
        printsize:      equ     $ - println ; tamanho para o comando acima
        tens:           dd      0           ; declaramos uma variavel tens = 0
        hundreds:       dd      0           ; variavel hundreds = 0
        prime:          dd      0           ; prime = 0

;-------------------------------------------------------------------------------------------------
;; Inicio do codigo
section .text

    ; (for y = 2; y < x; y++), where x has interval 2..1000
    int2str:

        mov eax, ebp
        mov [buffer], eax

        xor eax,eax     ; just in case
        xor ebx,ebx
        xor ecx,ecx

    ;; Printing buffers
    ; If (buffer > 100)
    ;   decrement by 100 until buffer < 100 and increment hundreds, then print hundreds
    ; If (buffer > 10)
    ;   decrement by 10 until buffer < 10 and increment tens, then pŕint tens
    ; Then buffer

        mov ecx, 0
        mov [hundreds], ecx

    whilehundreds:
        mov eax, [buffer]
        cmp eax, 100
        jl donehundreds
        
        sub eax, 100
        mov [buffer], eax
        inc ecx     
        mov [hundreds], ecx
        jmp whilehundreds

        mov ecx, 0

    donehundreds:           ; printing hundreds
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
    ;    if (buffer > 101)
    ;       buffer = buffer - 100  (hundreds = 1)
    ;    we will have buffer = 01
    ;    if (buffer < 10 && hundreds > 0)
    ;       print extra '0'
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

    donefinal:
        mov eax, [buffer]
        add eax, '0'
        mov [buffer], eax

        mov eax, 4
        mov ebx, 1
        mov ecx, buffer
        mov edx, 4
        int 0x80
        jmp return

    return:
        mov eax, 4
        mov ebx, 1
        mov ecx, println
        mov edx, printsize
        int 0x80
        
    out:
        ret
;
;; main
global _start
_start:
    ; sys_write

        mov ecx, 2
    loopStart:
        cmp ecx, 1000

        push ecx        ; push ecx para guardar seu valor

        jge loopEnd
        
        mov eax, ecx

        ;mov ebp, ecx
        mov ebp, eax
        call int2str

        pop ecx
        inc ecx
        jmp loopStart

    loopEnd:
        ; sys_exit(return_code)
        mov eax, 1
        mov ebx, 0
        int 0x80

