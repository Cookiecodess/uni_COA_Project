include irvine32.inc
INCLUDE generalFunctions.inc
INCLUDE coolMenu.inc
INCLUDE TicketingPage.inc
	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	TAB = 09h
	SPACE = 20h

.data

		receipt byte "Receipt",0
		
		person	byte ?
		personS	byte "Name : ",0
		total	byte ?
		totalS	byte "Total : ",0
		price	byte ?
		priceS	byte "Price : ",0
		detail	byte ?
		detailS byte "Details : ",0
		bookingS	byte "Your booking number is : ",0
		bookingnum	dword ?

    headerReceipt		byte "Receipt",0

.code

main proc
	call GenerateReceipt
exit
main endp


GenerateReceipt proc

	receiptStart:
		mov eax, offset headerReceipt
		mov ebx, lengthof headerReceipt
		call PrintHeader

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

		mov al, TAB
		call WriteChar
		lea edx,personS
		call WriteString
		call CRLF

		mov al, TAB
		call WriteChar
		lea edx,priceS
		call WriteString
		call CRLF

		mov al, TAB
		call WriteChar
		lea edx,detailS
		call WriteString
		



		call CRLF
		mov  eax,1000 ;delay 1 sec
		call Delay


		ret

GenerateReceipt endp

end main










