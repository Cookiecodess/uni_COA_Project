.data
	emptyMSG BYTE "Currently no ticket bought",0

.code
	show PROC
		mov eax, OFFSET boughtTicket.price ; 
		cmp eax, 0
		je Empty
		call writeString
		jmp done

		Empty:
		mov eax, OFFSET emptyMSG
		call writeString

		done:
			call waitmsg

	show ENDP
