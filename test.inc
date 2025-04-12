.data
	emptyMSG			BYTE "Currently no ticket bought",0
	msgTimeArrival		BYTE "Train Will Arrive At : ",0
	msgCustEta			BYTE "Your ETA : ",0
	msgDist				BYTE "Total Distance: ",0
	msgKM				BYTE "KM",0
	msgHOUR				BYTE " HOUR(S) ",0
	msgMINUTE			BYTE " MINUTES",0
	msgTravelTime		BYTE "Journey Duration: ",0
	msgMissedAll		BYTE "SORRY! You have missed all of the Train for today.", 0
	msgTrainTime		BYTE "Trains Are Available at 10AM, 3PM, and 6PM only.",0


	mytime				SYSTEMTIME <>
	arrivaltime			SYSTEMTIME <>
	durationtime		SYSTEMTIME <>
	minuteArrive		WORD ?
	distanceTraveled	DWORD ?
	indexHelper			DWORD ?
	sixty				REAL4 60.0
	result				REAL4 ?


.code
	show PROC
		INVOKE GetLocalTime, OFFSET mytime
		call clrscr

		mov eax, boughtTicket.custName ; 
		cmp eax, 0
		je Empty

		printDetails:
			call clrscr
			mov edx, OFFSET key1
			call writeString
			mov edx, boughtTicket.custName
			call writeString
			call crlf

			mov edx, OFFSET key2
			call writeString
			mov  eax,cyan+(black*16)
			call SetTextColor
			mov edx, boughtTicket.ticketType
			call writeString
			mov  eax,white+(black*16)
			call SetTextColor
			call crlf

			mov edx, OFFSET key3
			call writeString
			mov eax, boughtTicket.price
			call writeDec
			call crlf

			;---- if the ticket type is Weekly, print weekly details
			mov eax, boughtTicket.ticketType
			cmp eax, OFFSET tcktWeeklyPass
			je ShowWeeklyDetails
			;---- if the ticket type is Monthly, print monthly details
			mov eax, boughtTicket.ticketType
			cmp eax, OFFSET tcktMonthlyPass
			je ShowWeeklyDetails

			mov edx, OFFSET key4
			call writeString
			mov edx, boughtTicket.originStn
			call writeString
			call crlf

			mov edx, OFFSET key5
			call writeString
			mov edx, boughtTicket.destStn	
			call writeString
			call crlf

			mov edx, OFFSET msgDist
			call writeString
		

			; getting the Distance
			push boughtTicket.originStnIdx
			push boughtTicket.destStnIdx
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

			; if the current time is more than 10, we display train at 3,
			movzx eax, mytime.wHour
			cmp eax, 10
			jae secondTrain
			mov arrivaltime.wHour, 10
			mov arrivaltime.wMinute, 0
			jmp writeTime

			secondTrain:
				cmp eax, 15
				jae thirdTrain
				mov arrivaltime.wHour, 15
				mov arrivaltime.wMinute, 0
				jmp writeTime

			thirdTrain:
				cmp eax, 18
				jae noTrainLeft
				mov arrivaltime.wHour, 18
				mov arrivaltime.wMinute, 0
		
			
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
				jmp done

				formatMinute:
					mov ebx, eax
					mov eax, 0
					call writeDec
					mov eax, ebx
					call writeDec
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
		
		noTrainLeft:
			mov  eax,red+(black*16)
			call SetTextColor
			mov edx, OFFSET msgMissedAll
			call writeString
			mov  eax,white+(black*16)
			call SetTextColor
			jmp done

			

		done:
			call crlf
			call crlf
			call waitmsg
			mov eax, -1

		ret
	show ENDP
