INCLUDE Irvine32.inc

.data
SPACE = 32
;TICKETING--------------------------------------------------------------------
ticketTypeArr		BYTE	"Single Journey",0,
						"Daily Pass",0,
						"Weekly Pass",0,
						"Monthly Pass",0,
						0	; null terminator for array
; ticketTypeArrLength	DWORD	?
promptChooseHead	BYTE	"Please select a ticket for more options (1-",0
promptChooseTail	BYTE	"): ",0

msgChose			BYTE	"You chose ",0


.code 
main PROC
	
	push offset ticketTypeArr
	push offset promptChooseHead
	push offset promptChooseTail
	call WriteMenu
	; returned selected index in EAX, length of passed string array in EBX

	; call dumpregs

	push offset ticketTypeArr
	push ebx		; length of ticketTypeArr. This is the 2nd return value of WriteMenu
	push eax		; selected index. This is the 1st return value of WriteMenu
	call GetStrArrElem
	; returned offset of string at given index in EAX

	mov edx, offset msgChose
	call WriteString
	; Write the string from the array
	mov edx, eax
	call WriteString



	; print list of ticket types
	; push offset ticketTypeArr
	; call WriteStrArr
	; mov ticketTypeArrLength, eax

	; call CRLF

	; lea edx, promptChooseHead
	; call WriteString
	; mov eax, ticketTypeArrLength    ; the length value is already in EAX
	; call WriteDec
	; lea edx, promptChooseTail
	; call WriteString

	; get input
	; call ReadChar

	; call CrLf
	; fill all bits in EAX except AL with zeroes
	; movzx eax, al
	; the characters '0' to '9' are 30h to 39h in ASCII. Subtract by 30 to convert to number.
	; sub eax, 30h
	; call WriteDec
	
	

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

; Print a menu with a list of options, and a selection prompt.
; call args: 
	; [ebp+16] - (DWORD) string array offset address
	; [ebp+12] - (DWORD) prompt head offset address
	; [ebp+8] - (DWORD) prompt tail offset address
;
; return args: 
;     EAX - (DWORD) the selected index of option array
;     EBX - (DWORD) array length
WriteMenu PROC
	push ebp
	mov ebp, esp

	; save old values of registers
	push edx

	; print list of ticket types
	push dword ptr [ebp+16]
	call WriteStrArr				; EAX = length of string array
	mov ebx, eax					; EBX = length of string array; this is the second return value
	

	call CRLF

	; print selection prompt
	mov edx, dword ptr [ebp+12]
	call WriteString
	; Print the value in EAX, which is the length of string array
	call WriteDec
	mov edx, dword ptr [ebp+8]
	call WriteString

	; get input
	call ReadChar

	call CrLf
	; fill all bits in EAX except AL with zeroes
	movzx eax, al
	; the characters '0' to '9' are 30h to 39h in ASCII. Subtract by 30 to convert to number.
	sub eax, 30h
	dec eax			; minus 1 to get array index (0-based)
	; eax (the selected index) will be the return value.

	; restore old values of registers
	pop edx

	pop ebp
	ret 12
WriteMenu ENDP

; get a string element at a provided index in a provided array
; call args
; [ebp+16] - (DWORD) array offset
; [ebp+12] - (DWORD) array length
; [ebp+8] - (DWORD) index we're looking for
GetStrArrElem PROC
	push ebp		; save current base pointer first
	mov ebp, esp	; move sp to bp for us to access the parameters stored in the stack

	; save old values of registers
	push esi
	; push ecx

	mov esi, dword ptr [ebp+16] ; load the address of the string array to esi
	mov edx, 0					; initialize index
	; mov ecx, dword ptr [ebp+12]  ; times to loop

; at the start of each string, check if edx (current index) is the one we're looking for
findString:
	cmp edx, dword ptr [ebp+8]	; is the current index what we looking for?
	je grabString				; yes? get that sweet, sweet element

	inc edx						; no? increment the index
	; inc esi
; have we reached the null terminator of the current string?
findNull:
	inc esi						; move esi forward
	cmp byte ptr [esi], 0		; reached null terminator of a string yet?
	je foundNull
	
	; haven't reached null? keep looking
	jmp findNull

foundNull:
	inc esi	; move from null terminator of the prev string, to the start of the next string
	jmp findString

	

grabString:
	mov eax, esi		; load offset of the target string into EAX. This will be the return value.
	

	; restore old values of registers
	; pop ecx
	pop esi

	pop ebp			; restore the initial base pointer before returning back to caller
	ret 12			; clear the stack pointer before returning
GetStrArrElem ENDP
	
end main