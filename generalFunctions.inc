; generalFunctions.inc


; EXTERN FillConsoleOutputCharacterA:PROC
; EXTERN FillConsoleOutputAttribute:PROC
; EXTERN ScrollConsoleScreenBuffer:PROC

CalcStrArrSize PROTO
CalculateDistanceBetweenStations PROTO
CalculateFeeNew PROTO
ClearConsoleBuffer PROTO
FillConsoleOutputAttribute PROTO STDCALL,
    hConsoleOutput:DWORD,              ; Handle to the console output buffer
    wAttribute:WORD,                   ; Attribute (e.g., current text color)
    nLength:DWORD,                     ; Number of cells to fill
    dwWriteCoord:COORD,                ; Starting coordinate
    lpNumberOfAttrsWritten:PTR DWORD  ; Out: actual number written
FillConsoleOutputCharacterA PROTO STDCALL, 
    hConsoleOutput:DWORD,          ; Handle to the console output buffer
    cCharacter:BYTE,               ; Character to be written (e.g., ' ')
    nLength:DWORD,                 ; Number of characters to write
    dwWriteCoord:COORD,            ; Starting position
    lpNumberOfCharsWritten:PTR DWORD  ; Out: actual number written
GetConsoleScreenBufferInfo PROTO,
    outHandle:DWORD,
    pBufferInfo:PTR CONSOLE_SCREEN_BUFFER_INFO
GetStrArrElem PROTO
PrintErrorMessage PROTO
PrintHeader PROTO
PrintSpacesForCenterAlign PROTO

ScrollConsoleScreenBuffer EQU <ScrollConsoleScreenBufferA>

ScrollConsoleScreenBuffer PROTO,
	hConsoleOutput:DWORD,
	lpScrollRectangle:PTR SMALL_RECT,
	lpClipRectangle:PTR SMALL_RECT,
	dwDestinationOrigin:COORD,
	lpFill:PTR CHAR_INFO

; EXTERN ScrollConsoleScreenBufferA@20:NEAR
; ScrollConsoleScreenBuffer TEXTEQU <ScrollConsoleScreenBufferA@20>

SetConsoleCursorInfo PROTO, 
	hConsoleOutput:DWORD, 
	lpConsoleCursorInfo:PTR CONSOLE_CURSOR_INFO
SetConsoleCursorPosition PROTO,
	hConsoleOutput:DWORD, 
	dwCursorPosition:COORD
SetCursorVisibility PROTO isVisible:DWORD
WriteAlignCenter PROTO
WriteAlignRight PROTO
; WriteMenu PROTO
; WriteStrArr PROTO
WriteStrArr PROTO
WriteStringInColor PROTO
WriteStringInRed PROTO
	

.data
	; FOR PrintHeader -------------------------------------------------------------------
	equalSign           byte '='
    leftRightPadding    dword 40

	; consoleHandle		dword ?


.code
; USAGE EXAMPLE:
; mov edx, offset strArray
; call CalcStrArrSize
; mov strArraySize, eax
CalcStrArrSize PROC uses esi edx
		mov esi, edx	; Initialize pointer
		xor eax, eax	; Initialize counter

	FindNextString:
		inc esi			; Move pointer forward
		cmp byte ptr [esi], 0		; Is it pointng to a null terminator?
		jne FindNextString
		inc eax			; It is! counter++
		inc esi			; Move pointer past the null terminator
		cmp byte ptr [esi], 0		; Is the char right next door ANOTHER null terminator?
		jne FindNextString			; No? Ok, then we're at the start of a new string. Let's find its null terminator.			
		ret							; Yes? That means it's the end of the array. EAX now stores the no. of strings in the array.
CalcStrArrSize ENDP




; Example:
;		push 0
;		push 2
;		call CalculateDistanceBetweenStations 
;		mov distanceVar, eax
;
; TODO: Still lacks validation, must improve in future
CalculateDistanceBetweenStations PROC 
	push ebp
	mov ebp, esp

		push esi

			mov esi, dword ptr [ebp+8]
			mov eax, dword ptr distances[esi*4]
			mov esi, dword ptr [ebp+12]
			sub eax, distances[esi*4]
			jge not_negative
			neg eax		; convert to positive if it's a negative number
		not_negative:	

		pop esi

	mov esp, ebp
	pop ebp
	ret 8
CalculateDistanceBetweenStations ENDP



