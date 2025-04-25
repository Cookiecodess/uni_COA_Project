.data
	emptyMSG			BYTE "Currently no ticket bought",0
	msgTimeArrival		BYTE "Train Will Arrive At : ",0
	msgCustEta			BYTE "Your ETA : ",0
	msgDist				BYTE "Total Distance: ",0
	msgKM				BYTE "KM",0
	msgRM				BYTE "RM",0
	msgHOUR				BYTE " HOUR(S) ",0
	msgMINUTE			BYTE " MINUTES",0
	msgTravelTime		BYTE "Journey Duration: ",0
	msgTrainTime		BYTE "Trains Are Available at 10AM, 2PM, and 6PM only.",0


	mytime				SYSTEMTIME <>
	arrivaltime			SYSTEMTIME <>
	durationtime		SYSTEMTIME <>
	minuteArrive		WORD ?
	distanceTraveled	DWORD ?
	indexHelper			DWORD ?
	sixty				REAL4 60.0
	result				REAL4 ?
	indexTicket			DWORD ?


.code
	show PROC
		INVOKE GetLocalTime, OFFSET mytime
		call clrscr

		; looping through the array and finding ticket that the customer bought

		mov ecx, ticketCount
		mov esi, OFFSET ticketStructArray
	
		cmp ecx, 0
		je Empty ;	---If no tickets are bought no need to loop,

		FindTickets:
		  INVOKE GetTicketMember, esi, T_CUST_NAME ; EAX = ticketStructArray[index].custname
		  INVOKE Str_compare, OFFSET cUsername, eax ; Check if ticketStructArray[index].custname == cUsername (currently logged in username)
		  jne Next
		  mov indexTicket, esi	; if match then we save the current esi as index for accesing other columns
		  call printDetails  
		Next:
		  add esi, TYPE ticketStructArray ; ESI += 20 because each TicketStruct is 20 bytes
		  loop FindTickets

		jmp Empty		; If no tickets match

		printDetails:

			;---printing NAME------
			call clrscr
			mov edx, OFFSET key1
			call writeString

			INVOKE GetTicketMember, esi, T_CUST_NAME		; eax = custName
			mov edx, eax
			call writeString

			call crlf

			;---printing ticket type------
			mov edx, OFFSET key2
			call writeString
			mov  eax,cyan+(black*16)
			call SetTextColor

			INVOKE GetTicketMember, esi, T_TICKET_TYPE		; eax = ticket Type
			mov edx, eax
			call writeString

			mov  eax,white+(black*16)
			call SetTextColor
			call crlf

			;---printing ticket price------
			mov edx, OFFSET key3
			call writeString
			mov edx, OFFSET msgRM
			call writeString
			INVOKE GetTicketMember, esi, T_PRICE		; eax = ticket price
			call writeDec
			call crlf

			;---- if the ticket type is Weekly, print weekly details
			INVOKE GetTicketMember, esi, T_TICKET_TYPE
			cmp eax, OFFSET tcktWeeklyPass
			je ShowWeeklyDetails
			;---- if the ticket type is Monthly, print monthly details
			INVOKE GetTicketMember, esi, T_TICKET_TYPE
			cmp eax, OFFSET tcktMonthlyPass
			je ShowWeeklyDetails

			mov edx, OFFSET key4
			call writeString
			INVOKE GetTicketMember, esi, T_ORIGIN_STN
			mov edx, eax
			call writeString
			call crlf

			mov edx, OFFSET key5
			call writeString
			INVOKE GetTicketMember, esi, T_DEST_STN
			mov edx, eax	
			call writeString
			call crlf

			mov edx, OFFSET msgDist
			call writeString
		

			; getting the Distance
			INVOKE GetTicketMember, esi, T_ORIGIN_STN_IDX
			push eax
			INVOKE GetTicketMember, esi, T_DEST_STN_IDX
			push eax
			call CalculateDistanceBetweenStations
			mov distanceTraveled, eax
			call writeDec
			mov edx, OFFSET msgKM
			call writeString
			call crlf


			mov edx, OFFSET msgTravelTime
			call writeString

			CALCJouneyDuration:				;time = distance/speed
				mov eax, distanceTraveled
				mov edx, 0
				div speed
				mov durationtime.wHour, ax ; save the hour first

				fild distanceTraveled    ; ST(0) = 420.0
				fild speed               ; ST(0) = 320.0, ST(1) = 420.0
				fdiv                     ; ST(0) = 1.3125 (division result)
				
				fld st(0)                ; Duplicate ST(0), so now ST(0) = 1.3125, ST(1) = 1.3125
				frndint                  ; ST(0) = 1.0 (int part), ST(1) = 1.3125 (original)
				fsubr st(0), st(1)       ; ST(0) = 0.3125 (fractional part)
				
				fmul dword ptr [sixty]   ; Multiply the fractional part by 60: 0.3125 * 60 = 18.75
										   ; Now ST(0) = 18.75
  	
				frndint						; round up
				fist minuteArrive			; store in the minuteArrive

				mov ax, minuteArrive
				mov durationtime.wMinute, ax

			WriteJourneyDuration:
				movzx eax, durationtime.wHour
				call writeDec
				mov edx, OFFSET msgHOUR
				call writeString

				movzx eax, durationtime.wMinute
				call writeDec
				mov edx, OFFSET msgMINUTE
				call writeString



			; ------writing arrival time--------
			call crlf
			call crlf
			call crlf
			
			writeTime:
				mov edx, OFFSET msgTimeArrival
				call writeString


				movzx eax, arrivaltime.wHour
				call writeDec
				mov al, ':'
				call writeChar
				movzx eax, arrivaltime.wMinute
				call writeDec
				call writeDec
				call crlf

				;---writing ETA---
				mov edx, OFFSET msgCustEta
				call writeString

				movzx eax, arrivaltime.wHour
				add ax, durationtime.wHour
				call writeDec
				mov al, ':'
				call writeChar

				movzx eax, arrivaltime.wMinute
				add ax, durationtime.wMinute
				cmp eax, 10
				jb formatMinute
				call writeDec
				pop eax ; for some reason, the stack is misaligned, the return address is one level below so we pop first
				jmp done

				formatMinute:
					mov ebx, eax
					mov eax, 0
					call writeDec
					mov eax, ebx
					call writeDec
					pop eax ; for some reason, the stack is misaligned, the return address is one level below so we pop first
					jmp done

		
		Empty:
			; --if no ticket is bought yet--
			mov  eax,lightGreen+(black*16)
			call SetTextColor
			mov edx, OFFSET emptyMSG
			call writeString
			mov  eax,white+(black*16)
			call SetTextColor
			call crlf
			jmp done

		ShowWeeklyDetails:
			call crlf
			mov  eax,black+(white*16)
			call SetTextColor
			mov edx, OFFSET msgTrainTime
			call writeString
			mov  eax,white+(black*16)
			call SetTextColor

			mov ecx, LENGTHOF locations		; ecx = 0
			lea esi, OFFSET locations
			dec ecx
			printLoc:
				call crlf
				mov ebx, ecx	; ebx = 4; ecx =4

				innerPrintLoc:
					mov edx, [esi]
					call writeString

					mov al, ' '
					call writeChar
					mov al, '<'
					call writeChar
					mov al, '-'
					call writeChar
					mov al, '>'
					call writeChar
					mov al, ' '
					call writeChar

					mov eax, ebx
					sub eax, ecx
					inc eax
					mov edx, 4
					mul dx		; eax = 4

					mov edx, [esi+eax]
					call writeString
					call crlf
					loop innerPrintLoc

				mov ecx, LENGTHOF msgTrainTime
				printBorderBottom:
					mov al, "-"
					call writeChar
					loop printBorderBottom

				add esi, 4		; esi = esi+4
				mov ecx, ebx	; ecx = 4
				loop printLoc	; ecx = 4

			jmp done	

		done:
			call crlf
			call crlf
			call waitmsg
			mov eax, -1
		
		ret
	show ENDP
