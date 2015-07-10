
        xor eax,eax		; just in case
        xor ebx,ebx
        xor ecx,ecx

        mov eax, 1		; prime = 1
        mov [prime], eax       
        mov eax, [unit]		; eax tem valor de [unit]
        mov ecx, 2		; ecx = 2
	mov [j], ecx
; loop
l2_start:
	mov ecx, [j]
        cmp ecx, [unit]    	; (ecx - [unit])
        jge l2_end      	; if (ecx >= unit) jump to end

        xor edx, edx	; just in case 
        mov eax, [unit]
        mov ebx, ecx

	and eax, 0xfff
	and ebx, 0xfff
	div ebx

        mov eax, edx	; edx is the reminder, so store in eax
        cmp eax, 0	
        je primefalse	; if (eax % ebx == 0) prime = FALSE
        jmp primetrue	; else prime = TRUE
keep:
        add ecx, 1		; (ecx++)
	mov [j], ecx




example:
	mov eax, 1007
	mov ecx, 10
	xor edx, edx
	div ecx