; Example:
;		push 0
;		push 2
;		call CalculateFeeNew 
;		mov ticketPrice, eax
CalculateFeeNew PROC
	push ebp
	mov ebp, esp
    
		push ebx
		push edx
		
			push dword ptr [ebp+8]
			push dword ptr [ebp+12]
			call CalculateDistanceBetweenStations	; EAX = distance to travel
			
			;calculate fee (distance / 2)
			mov ebx, 2					
			xor edx, edx
			div ebx						; EAX = Ticket fee

		pop edx ;restore edx
		pop ebx
	
	mov esp, ebp
	pop ebp
	ret 8
CalculateFeeNew ENDP



; failed to clear entire buffer. only clears visible area.
ClearConsoleBuffer PROC
	LOCAL csbi:CONSOLE_SCREEN_BUFFER_INFO  ; Store the console info
	LOCAL scrollRect:SMALL_RECT             ; Scroll rectangle
	LOCAL scrollTarget:COORD                ; Scroll target
	LOCAL fill:CHAR_INFO                    ; Character fill for clearing
	LOCAL hConsole:DWORD                    ; Handle for the console

	; Get the handle to the console output
    invoke GetStdHandle, STD_OUTPUT_HANDLE
    mov hConsole, eax

    ; Get the console screen buffer info
    invoke GetConsoleScreenBufferInfo, hConsole, ADDR csbi
    ; If failed to get buffer info, exit
    ; test eax, eax
    ; jz exit

    ; Set the scroll rectangle to the entire buffer
    mov ax, csbi.dwSize.X         ; Width of the buffer
    mov scrollRect.Right, ax
    mov ax, csbi.dwSize.Y         ; Height of the buffer
    mov scrollRect.Bottom, ax
    mov scrollRect.Left, 0
    mov scrollRect.Top, 0

    ; Set the scroll target to move the buffer up (scroll it off the screen)
    mov scrollTarget.X, 0
    neg csbi.dwSize.Y              ; Scroll up by the height of the buffer
	mov ax, csbi.dwSize.Y
    mov scrollTarget.Y, ax

    ; Set the fill character to space (' ') and use the console's text attributes
    mov fill.Char.UnicodeChar, ' '  ; Fill with space
	mov ax, csbi.wAttributes
    mov fill.Attributes, ax

    ; Scroll the console screen buffer
    lea edx, scrollRect
    ; lea ecx, scrollTarget
    lea ebx, fill
    invoke ScrollConsoleScreenBuffer, hConsole, edx, NULL, scrollTarget, ebx

    ; Move the cursor to the top-left corner (0,0)
    mov csbi.dwCursorPosition.X, 0
    mov csbi.dwCursorPosition.Y, 0
    ; lea edx, csbi.dwCursorPosition
    invoke SetConsoleCursorPosition, hConsole, csbi.dwCursorPosition

; exit:
    ; Exit the program
    ; invoke ExitProcess, 0

	ret
ClearConsoleBuffer ENDP







; ClearConsoleBuffer PROC USES eax ebx edx
	; LOCAL hConsole:DWORD
    ; LOCAL csbi:CONSOLE_SCREEN_BUFFER_INFO
    ; LOCAL cells:DWORD
    ; LOCAL written:DWORD
    ; LOCAL coordZero:COORD

	; Initialize coordZero to <0,0>
	; mov coordZero.X, 0
	; mov coordZero.Y, 0

    ; Get handle to console output
    ; INVOKE GetStdHandle, STD_OUTPUT_HANDLE
    ; mov hConsole, eax

    ; Get console screen buffer info
    ; INVOKE GetConsoleScreenBufferInfo, hConsole, ADDR csbi

    ; Calculate total number of cells (rows * columns)
    ; movzx eax, csbi.dwSize.Y
    ; mov eax, 300 ; an arbitrarily large height
    ; movzx ebx, csbi.dwSize.X
    ; mul ebx
    ; mov cells, eax

    ; Fill entire screen with spaces
    ; INVOKE FillConsoleOutputCharacterA,
        ; hConsole,
        ; ' ',                ; ASCII space
        ; cells,
        ; coordZero,
        ; ADDR written

    ; Fill all cells with current text attribute
    ; mov ax, csbi.wAttributes
    ; INVOKE FillConsoleOutputAttribute,
        ; hConsole,
        ; csbi.wAttributes,
        ; cells,
        ; coordZero,
        ; ADDR written

    ; Reset cursor to top-left
    ; INVOKE SetConsoleCursorPosition,
        ; hConsole,
        ; coordZero

    ; ret
