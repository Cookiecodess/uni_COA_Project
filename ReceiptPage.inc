
	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	TAB = 09h
	SPACE = 20h

.data


		receipt byte "=================== Receipt ===================",0
		emptyLine BYTE 13, 10
		waitSec byte "Please wait for 5 seconds...",0
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
			confPrintOptions	DWORD	OFFSET optionSaveR, OFFSET optionCancelS


    headerReceipt		byte "Receipt",0

	;output receipt to file
		receiptFile byte "OutputReceipt.txt",0
		filehandle dword ?	;save the line

	errorNoData			byte	"No ticket data available.",0

.code


GenerateReceipt proc
	

	receiptStart:
		
	call Clrscr
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
		mov  eax,green+(black*16)
			 call SetTextColor
		call WriteDec
		mov  eax,lightGray+(black*16)
			 call SetTextColor
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
		mov eax,boughtTicket.price
		call WriteDec
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
		
		call waitmsg

		INVOKE InitMenu,0,offset confPrintOptions,lengthof confPrintOptions,offset askMsg,0,0
		call CRLF
		
			cmp eax, 0		
			je saveR

			cmp eax,1
			je return

			saveR:
				call saveReceipt
				lea edx,waitSec
				call writeString
				mov  eax,5000 ;delay 5 sec
				call Delay
				jmp return


		

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
		;mov edx,offset priceS
		;mov ecx, lengthOf priceS- 1
		;mov eax, filehandle      ; Load file handle
		;call WriteToFile

		;mov edx,boughtTicket.custName
		;call strLength
		;mov ecx, eax
		;mov eax, filehandle      ; Load file handle
		;call WriteToFile
		;call newline


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
		;call ExitProcess
		jmp return
	error_exit:
    call WriteString         ; Print error message
    ;call ExitProcess
	jmp return

	return:
	ret
saveReceipt endp

newLine proc
		mov edx,offset emptyLine
		mov ecx, 2
		mov eax, filehandle  
		call WriteToFile
		ret
newLine endp

