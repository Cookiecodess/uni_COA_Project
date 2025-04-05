; INCLUDE generalFunctions.inc



.data
	; SPACE = 32
	;GENERAL------------------------------------------------------------------
	; MAX	= 20								; max characters to read
	; inputBuffer			byte  MAX+1 dup(?)  ; room for null character

	;TICKETING--------------------------------------------------------------------
	headerBookTicket	BYTE	"Book ticket",0

	
	ticketTypeOptions		DWORD	OFFSET tcktSingleJourney, OFFSET tcktWeeklyPass, OFFSET tcktMonthlyPass, OFFSET backOption

	backIndex			DWORD   3		; note that this is an index. Index 3 = 4th element
	ticketTypeOptionsCount	DWORD	?
	; promptChooseHead	BYTE	"Please select a ticket for more options (1-",0")"
	; promptChooseTail	BYTE	"): ",0
	promptChoose	BYTE	"Please select a ticket for more options.",0

	msgChose			BYTE	"You chose ",0
	; selectionIndex		DWORD	?
	
	boughtTicket TicketStruct <>


.code 
TicketingPage PROC USES eax ebx edx
		; Ticket type menu
		invoke InitMenu, offset headerBookTicket, offset ticketTypeOptions, lengthof ticketTypeOptions, offset promptChoose, 0, 0

		; Did user select "Back"? If so, let's not print anything and just return.
		; mov eax, currentSelection	; Note: currentSelection is a global var defined in coolMenu.inc
		; cmp eax, backIndex
		; je go_back_whence_you_came

		; Actions for each selection
		cmp eax, SINGLE_JOURNEY		; Single journey ticket chosen
		je SelectSingleJourney
		cmp eax, WEEKLY_PASS		; Weekly pass selected
		je SelectWeeklyPass
		cmp eax, MONTHLY_PASS		; Weekly pass selected
		je SelectMonthlyPass

		; Selected Back
		jmp go_back_whence_you_came

	SelectSingleJourney:
		mov boughtTicket.ticketType, SINGLE_JOURNEY
		jmp Continue
	SelectWeeklyPass:
		mov boughtTicket.ticketType, WEEKLY_PASS
		jmp Continue
	SelectMonthlyPass:
		mov boughtTicket.ticketType, MONTHLY_PASS
		jmp Continue
	
	Continue:
		mov boughtTicket.custName, OFFSET cUsername 

		; mov edx, offset msgChose
		; call WriteString
		;Write the string from the array
		; mov edx, offset ticketTypeOptions
		; mov eax, currentSelection
		; call GetStrArrElem
		; call WriteString
		
		; exit	; TEMP. TO-DO: Implement actions for each selection.
	
	go_back_whence_you_came:
		ret
TicketingPage ENDP