; ClearConsoleBuffer ENDP





;--------------------------------------------------------
; GetStrArrElem
;
; Description:
;   Get the string at the provided index in a provided string array
;
; Call args:
;	EAX - (DWORD) index to look for
;	EDX - (DWORD) offset of string array 
;
; Returns:
;   EDX  - (DWORD) offset of found string 
;				   OR if ERROR, EDX = -1
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
;
;   .code
;		mov eax, index
;		mov edx, offset stringArray
;		call GetStrArrElem
; 
;		cmp edx, -1
;		je outOfBoundsError
;		call WriteString				; EDX = offset
;	OutOfBoundsError:
;		call PrintErrorMessage		; print error message ONLY IF errorCode != 0
;--------------------------------------------------------
GetStrArrElem PROC USES eax ecx esi
		; ECX = index to look for
		mov ecx, eax

		; Calculate size of string array
			; EDX already stores the offset of str arr
		call CalcStrArrSize
			; EAX now stores size of str arr

		; Check for index-out-of-bounds error
		cmp ecx, eax	; target index >= array size? ABORT
		jge abort
		cmp ecx, 0		; target index < 0? ABORT
		jl abort

		; index not out of bounds! we can now start executing the actual function code

		mov esi, edx ; load the address of the string array to esi
		mov eax, 0					; initialize index to 0
		; mov ecx, dword ptr [ebp+12]  ; times to loop

	; at the start of each string, check if EAX (current index) is the index we're looking for (ECX)
	findString:
		cmp eax, ecx	; is the current index what we looking for?
		je grabString				; yes? get that sweet, sweet element

		inc eax						; no? increment the index

	; have we reached the null terminator of the current string?
	findNull:
		inc esi						; move esi forward
		cmp byte ptr [esi], 0		; reached null terminator of a string yet?
	
		; haven't reached null? keep looking
		jne findNull

		; reached the null terminator of the current string!
	foundNull:
		inc esi	; move from null terminator of the prev string, to the start of the next string
		jmp findString

	

	grabString:
		mov edx, esi		; load offset of the target string into EDX. This will be the return value.
		jmp return
	
	abort:
		mov eax, 1
		mov errorCode, eax			; Set error code = 1
									; EBX will still store the length of array
		mov edx, -1		; Return EDX = -1, to indicate error

	return:
		ret 			; clear the stack pointer before returning
GetStrArrElem ENDP






; Print error message
; No call args, no return args
PrintErrorMessage PROC
		; Return if no error (error code = 0)
		cmp errorCode, 0
		je return

		; Print error message
		mov eax, errorCode
		dec eax
		mov eax, [errorMessages + eax * 4]
		mov edx, eax
		call WriteStringInRed

		; Reset errorCode to 0
		mov errorCode, 0
	return:
		ret
PrintErrorMessage ENDP




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






SetCursorVisibility PROC USES eax, isVisible:DWORD
	LOCAL cursorInfo:CONSOLE_CURSOR_INFO
	LOCAL consoleHandle:DWORD

	; Set cursor size and visibility
	mov cursorInfo.dwSize, 10
	mov eax, isVisible
	mov cursorInfo.bVisible, eax

	; Get the console output handle (Irvine library function)
    INVOKE GetStdHandle, STD_OUTPUT_HANDLE
    mov consoleHandle, eax
    
    ; Hide the cursor
    INVOKE SetConsoleCursorInfo, 
        consoleHandle,
        ADDR cursorInfo

	ret
SetCursorVisibility ENDP


; ECX = length of string
; Returns number of spaces printed in EAX
PrintSpacesForCenterAlign PROC USES ebx ecx edx
	LOCAL consoleHandle:DWORD
	LOCAL bufferInfo:CONSOLE_SCREEN_BUFFER_INFO
	LOCAL strLen:DWORD
	LOCAL spacesPrinted:DWORD

	mov strLen, ecx

	; Get the console output handle (Irvine library function)
    INVOKE GetStdHandle, STD_OUTPUT_HANDLE
    mov consoleHandle, eax

	; Get console screen buffer info
	INVOKE GetConsoleScreenBufferInfo, consoleHandle, ADDR bufferInfo

	; Get width of the console screen
	; note: CONSOLE_SCREEN_BUFFER_INFO.dwSize.x is of type WORD
	movzx eax, bufferInfo.dwSize.x
	sub eax, strLen
	jl StringExceedsWidth ; print no spaces if string length exceeds width

	mov edx, 0	; EDX:EAX - ensures that the higher bits of the dividend is 0.
	mov ebx, 2
	div ebx

	; EDX:EAX (screenWidth - strLen) / EBX (2) 
	; EAX = quotient, EDX = remainder

	mov spacesPrinted, eax
	mov ecx, spacesPrinted
	mov al, ' '
