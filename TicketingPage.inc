; INCLUDE generalFunctions.inc

.data
	; SPACE = 32
	;GENERAL------------------------------------------------------------------
	; MAX	= 20								; max characters to read
	; inputBuffer			byte  MAX+1 dup(?)  ; room for null character

	;TICKETING--------------------------------------------------------------------
	headerTicketType	BYTE	"Choose ticket type",0
	ticketTypeArr		BYTE	"Single Journey",0,
							"Daily Pass",0,
							"Weekly Pass",0,
							"Monthly Pass",0,
							"Back",0,
							0	; null terminator for array
	backIndex			DWORD   4		; note that this is an index. Index 4 = 5th element
	ticketTypeArrLength	DWORD	?
	promptChooseHead	BYTE	"Please select a ticket for more options (1-",0
	promptChooseTail	BYTE	"): ",0
	promptChoose	BYTE	"Please select a ticket for more options.",0

	msgChose			BYTE	"You chose ",0
	; selectionIndex		DWORD	?
	


.code 
TicketingPage PROC USES eax ebx edx
		invoke InitMenu, offset headerTicketType, offset ticketTypeArr, offset promptChoose, 0, 0

		; initial clear screen
		; call ClrScr

		; Print header
		; lea eax, headerTicketType
		; mov ebx, lengthof headerTicketType
		; call PrintHeader
	; displayMenuLoop:

		; print list of options and a selection prompt
		; push offset ticketTypeArr
		; push offset promptChooseHead
		; push offset promptChooseTail
		; call WriteMenu
		; mov selectionIndex, eax
		; mov ticketTypeArrLength, ebx
		; returned: selected index in EAX, length of passed string array in EBX
		; length of string array is needed to call GetStrArrElem



		; push offset ticketTypeArr
		; push ticketTypeArrLength		; length of ticketTypeArr. This is the 2nd return value of WriteMenu
		; mov eax, 4
		; push selectionIndex		; selected index. This is the 1st return value of WriteMenu
		; call GetStrArrElem
		; returned: offset of string in EAX

		; check for index-out-of-bounds error
		; cmp eax, -1
		; jne index_is_good
	
		; index is out of bounds!
		; call ClrScr							; Clear screen

		; lea eax, headerTicketType			; Print header again
		; mov ebx, lengthof headerTicketType
		; call PrintHeader

		; mov edx, offset errorOutOfBounds	; Print error message!
		; call WriteError
		; call CrLf							; print newlines (x2)
		; call CrLf
		; jmp displayMenuLoop					; reprint menu and selection prompt

	; index_is_good:
		; Did user select "Back"? If so, let's not print anything and just return.
		mov eax, currentSelection
		cmp eax, backIndex
		je go_back_whence_you_came

		mov edx, offset msgChose
		call WriteString
		;Write the string from the array
		mov edx, offset ticketTypeArr
		mov eax, currentSelection
		call GetStrArrElem
		call WriteString
		
		exit	; TEMP. TO-DO: Implement actions for each selection.
	
	go_back_whence_you_came:
		ret
TicketingPage ENDP