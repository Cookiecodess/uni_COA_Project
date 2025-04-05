; INCLUDE generalFunctions.inc

SINGLE_JOURNEY EQU 0	; Single journey
WEEKLY_PASS	EQU 1	; Weekly pass
MONTHLY_PASS EQU 2	; Monthly pass

TicketStruct STRUCT
	custPtr DWORD ?
	ticketType BYTE ?
TicketStruct ENDS

.data
	; SPACE = 32
	;GENERAL------------------------------------------------------------------
	; MAX	= 20								; max characters to read
	; inputBuffer			byte  MAX+1 dup(?)  ; room for null character

	;TICKETING--------------------------------------------------------------------
	headerBookTicket	BYTE	"Book ticket",0
	ticketTypeArr		BYTE	"Single Journey",0,
								"Weekly Pass",0,
								"Monthly Pass",0,
								"Back",0,
								0	; null terminator for array
	backIndex			DWORD   3		; note that this is an index. Index 3 = 4th element
	ticketTypeArrLength	DWORD	?
	; promptChooseHead	BYTE	"Please select a ticket for more options (1-",0")"
	; promptChooseTail	BYTE	"): ",0
	promptChoose	BYTE	"Please select a ticket for more options.",0

	msgChose			BYTE	"You chose ",0
	; selectionIndex		DWORD	?
	



.code 
TicketingPage PROC USES eax ebx edx
		; Ticket type menu
		invoke InitMenu, offset headerBookTicket, offset ticketTypeArr, offset promptChoose, 0, 0

		; Did user select "Back"? If so, let's not print anything and just return.
		mov eax, currentSelection	; Note: currentSelection is a global var defined in coolMenu.inc
		cmp eax, backIndex
		je go_back_whence_you_came

		; Actions for each selection
		cmp eax, SINGLE_JOURNEY		; Single journey ticket chosen
		;je 		
		cmp eax, WEEKLY_PASS		; Weekly pass selected
		;je pass
		cmp eax, MONTHLY_PASS		; Weekly pass selected
		;je 


		; mov edx, offset msgChose
		; call WriteString
		;Write the string from the array
		; mov edx, offset ticketTypeArr
		; mov eax, currentSelection
		; call GetStrArrElem
		; call WriteString
		
		; exit	; TEMP. TO-DO: Implement actions for each selection.
	
	go_back_whence_you_came:
		ret
TicketingPage ENDP