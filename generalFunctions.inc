; generalFunctions.inc

PrintHeader PROTO
WriteStrArr PROTO
	
	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	SPACE =09h
.data
	; FOR PrintHeader -------------------------------------------------------------------
	equalSign           byte '='
    leftRightPadding    dword 40

	; ERROR MESSAGES -------------------------------------------------------------------
	errorOutOfBounds	BYTE	"Invalid option: Selected number not in list!",0

.code
;--------------------------------------------------------
; PrintHeader
;
; Description:
;   Prints a formatted header with top and bottom borders of `=` characters.
;
; Call args:
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



; Example usage:
;     mov edx, offset errorMsg
;     call WriteError
WriteError PROC USES eax
	mov eax, lightRed+(black*16)		; set to red color
	call SetTextColor
	call WriteString			; write error message in red
	mov eax, lightGray+(black*16)	; reset text color
	call SetTextColor

	ret
WriteError ENDP






;--------------------------------------------------------
; WriteMenu
;
; Description:
;   Prints a numbered list of options (just like WriteStrArr) AND a selection prompt
;
; Call args:
;	[ebp+16] - (DWORD) string array offset address
;	[ebp+12] - (DWORD) prompt head offset address
;	[ebp+8] - (DWORD) prompt tail offset address
;
; Returns:
;   EAX  - (DWORD) the selected index of option array
;				   OR -1 IF SELECTED NUMBER IS OUT OF BOUNDS
;   EBX  - (DWORD) Length of the string array
;
; Example usage:
;   .data
;   stringArray		  BYTE  "option 1",0,
;					  BYTE  "option 2",0,
;					  BYTE  "option 3",0,
;					  BYTE  "option 4",0,
;					  BYTE  0				; IMPORTANT: This last 0 acts as
;											; terminator for the whole array.
;   promptHead		  BYTE  "Select an option (1-",0
;   promptTail		  BYTE  "): ",0
;   stringArrayLength DWORD ?
;	selectionIndex	  DWORD ?
;
;   .code
;       push offset stringArray
;       push offset promptHead
;       push offset promptTail
;       call WriteMenu					; The prompt will be output as:
;										;    "Select an option (1-4): "
;
;       mov selectionIndex, eax			; if user input "3", selection = 2
;       mov stringArrayLength, ebx		; stringArrayLength = 4
;		
;--------------------------------------------------------
WriteMenu PROC
		push ebp
		mov ebp, esp

		; save old values of registers
		push edx

		; print list of ticket types
		push dword ptr [ebp+16]
		call WriteStrArr				; EAX = length of string array
		mov ebx, eax					; EBX = EAX = length of string array; EBX is the second return value
	

		call CRLF

		; print selection prompt
		mov edx, dword ptr [ebp+12]
		call WriteString
		; Print the value in EAX, which is the length of string array
		call WriteDec
		mov edx, dword ptr [ebp+8]
		call WriteString

		; read input (number of selected option)
		mov edx, offset inputBuffer		; offet of input buffer 
		mov ecx, MAX					; number of non-null chars to read
		call ReadString
		call ParseDecimal32				; parse input (a signed decimal integer string) 
										; into 4-byte binary, 
										; stored in EAX.
		dec eax							; minus 1 to get array index (0-based)

		; Check for out-of-bounds error
		cmp eax, 0
		jl abort
		cmp eax, ebx	; EBX is the length of the string array
		jge abort

		; If able to reach this point, then index is good.
		; EAX will be returned as the selected index.

		call CrLf
		jmp done

	abort:
		mov eax, -1		; EAX = -1 indicates out-of-bounds error!
						; Note: EBX will still be returned as the length of string array.

	done:
		; restore old values of registers
		pop edx

		pop ebp
		ret 12
WriteMenu ENDP








;--------------------------------------------------------
; WriteStrArr
;
; Description:
;   Prints a numbered list of an array
;
; Call args:
;   [EBP+8]  - (DWORD) Offset address of the string array
;
; Returns:
;   EAX  - (DWORD) Length of the string array
;
; Example usage:
;   .data
;   stringArray		  BYTE  "hello",0,
;					  BYTE  "world",0,
;					  BYTE  "this",0,
;					  BYTE  "test",0,
;					  BYTE  "dope",0,
;					  BYTE  0			; IMPORTANT: This last 0 acts as a 
;										; terminator for the whole array.
;
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
















;--------------------------------------------------------
; GetStrArrElem
;
; Description:
;   Get the string at the provided index in a provided string array
;
; Call args:
;	[ebp+16] - (DWORD) string array offset address
;	[ebp+12] - (DWORD) string array length
;	[ebp+8] - (DWORD) the index we're looking for
;
; Returns:
;   EAX  - (DWORD) pointer to the wanted string 
;   EBX  - (DWORD) Length of the string array
;
; Example usage:
;   .data
;   stringArray		  BYTE  "option 1",0,
;					  BYTE  "option 2",0,
;					  BYTE  "option 3",0,
;					  BYTE  "option 4",0,
;					  BYTE  0				; IMPORTANT: This last 0 acts as
;											; terminator for the whole array.
;	index			  DWORD 1
;   stringArrayLength DWORD 5
;
;   .code
;       push offset stringArray
;       push stringArrayLength
;       push index
;       call GetStrArrElem				
;
;       mov edx, eax
;		call WriteString
;--------------------------------------------------------
GetStrArrElem PROC
		push ebp		; save current base pointer first
		mov ebp, esp	; move sp to bp for us to access the parameters stored in the stack

		; save old values of registers
		push esi
		push edx

		; check for index-out-of-bounds
		mov edx, dword ptr [ebp+8]		; EDX = the index we're looking for
		cmp edx, dword ptr [ebp+12]		; index >= array length? ABORT
		jge abort
		cmp edx, 0						; index < 0? ABORT
		jl abort

		; index not out of bounds! we can now start executing the actual function code

		mov esi, dword ptr [ebp+16] ; load the address of the string array to esi
		mov edx, 0					; initialize index to 0
		; mov ecx, dword ptr [ebp+12]  ; times to loop

	; at the start of each string, check if edx (current index) is the one we're looking for
	findString:
		cmp edx, dword ptr [ebp+8]	; is the current index what we looking for?
		je grabString				; yes? get that sweet, sweet element

		inc edx						; no? increment the index

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
		jmp done
	
	abort:
		mov eax, -1			; return -1 to indicate failure
							; EBX will still store the return value: length of array

	done:
		; restore old values of registers
		pop edx
		pop esi

		pop ebp			; restore the initial base pointer before returning back to caller
		ret 12			; clear the stack pointer before returning
GetStrArrElem ENDP