include Irvine32.inc

.data
	stringarr BYTE "hello",0,"world",0,"this",0,"this",0,"test",0,0

.code
main PROC

	call dumpregs

	push OFFSET stringarr
	call WriteStrArr
	call dumpregs
	
	
	exit
main ENDP

 
WriteStrArr PROC
	push ebp		; save current base pointer first
	mov ebp, esp	; move sp to bp for us to access the parameters stored in the stack

	; save old values of ESI and EDX
	push esi
	push edx

	mov esi, [ebp+8] ; load the address of the string array to esi

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

end main