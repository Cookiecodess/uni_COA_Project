include irvine32.inc

.data

	; ASSOCIATIVE ARRAY FOR BOUGHT TICKET DATA
	key1				BYTE	"Customer name",0
	key2				BYTE	"Ticket type",0
	key3				BYTE	"Price",0
	key4				BYTE	"Origin station",0
	key5				BYTE	"Destination station",0
	TicketStructKeys	DWORD	OFFSET key1, OFFSET key2, OFFSET key3, OFFSET key4, OFFSET key5 


	; TICKET STRUCT DEFINITION - Stores information of a single ticket bought by a customer

	TicketStruct STRUCT
		custName		DWORD	?	; points to a customer username string
		ticketType		DWORD	?	; points to a ticket type string
		price			DWORD	?
		originStn		DWORD	?	; points to a location string (only applicable for single-journey)
		destStn			DWORD	?	; points to a location string (only applicable for single-journey)
		originStnIdx	DWORD	?
		destStnIdx		DWORD	?
	TicketStruct ENDS

	; --------- dummy data -----------
	namess			BYTE "Alex",0
	ticketType		BYTE "Single Journey", 0
	price			DWORD 900
	ori				BYTE "Melaka", 0
	des				BYTE "Kuala Lumpur",0

	boughtTicket TicketStruct <>


	emptyMSG BYTE "Currently no ticket bought",0
	msgTime BYTE "Current Time : ",0
	msgTimeArrival BYTE "Train Arrival Time : ",0
	

	mytime SYSTEMTIME <>
	arrivaltime SYSTEMTIME <>
	minuteArrive DWORD 2

.code
main proc
	mov eax, OFFSET namess
    mov boughtTicket.custName, eax
    mov eax, OFFSET ticketType
    mov boughtTicket.ticketType, eax
    mov eax, price
    mov boughtTicket.price, eax
    mov eax, OFFSET ori
    mov boughtTicket.originStn, eax
    mov eax, OFFSET des
    mov boughtTicket.destStn, eax
	mov boughtTicket.originStnIdx, 3
	mov boughtTicket.destStnIdx, 3

	
	INVOKE GetLocalTime, OFFSET mytime

    call showSchedule

	exit
main endp


showSchedule PROC
	mov eax, boughtTicket.custName ; 
		cmp eax, 0
		je Empty

		printDetails:
			call clrscr
			mov edx, OFFSET key1
			call writeString
			mov al, ':'
			call writeChar
			mov edx, boughtTicket.custName
			call writeString
			call crlf

			mov edx, OFFSET key2
			call writeString
			mov al, ':'
			call writeChar
			mov edx, boughtTicket.ticketType
			call writeString
			call crlf

			mov edx, OFFSET key3
			call writeString
			mov al, ':'
			call writeChar
			mov eax, boughtTicket.price
			call writeDec
			call crlf

			mov edx, OFFSET key4
			call writeString
			mov al, ':'
			call writeChar
			mov edx, boughtTicket.originStn
			call writeString
			call crlf

			mov edx, OFFSET key5
			call writeString
			mov al, ':'
			call writeChar
			mov edx, boughtTicket.destStn	
			call writeString
			call crlf


			; ------writing arrival time assuming will arrive in 2 minutes--------
			call crlf
			mov edx, OFFSET msgTimeArrival
			call writeString

			; if the current time is more than 10, we display train at 3,
			movzx eax, mytime.wHour
			cmp eax, 10
			ja secondTrain
			mov arrivaltime.wHour, 10
			mov arrivaltime.wMinute, 0
			jmp writeTime

			secondTrain:
				cmp eax, 3
				ja thirdTrain
				mov arrivaltime.wHour, 15
				mov arrivaltime.wMinute, 0
				jmp writeTime

			thirdTrain:
				mov arrivaltime.wHour, 18
				mov arrivaltime.wMinute, 0
		
			
			writeTime:
				movzx eax, arrivaltime.wHour
				call writeDec
				mov al, ':'
				call writeChar
				movzx eax, arrivaltime.wMinute
				call writeDec
				call writeDec


				

			jmp done


		Empty:
		; --if no ticket is bought yet--
		mov edx, OFFSET emptyMSG
		call writeString
		call crlf

		done:
			call crlf
			call crlf
			call waitmsg
	ret
showSchedule ENDP

end main