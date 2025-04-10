.data
	emptyMSG BYTE "Currently no ticket bought",0

.code
	show PROC
		call clrscr

		mov eax, boughtTicket.custName ; 
		cmp eax, 0
		je Empty

		printDetails:
			mov edx, OFFSET key1
			call writeString
			mov edx, boughtTicket.custName
			call writeString
			call crlf

			mov edx, OFFSET key2
			call writeString
			mov edx, boughtTicket.ticketType
			call writeString
			call crlf

		;	mov edx, OFFSET key3
		;	call writeString
		;	mov edx, boughtTicket.price
		;	call writeString
		;	call crlf

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

			jmp done


		Empty:
		; --if no ticket is bought yet--
		mov edx, OFFSET emptyMSG
		call writeString
		call crlf

		done:
			call waitmsg
			mov eax, -1

		ret
	show ENDP
