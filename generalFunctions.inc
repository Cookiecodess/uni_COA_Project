; generalFunctions.inc

PrintHeader PROTO
WriteStrArr PROTO

.code
;--------------------------------------------------------
; PrintHeader
;
; Description:
;   Prints a formatted header with top and bottom borders of `=` characters.
;
; Parameters:
;   EAX  - (DWORD) Address of the header string (offset)
;   EBX  - (DWORD) Length of the header string
;
; Returns:
;   None
;
; Example usage:
;   mov eax, offset header
;   mov ebx, lengthof header
;   call PrintHeader
;--------------------------------------------------------
PrintHeader PROC
    push ebp        ; save old base pointer
    mov ebp, esp    ; store the base address of the stack frame
    sub esp, 8      ; allocate memory for local variables

    ; store local variables
    mov dword ptr [ebp-4], eax
    mov dword ptr [ebp-8], ebx

    ; print top border
    ; for (2*leftRightPadding + lengthof(header)) { print('='); }
    mov eax, leftRightPadding
    mov ecx, 2
    mul ecx
    add eax, dword ptr [ebp-8]  
    mov ecx, eax
PrintLoop:
    mov al, equalSign
    call WriteChar
    loop PrintLoop

    ; print newline
    call Crlf  ; Irvine32 provides this

    ; print left padding spaces
    mov ecx, leftRightPadding
SpaceLoop:
    mov al, ' '
    call WriteChar
    loop SpaceLoop

    ; print header text
    mov edx, [ebp-4]  ; Adjusted stack offset
    call WriteString

    ; print newline
    call Crlf

    ; print bottom border
    ; for (2*leftRightPadding + lengthof(header)) { print('='); }
    mov eax, leftRightPadding
    mov ebx, 2
    mul ebx
    add eax, [ebp-8]  ; Adjusted
    mov ecx, eax
PrintLoop2:
    mov al, equalSign
    call WriteChar
    loop PrintLoop2

    ; print newline
    call Crlf
    
    ; restore stack pointer
    mov esp, ebp
    ; restore base pointer
    pop ebp
    ret
PrintHeader ENDP



;--------------------------------------------------------
; WriteStrArr
;
; Description:
;   Prints a numbered list of an array
;
; Parameters:
;   [EBP+8]  - (DWORD) Offset address of the string array
;
; Returns:
;   EAX  - (DWORD) Length of the string array
;
; Example usage:
;   .data
;   stringArray		  BYTE  "hello",0,"world",0,"this",0,"test",0,"dope",0,0
;   stringArrayLength DWORD ?
;
;   .code
;       push offset stringArray
;       call WriteStrArr
;       mov stringArrayLength, eax		; stringArrayLength = eax = 5
;--------------------------------------------------------
WriteStrArr PROC
	push ebp		; save current base pointer first
	mov ebp, esp	; move sp to bp for us to access the parameters stored in the stack

	; save old values of ESI and EDX
	push esi
	push edx

	mov esi, [ebp+8] ; load the address of the string array to esi
	mov al, SPACE  
	call WriteChar

	; printing the index number
	mov edx, 1
	mov eax, edx
	call writeDec
	mov al, ")"
	call writeChar
	mov al, " "
	call writeChar
	
	writeDatShitOut:

		mov al, [esi]		; load character into al then compare is it 0
		cmp al, 0			; if its 0, we know to print new line and write next string
		je nextShit

		call writeChar		; write that char out if not equal
		inc esi
		jmp writeDatShitOut


	nextShit:
		call crlf
		inc esi

		cmp byte ptr [esi], 0		; if the next char is also 0 then we know its the end of the array
		je done

		mov al, SPACE  
		call WriteChar

		inc edx				; writing the index number
		mov eax, edx
		call writeDec
		mov al, ")"
		call writeChar
		mov al, " "
		call writeChar

		jmp writeDatShitOut



	done:

	; store return value (length of the string array) in EAX
	mov eax, edx

	; restore old ESI and EDX
	pop edx
	pop esi

	pop ebp			; restore the initial base pointer before returning back to caller
	ret 4			; clear the stack pointer before returning

WriteStrArr ENDP