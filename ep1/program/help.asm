enunciado
=========

Entregar escrito em linguagem de montagem para o NASM um programa que calcula e imprime todos os números primos menores que 1000. 

Não é permitido usar nenhuma biblioteca de funções, como por exemplo a libc e 
deve-se implementar uma subrotina de nome int2str
					  =======
 que recebe como argumentos um inteiro de 32 bits
 e um endereço de um vetor de 12bytes

 e codifica neste vetor a representação decimal do inteiro fornecido usando a tabela ASCII,
 seguido de um LF (0AH)
 e de um 0.

 Deve-se usar a interrupção de sistema para escrita de string no dispositivo padrão de saída.  

Checking prime numbers algorithm
================================
http://www.cquestions.com/2012/02/check-given-number-is-prime-number-or.html

Assembly loops
==============
http://www.tutorialspoint.com/assembly_programming/assembly_loops.htm

Assembly, writing a loop
========================
http://www.dreamincode.net/forums/topic/166433-assembler-writing-a-simple-loop/

Getting what a buffer is
========================
http://stackoverflow.com/questions/13468991/how-do-i-use-a-buffer-in-an-assembly-procedure

http://www.csee.umbc.edu/courses/undergraduate/313/spring05/burt_katz/lectures/Lect14/program.html

-------------------------------------------------------------------------------------------------------
entendendo enunciado
====================

"recebe como argumentos um inteiro de 32 bits 
 e um endereço de um vetor de 12bytes e codifica 
 neste vetor a representação decimal do inteiro 
 fornecido usando a tabela ASCII"
----------------------------------------------------

 Essa subrotina é tipo uma função que recebe um número inteiro e o endereço de um vetor, 
 e "imprime" nesse vetor os dígitos do número recebido representados em código ASCII. 
 Em C seria algo do tipo: void(int n, char* str) - Mas muuuuito cuidado com essa analogia, 
 porque o número no caso tem 32 bits (uma double word, geralmente), e a "string" é um vetor 
 com 12 posições de um byte cada. Quem vai fazer a "interpretação" desse vetor como 
 caracteres para imprimir a mensagem na tela é o sistema operacional.

 é pq se vc imprimir o número 2 por exemplo, vai sair um símbolo estranho q é o q ele representa na tabela ascii

 Seria algo do tipo:
  mov eax, INTEIRO
  mov ebx, VETOR
  call int2str

 e na int2str você usa o ebx como se fosse um apontador para VETOR

 eu posso declarar esse vetor na seção .data
 algo como: 
  vetor: db  TIMES  12  0

 Como é uma variável não inicializada, teria que ser declarada na seção .bss, se não me engano. Algo assim:
  vetor: resb 12
---------------------------------------------------------------------------------------------------------------
; how to print numbers greater than 10

	mov ax, the_number
	xor cx, cx 	; digit counter (zerando register ecx)
	mov bx, 10
pushem:
	xor  dx, dx 	; important! - zerando register dx
	div  bx		; ax / bx -> quotient in ax, resto in dx
	add  dl, '0' 	; convert to character
	push dx  	; save 'em to get back in right order
	inc  cx		; count 'em
	test ax, ax  	; done? - set the flag sf to 1 if ax == 0
	jnz  pushem	; no, do more
popem:
	pop  ax 	; we're only interested in al
	int  29h 	; print al
	loop popem  	; until done

---------------------------------------------------------------------------------------------------------------
; How to print numbers greater then 10 in ARM200 Assembly language
http://gotoanswer.com/?q=How+to+print+numbers+greater+then+10+in+ARM200+Assembly+language

You could do so with a simple loop, dividing your number by 10 and storing the remainder each time, 
until the number finally gets to 0. You'll basically end up with an array of numbers from 0-9 
which you can then print one at a time. You have to store them before printing because you'll end up with each digit in reverse order. eg:
Number | Buffer
123    | { EMPTY }
12     | 3         (123 / 10 = 12, remainder 3)
1      | 3 2       (12 / 10 = 1, remainder 2)
0      | 3 2 1     (1 / 10 = 0, remainder 1)