PrintSpace:
	call WriteChar
	loop PrintSpace

	mov eax, spacesPrinted
	ret

StringExceedsWidth:	
	mov eax, 0	; spaces printed = 0
	ret
PrintSpacesForCenterAlign ENDP




; EDX = offset of string
; Returns no. of spaces printed in EAX
WriteAlignCenter PROC USES ecx edx

	; Get string length
	call StrLength ; returns string length in EAX
	mov ecx, eax

	call PrintSpacesForCenterAlign ; accepts string length in ECX, returns no. of spaces printed in EAX
	push eax ; save no. of spaces printed
	call WriteString ; prints string whose offset is stored in EDX
	call CrLf

	pop eax ; restore no. of spaces printed. EAX is the return value.
	ret
WriteAlignCenter ENDP






; Example usage:
;     lea edx, errorMsg
;     mov ecx, sizeof errorMsg
;     call WriteAlignRight
WriteAlignRight PROC
		push ebp
		mov ebp, esp

		; Allocate memory for 2 local variables
		sub esp, 8

		; Store arguments in local variables
		mov dword ptr [ebp-4], edx
		mov dword ptr [ebp-8], ecx

		; save original values of registers
		push edx
		push ecx

		; clear edx
		xor edx, edx		
	
		; get max no. of columns in console
		call GetMaxXY		; dl = number of columns in console
		movzx edx, dl		; we only want dl, fill the rest with 0's

		; edx = number of columns - (ticketTypeArr.length() - 1)
		sub edx, dword ptr [ebp-8]
		inc edx

		; if edx >= 0 (string length not larger than console width), continue
		jge continue
		; else, write string as normal and abort
		mov edx, dword ptr [ebp-4]
		call WriteString
		jmp done

	continue:
		; Left-pad with spaces
		mov ecx, edx		; ECX = number of columns - (ticketTypeArr.length() - 1)
		mov al, SPACE		; load SPACE character
	SpaceLoop:				
		call WriteChar
		loop SpaceLoop

		; write string
		mov edx, dword ptr [ebp-4]
		call WriteString

	done:
		; restore old values of registers
		pop ecx
		pop edx

		; restore stack pointer
		mov esp, ebp
		; restore base pointer
		pop ebp
		ret
WriteAlignRight ENDP







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
; WriteMenu PROC
		; push ebp
		; mov ebp, esp

		; save old values of registers
		; push edx

		; print list of ticket types
		; push dword ptr [ebp+16]
		; call WriteStrArr				; EAX = length of string array
		; mov ebx, eax					; EBX = EAX = length of string array; EBX is the second return value
	

		; call CRLF

		; print selection prompt
		; mov edx, dword ptr [ebp+12]
		; call WriteString
		; Print the value in EAX, which is the length of string array
		; call WriteDec
		; mov edx, dword ptr [ebp+8]
		; call WriteString

		; read input (number of selected option)
		; mov edx, offset inputBuffer		; offet of input buffer
		; mov ecx, MAX					; number of non-null chars to read
		; call ReadString
		; call ParseDecimal32				; parse input (a signed decimal integer string)
										; into 4-byte binary, 
										; stored in EAX.
		; dec eax							; minus 1 to get array index (0-based)

		; Check for out-of-bounds error
		; cmp eax, 0
		; jl abort
		; cmp eax, ebx	; EBX is the length of the string array
		; jge abort

		; If able to reach this point, then index is good.
		; EAX will be returned as the selected index.

		; call CrLf
		; jmp done

	; abort:
		; mov eax, -1		; EAX = -1 indicates out-of-bounds error!
						; Note: EBX will still be returned as the length of string array.

	; done:
		; restore old values of registers
		; pop edx

		; pop ebp
		; ret 12
