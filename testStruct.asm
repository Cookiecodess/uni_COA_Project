INCLUDE Irvine32.inc


.data
TicketStruct STRUCT
	custName		DWORD	?	; points to a customer username string
	ticketType		DWORD	?	; points to a ticket type string
	price			DWORD	?
	originStn		DWORD	?	; points to a location string (only applicable for single-journey)
	destStn			DWORD	?	; points to a location string (only applicable for single-journey)
	originStnIdx	DWORD	?
	destStnIdx		DWORD	?
	timeSlotIdx		DWORD	?
TicketStruct ENDS

; TICKET_STRUCT_SIZE = 32

ticketStructArray	TicketStruct	20 dup(<>)
ticketStructCount	DWORD			0

sampleName			BYTE	"Cookie",0
sampleTicketType	BYTE	"Single Journey",0
samplePrice			DWORD	100


SaveTicket PROTO :DWORD
LoadTicket PROTO

.code

main PROC

mov esi, offset TicketStruct
; [base + (index * scale) + offset]    where scale must be 1,2,4 or 8 due to hardware limitations.
mov dword ptr [esi + (eax * 8) + TicketStruct.custName], 1				; ticketStructArray[0].custName = "Cookie";
mov dword ptr [esi + (0 * TICKET_STRUCT_SIZE) + TicketStruct.ticketType], offset sampleTicketType		; ticketStructArray[0].ticketType = "Single Journey";
mov dword ptr [esi + (0 * TICKET_STRUCT_SIZE) + TicketStruct.price], eax						; ticketStructArray[0].price = 100;
inc ticketStructCount



main ENDP

; accepts:  pointer to source struct
; returns: EDI - pointer to saved struct in array
SaveTicket PROC sourceStructOffset:DWORD
	mov esi, sourceStructOffset
	; ecx = 0
	; for (sizeof ticketStruct / 4) times:
	;		
	ret
SaveTicket ENDP

; accepts: ECX - index of struct array
; returns: ESI - pointer to obtained struct from array
LoadTicket PROC
LoadTicket ENDP

end main