Each time you add a number to the buffer, increment your counter. Once you've finished dividing and your number is now 0, 
you can then start printing. Loop from count to 0, printing out the number stored at Buffer + count.

For the buffer, if you're saying that each number can be up to 4 bytes (32 bits), 
than you know that in decimal the largest number that can be represented is either 2147483647 (signed) or 
4294967295 (unsigned). In both cases, the largest most amount of digits is 10, 
so you can allocate a buffer of 10 bytes (1 byte is enough to hold each digit from 0-9).

Another alternative (which I've done before doing the same task as you for a different chip) 
is to use the stack rather than have a buffer allocated, and push each digit onto the stack at each iteration in the loop. 
Both ways are fairly simple.

I'll let you come up with the code, since you're meant to be learning how to do it.

Edit: This is some pseudo code of the common method I described above:
Buffer[10]

Num = 123

count = 0
// Split the number into an array of digits
WHILE Num IS NOT 0
   Buffer[count] = Num % 10 // Store the remainder
   Num = Num / 10
   count++

count-- // count will be 1 more than the amount in Buffer

// Print the digits
WHILE count IS >= 0
   PRINT Buffer[count]
   count--


Since you're learning (ie, possible homework), I'll give general advice only.

Let's say you have the number 247 and you wanted to print out the three digits of it, one by one.

How can you get the hundreds digit 2 from 247 and leave 47 for the next iteration?

Put that value into a temporary variable and set a counter to zero. 
Then, while the temp value is greater than 99, subtract 100 from it and add 1 to the counter.

This will give you a counter of 2 and a temp value of 47. Use the 2 to output your digit (you state you can already do this).

Now move on to tens.

Set the counter back to zero.

Then, while the temp value is greater than 9, subtract 10 from it and add 1 to the counter.

This will give you a counter of 4 and a temp value of 7. Use the 4 to output your digit.

Finally, units.

Use the final remainder 7 to output the last digit.

Here's some assembler-like pseudo-code I used in another answer (slightly modified) to do a similar thing.
    val = 247

    units = val
    tens = 0
    hundreds = 0
loop1:
    if units < 100 goto loop2
    units = units - 100
    hundreds = hundreds + 1
    goto loop1
loop2:
    if units < 10 goto done
    units = units - 10
    tens = tens + 1
    goto loop2
done:
    if hundreds > 0:                 # Don't print leading zeroes.
        output hundreds
    if hundreds > 0 or tens > 0:
        output tens
    output units
    ;; hundreds = 2, tens = 4, units = 7.


And one other thing, all this stuff needs to go into subroutines so that you can re-use them. 
Having thirty-two copies of that algorithm above converted to assembly would be a very tedious piece of code.
---------------------------------------------------------------------------------------------------------------
; input and output a 2 digit number
.model small
    .stack 100h
    .data
        msg db "Enter a number: $"
        msg2 db "You have entered: $"
        num1 db 0
        num2 db 0
        temp db 0
        ten db 10
        readNum db 0
        t2 db 0
        t1 db 0
    .code
        mov ax,@data
        mov ds,ax

        call read
        call endL
        call write

proc endL
            mov dl,0ah
            mov ah,02h
            int 21h
            ret
            endp

proc read
            mov dx,offset msg
            mov ah,09h
            int 21h

            mov ah,01h
            int 21h
            sub al,48
            mov num1,al

            mov ah,01h
            int 21h
            sub al,48
            mov num2,al     

            mov al,num1
            mul ten
            add al,num2

            mov readNum,al
            ret
            endp

proc write
            mov dx,offset msg2
            mov ah,09h
            int 21h

            mov al,readNum
            mov ah,00
            div ten		; ten db 10
	; t2 <- dl
            mov dl,ah		; dl <- ah
            mov t2,dl		; t2 <- dl

            mov dl,al		; al <- al
            add dl,48		; add 48 to dl
            mov ah,02h
            int 21h

            mov dl,t2
            add dl,48
            mov ah,02h
            int 21h
        endp

    mov ax,4c00h
    int 21h

    end 