; WriteMenu ENDP








; NOTE: THIS FUNCTION IS LEGACY CODE
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
; WriteStrArr PROC
		; push ebp		; save current base pointer first
		; mov ebp, esp	; move sp to bp for us to access the parameters stored in the stack

		; save old values of ESI and EDX
		; push esi
		; push edx

		; mov esi, [ebp+8] ; load the address of the string array to esi
		; mov al, TAB
		; call WriteChar

		; printing the index number
		; mov edx, 1
		; mov eax, edx
		; call writeDec
		; mov al, ")"
		; call writeChar
		; mov al, " "
		; call writeChar
	
	; writeDatShitOut:

		; mov al, [esi]		; load character into al then compare is it 0
		; cmp al, 0			; if its 0, we know to print new line and write next string
		; je nextShit

		; call writeChar		; write that char out if not equal
		; inc esi
		; jmp writeDatShitOut


	; nextShit:
		; call crlf
		; inc esi

		; cmp byte ptr [esi], 0		; if the next char is also 0 then we know its the end of the array
		; je done

		; mov al, TAB
		; call WriteChar

		; inc edx				; writing the index number
		; mov eax, edx
		; call writeDec
		; mov al, ")"
		; call writeChar
		; mov al, " "
		; call writeChar

		; jmp writeDatShitOut



	; done:

		; store return value (length of the string array) in EAX
		; mov eax, edx

		; restore old ESI and EDX
		; pop edx
		; pop esi

		; pop ebp			; restore the initial base pointer before returning back to caller
		; ret 4			; clear the stack pointer before returning

; WriteStrArr ENDP











;--------------------------------------------------------
; WriteStrArr
;
; Description:
;   Prints a numbered list of an array
;
; Call args:
;   [EBP+12]  - (DWORD) Offset address of the string array
;   [EBP+8]   - (DWORD) Length of the string array
;
; Returns:
;   None
;
; Example usage:
;   .data
;   string1				BYTE  "hello",0
;	string2				BYTE  "world",0
;	string3				BYTE  "this",0
;	string4				BYTE  "test",0
;	string5				BYTE  "dope",0
;	stringArr			DWORD OFFSET string1, OFFSET string2, OFFSET string3, OFFSET string4, OFFSET string5
;
;   .code
;       push offset stringArr
;       push lengthof stringArr
;       call WriteStrArr
;--------------------------------------------------------
WriteStrArr PROC
		push ebp		; save current base pointer first
		mov ebp, esp	; move sp to bp for us to access the parameters stored in the stack

		; save old values of registers that will be used
		push esi
		push eax
		push ebx
		push edx

		mov esi, [ebp+12] ; load the address of the string array to esi
		mov ebx, 1		; initialize index counter

		mov ecx, [ebp+8]	; load the length of array to ecx (times to loop)
	writeAllDatShitOut:
		; print tab character
		mov al, TAB  
		call WriteChar

		; print the index number
		mov eax, ebx
		call writeDec
		mov al, ")"
		call writeChar
		mov al, " "
		call writeChar

		; print one string
		mov edx, dword ptr [esi]	; ESI points to the offset of a string element. We dereference ESI to get that offset, and store the offset in EDX.
		call WriteString

		; print newline
		call CrLf

		; Increment index number and ESI
		inc ebx
		add esi, 4	; the array is an array of offset addresses, which are 4 bytes each. We add 4 ESI to make it point to the next string's offset address.
		
		loop writeAllDatShitOut

	done:
		; restore values of registers used
		pop edx
		pop ebx
		pop eax
		pop esi

		pop ebp			; restore the initial base pointer before returning back to caller
		ret 8			; clear the 2 arguments that were pushed to stack from the stack

WriteStrArr ENDP



; EAX = color constant (based on the SetTextColor function from Irvine)
; EDX = offset of string
WriteStringInColor PROC USES eax edx
	call SetTextColor
	call WriteString
	mov eax, NORMAL_COLOR
	call SetTextColor

	ret
WriteStringInColor ENDP



; Example usage:
;     mov edx, offset errorMsg
;     call WriteStringInRed
WriteStringInRed PROC USES eax
	mov eax, lightRed+(black*16)		; set to red color
	call SetTextColor
	call WriteString			; write error message in red
	mov eax, lightGray+(black*16)	; reset text color
	call SetTextColor

	ret
WriteStringInRed ENDP







