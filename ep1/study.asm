; STUDY Assembly....science

really good site:   http://www.tutorialspoint.com/assembly_programming/assembly_registers.htm
                    https://wiki.skullsecurity.org/Simple_Instructions

lastest update:
http://www.csee.umbc.edu/courses/undergraduate/CMSC313/spring04/burt_katz/lectures/Lect05/loops.html
http://www.csee.umbc.edu/courses/undergraduate/313/spring05/burt_katz/lectures/Lect14/program.html
http://www.tutorialspoint.com/assembly_programming/assembly_procedures.htm
http://www.tutorialspoint.com/assembly_programming/assembly_loops.htm
http://www.tutorialspoint.com/assembly_programming/assembly_numbers.htm
http://www.tutorialspoint.com/assembly_programming/assembly_strings.htm
http://www.tutorialspoint.com/assembly_programming/assembly_arrays.htm
https://courses.engr.illinois.edu/ece390/books/artofasm/CH11/CH11-4.html

-----------------------------------------------------------------------------------------
A Sample Macro to Implement For Loops (chapter x08) page x429 from .pdf

-----------------------------------------------------------------------------------------
what does int 80h does?
http://stackoverflow.com/questions/1817577/what-does-int-0x80-mean-in-assembly-code

-----------------------------------------------------------------------------------------
section .data

;nome 	  tipo   valor inicial
LF:       equ    0Ah			; LF = line feed "\n"

mensagem: db     'hello world', LF	; mensagem = nome, db = pseudo instruction (define byte) 
-----------------------------------------------------------------------------------------
; Procedures: CallProc(i, j, k+4);
source: https://courses.engr.illinois.edu/ece390/books/artofasm/CH11/CH11-4.html

	push    i
       	push    j
        mov     ax, k
        add     ax, 4
        push    ax
        call    CallProc
CallProc        
	proc    near
	pop     RtnAdrs
        pop     kParm
	pop     jParm
	pop     iParm
	push    RtnAdrs
                 .
                 .
                 .
	ret

There is, however, a better way. The 80x86's architecture allows you to use the bp (base pointer) register 
to access parameters passed on the stack. This is one of the reasons the 
disp[bp], [bp][di], [bp][si], disp[bp][si], and disp[bp][di] 
addressing modes use the stack segment rather than the data segment. 
The following code segment gives the standard procedure entry and exit code:

StdProc         
	proc    near
        push    bp
	mov     bp, sp
                 .
                 .
                 .
	pop     bp
	ret     ParmSize

ParmSize is the number of bytes of parameters pushed onto the stack before calling the procedure. 
In the CallProc procedure there were six bytes of parameters pushed onto the stack so ParmSize would be six.

Now the parameters can be fetched by indexing off the bp register:
	mov     ax, 8[bp]        ;Accesses the first parameteri
	mov     ax, 6[bp]        ;Accesses the second parameter
	mov     ax, 4[bp]        ;Accesses the third parameter

When returning to the calling code, the procedure must remove these parameters from the stack. 
To accomplish this, pop the old bp value off the stack and execute a ret 6 instruction. 
This pops the return address off the stack and adds six to the stack pointer, effectively removing the parameters from the stack.

The displacements given above are for near procedures only. When calling a far procedure,

    0[BP] will point at the old BP value,
    2[BP] will point at the offset portion of the return address,
    4[BP] will point at the segment portion of the return address, and
    6[BP] will point at the last parameter pushed onto the stack. 
-----------------------------------------------------------------------------------------
; c = a * b
section .data
    a dd 21
    b dd 2
section .bss
    c resd 1
section .text
global _start:
_start:
    push dword [b]
    push dword [a]
    call multiply
    add esp, 4 * 2	; pop 2 times 4 bytes (?)
    mov [c], eax
    			; print the answer?
    			; exit cleanly, as always
multiply:
    push ebp 		; save caller's ebp - they're probably using it
    mov ebp, esp
    ; sub esp, localsize ; if local variables needed

    mov  eax, [ebp + 8]
    imul dword [ebp + 12]
    ; result in edx:eax... we ignore edx...

; since we didn't use any local variables
; or otherwise alter esp, we could do without this next line
; if we had, this would restore esp
    mov esp, ebp
    pop ebp  	; restore caller's ebp
		; the "leave" instruction does both of the above two lines
    ret		; end of subroutine
-----------------------------------------------------------------------------------------
; EXAMPLE of printf calling external C function
http://www.csee.umbc.edu/portal/help/nasm/sample.shtml

; Assemble:	nasm -f elf -l printf.lst  printf1.asm
; Output:	a=5, eax=7

; #include 
; int main()
; {
;   int a=5;
;   printf("a=%d, eax=%d\n", a, a+2);
;   return 0;
; }
        extern	printf		; the C function, to be called

SECTION .data			; Data section, initialized variables

a:	dd	5		; int a=5;
fmt:    db "a=%d, eax=%d", 10, 0 ; The printf format, "\n",'0'


SECTION .text                   ; Code section.


        global main		; the standard gcc entry point
main:				; the program label for the entry point
        push    ebp		; set up stack frame
        mov     ebp,esp

	mov	eax, [a]	; load orput 'a' from store into register
	add	eax, 2		; a+2
	push	eax		; value of a+2 -> saves up eax
        push    dword [a]	; value of variable a
        push    dword fmt	; address of ctrl string
        call    printf		; Call C function
        add     esp, 12		; pop stack 3 push times 4 bytes -> as we pushed 3 times

        mov     esp, ebp	; takedown stack frame
        pop     ebp		; same as "leave" op

	mov	eax,0		;  normal, no error, return value
	ret			; return
----------------------------------------------------------------------------
