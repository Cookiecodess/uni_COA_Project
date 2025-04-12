
	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	TAB = 09h
	SPACE = 20h

.data


		receipt byte "=================== Receipt ===================",0
		emptyLine BYTE 13, 10

		personS	byte "Name : ",0

		priceS	byte "Price : ",0
		detailS byte "Details : ",0
		bookingS	byte "Your booking number is : ",0
		bookingnum	dword ?
		ticketTypeWord byte "Ticket Type : ",0
		toWord byte " to ",0

		askMsg byte "Do you want to save your receipt",0
			optionSaveR	BYTE	"YES",0
			optionCancelS	BYTE	"No and Quit",0
			confPrintOptions	DWORD	OFFSET optionSaveR, OFFSET optionSaveR


    headerReceipt		byte "Receipt",0

	;output receipt to file
		receiptFile byte "OutputReceipt.txt",0
		filehandle dword ?	;save the line

	errorNoData			byte	"No ticket data available.",0

.code


GenerateReceipt proc
	

	receiptStart:
		

		;mov edx, boughtTicket.custName
			;call WriteString
		mov eax, offset headerReceipt
		mov ebx, lengthof headerReceipt
		call PrintHeader

		; If boughtTicket.custName = 0 (points to null), then skip the printing of all this
		cmp boughtTicket.custName, 0
		jne continuePrint
		mov edx, offset errorNoData
		call WriteString
		call CrLf
		call CrLf
		call WaitMsg
		jmp return
		
	continuePrint:

		mov al, TAB 
		call WriteChar 

		lea edx,bookingS
		call WriteString


		;call Random32
		call Randomize
		mov eax, 90000000			;since rand is from 0 to n-1 so it will generate 0 - 89999999
		call RandomRange		; EAX = random number in range 0-89999999
		add eax, 10000000			; adjust to range 10000000-99999999
		mov bookingnum,eax
		call WriteDec
		call CRLF

		
		;Name
		mov al, TAB
		call WriteChar
		lea edx,personS
		call WriteString
		mov edx,boughtTicket.custName
		call WriteString
		call CRLF

		;Price
		mov al, TAB
		call WriteChar
		lea edx,priceS
		call WriteString
		;mov edx,boughtTicket.price
		call WriteString
		call CRLF

		;TicketType
		mov al, TAB
		call WriteChar
		lea edx,ticketTypeWord
		call WriteString
		mov edx,boughtTicket.ticketType
		call WriteString
		call CRLF


		;from to destination
		mov al, TAB
		call WriteChar
		lea edx,detailS
		call WriteString
		mov edx,boughtTicket.originStn
		call WriteString
		lea edx,toWord
		call WriteString
		mov edx,boughtTicket.destStn
		call WriteString
		call CRLF


		;INVOKE InitMenu, 0, offset confPrintOptions, lengthof offset confPrintOptions, offset askMsg, 0, 0

		call CRLF
		mov  eax,1000 ;delay 1 sec
		call Delay

		call saveReceipt

	return:
		ret

GenerateReceipt endp

saveReceipt proc

	;Create file
		mov edx, offset receiptFile
		call CreateOutputFile      ; Irvine32 function to create file
		mov filehandle, eax 

    ; Check if file was created successfully
		cmp eax, INVALID_HANDLE_VALUE
		je error_exit

	;Write to file 
		mov edx,offset receipt
		mov ecx, lengthOf receipt- 1
		mov eax, filehandle      ; Load file handle
		call WriteToFile
		
		call newline

		;name
		mov edx,offset personS
		mov ecx, lengthOf personS- 1
		mov eax, filehandle      ; Load file handle
		call WriteToFile

		mov edx,boughtTicket.custName
		call strLength
		mov ecx, eax
		mov eax, filehandle      ; Load file handle
		call WriteToFile
		call newline

		;Price
		mov edx,offset priceS
		mov ecx, lengthOf priceS- 1
		mov eax, filehandle      ; Load file handle
		call WriteToFile

		mov edx,boughtTicket.custName
		call strLength
		mov ecx, eax
		mov eax, filehandle      ; Load file handle
		call WriteToFile
		call newline


		;Ticket Type
		mov edx,offset ticketTypeWord
		mov ecx, lengthOf ticketTypeWord- 1
		mov eax, filehandle      ; Load file handle
		call WriteToFile

		mov edx,boughtTicket.ticketType
		call strLength
		mov ecx, eax
		mov eax, filehandle      ; Load file handle
		call WriteToFile
		call newline


		;Detail
		mov edx,offset detailS
		mov ecx, lengthOf detailS- 1
		mov eax, filehandle      ; Load file handle
		call WriteToFile

		mov edx,boughtTicket.originStn
		call strLength
		mov ecx, eax
		mov eax, filehandle      ; Load file handle
		call WriteToFile

		mov edx,offset toWord
		mov ecx, lengthOf toWord-1
		mov eax, filehandle      ; Load file handle
		call WriteToFile

		mov edx,boughtTicket.destStn
		call strLength
		mov ecx, eax
		mov eax, filehandle      ; Load file handle
		call WriteToFile



		call newline




	; Close file
		mov eax, filehandle   
		call CloseFile  
		call ExitProcess

	error_exit:
    call WriteString         ; Print error message
    call ExitProcess



	ret
saveReceipt endp

newLine proc
		mov edx,offset emptyLine
		mov ecx, 2
		mov eax, filehandle  
		call WriteToFile
		ret
newLine endp

