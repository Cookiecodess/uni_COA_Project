INCLUDE Irvine32.inc
INCLUDE generalFunctions.inc

.data
	SPACE = 32
	;GENERAL------------------------------------------------------------------
	MAX	= 20								; max characters to read
	inputBuffer			byte  MAX+1 dup(?)  ; room for null character

	;TICKETING--------------------------------------------------------------------
	ticketTypeArr		BYTE	"Single Journey",0,
							"Daily Pass",0,
							"Weekly Pass",0,
							"Monthly Pass",0,
							"Yearly Pass",0,
							0	; null terminator for array
	ticketTypeArrLength	DWORD	?
	promptChooseHead	BYTE	"Please select a ticket for more options (1-",0
	promptChooseTail	BYTE	"): ",0

	msgChose			BYTE	"You chose ",0
	errorOutOfBounds	BYTE	"Invalid option!",0


.code 
main PROC
	call TicketingPage
	exit
main ENDP



TicketingPage PROC USES eax ebx edx
		; initial clear screen
		call ClrScr

	displayMenuLoop:
		; print list of options and a selection prompt
		push offset ticketTypeArr
		push offset promptChooseHead
		push offset promptChooseTail
		call WriteMenu
		mov ticketTypeArrLength, ebx
		; returned: selected index in EAX, length of passed string array in EBX
		; length of string array is needed to call GetStrArrElem



		push offset ticketTypeArr
		push ticketTypeArrLength		; length of ticketTypeArr. This is the 2nd return value of WriteMenu
		; mov eax, 4
		push eax		; selected index. This is the 1st return value of WriteMenu
		call GetStrArrElem
		; returned: offset of string in EAX

		; check for index-out-of-bounds error
		cmp eax, -1
		jne index_is_good
	
		; index is out of bounds!
		call ClrScr							; clear screen
		mov edx, offset errorOutOfBounds	; print error message
		call WriteString
		call CrLf							; print newlines (x2)
		call CrLf							
		jmp displayMenuLoop					; reprint menu and selection prompt

	index_is_good:
		mov edx, offset msgChose
		call WriteString
		; Write the string from the array
		mov edx, eax
		call WriteString
	
		ret
TicketingPage ENDP
	
end main