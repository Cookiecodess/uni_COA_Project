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

; Enums for accessing members of a TicketStruct struct.
T_CUST_NAME			= 0
T_TICKET_TYPE		= 1
T_PRICE				= 2
T_ORIGIN_STN		= 3
T_DEST_STN			= 4
T_ORIGIN_STN_IDX	= 5
T_DEST_STN_IDX		= 6
T_TIME_SLOT_IDX		= 7


; TICKET_STRUCT_SIZE = 32


ticketStructArray	TicketStruct	20 dup(<>)
ticketCount	DWORD			0

sampleName			BYTE	"Cookie",0
sampleTicketType	BYTE	"Single Journey",0
samplePrice			DWORD	637
sampleLoc			BYTE	"Land of Ooo",0
sampleLoc2			BYTE	"Land of Eee",0

sampleTicket		TicketStruct <OFFSET sampleName, OFFSET sampleTicketType, 637, OFFSET sampleLoc, OFFSET sampleLoc2, 0, 3, 1>

SaveTicket PROTO :PTR TicketStruct
GetTicketPointer PROTO :DWORD
GetTicketMember PROTO :PTR TicketStruct, :DWORD

.code

main PROC
	INVOKE SaveTicket, ADDR sampleTicket

	; INVOKE GetTicketPointer, 0 ; EAX = pointer to ticketStructArray[0]

	INVOKE GetTicketMember, eax, T_ORIGIN_STN
	mov edx, eax
	call WriteString

	exit



main ENDP

;------------------------------------------------
; SaveTicket: Stores all members' data from given source TicketStruct to ticketStructArray as a new element.
; Receives: pointer to source struct
; Uses: ticketCount (DWORD) in globals
; Returns: EAX - pointer to saved struct in ticketStructArray
;------------------------------------------------
SaveTicket PROC USES esi ebx ecx edx, pSourceStruct:PTR TicketStruct
	LOCAL pTargetStruct:DWORD

	; ESI = address of source TicketStruct
	mov esi, pSourceStruct

	; EAX = Array index = ticketCount * sizeof(ticketStructArray)
	mov eax, ticketCount
	mov ebx, TYPE ticketStructArray
	mul ebx

	; EDI = address of ticketStructArray[index]
	lea edi, [ticketStructArray + eax]
	mov pTargetStruct, edi

	; Copy each member's data from source TicketStruct to target TicketStruct (in ticketStructArray)
	mov ecx, SIZEOF TicketStruct / 4
CopyMemberData:
	mov eax, dword ptr [esi]
	mov dword ptr [edi], eax
	add esi, 4
	add edi, 4
	loop CopyMemberData
	
	; Increment ticket count
	inc ticketCount

	; Return pointer to saved struct in the array
	mov eax, pTargetStruct
	ret
SaveTicket ENDP

;------------------------------------------------
; GetTicketPointer: Get a pointer to the nth TicketStruct in ticketStructArray, where n is a number
; Receives: n (index of TicketStruct in the array)
; Returns: EAX - pointer to target struct in array
;------------------------------------------------
GetTicketPointer PROC USES ebx ecx edx, index:DWORD
	; EAX = index of ticket * sizeof(ticketStructArray)
	mov eax, index
	mov ebx, TYPE ticketStructArray
	mul ebx

	lea eax, [ticketStructArray + eax]
	ret
GetTicketPointer ENDP


;------------------------------------------------
; GetTicketMember: Gets the value of a specified member in a TicketStruct
; Receives: 
;		structOffset (PTR TicketStruct) - A pointer to a TicketStruct
;		memberOffset (DWORD) - A number, which is the offset of a member in the TicketStruct
; Returns: EAX - The value of the specified member
;------------------------------------------------
GetTicketMember PROC USES esi ebx edx, structOffset:PTR TicketStruct, memberOffset:DWORD
	mov esi, structOffset

	; EAX = memberOffset * 4
	mov ebx, memberOffset
	mov eax, 4
	mul ebx

	; ESI = structOffset + (memberOffset * 4)
	add esi, eax
	
	mov eax, dword ptr [esi]
	ret
GetTicketMember ENDP

end main