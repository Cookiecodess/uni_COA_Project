INCLUDE Irvine32.inc
	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	SPACE =09h
.data
	header byte	"======================================",0
	;USER LOGIN----------------------------------------------------------------
		welcome byte " Welcome,here is a login page",0
		choose byte "Choose a number to login",0
		user1 byte "1. Customer",0
		user2 byte "2. Admin",0

	;RECEIPT--------------------------------
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
	;REPORT--------------------------------------------------------------------
		totalprice byte ?
		totalticketsold byte ?

		report byte "Report",0

.code
main PROC

	;USER LOGIN------------------------------
		;LOGIN:
		lea edx,header

		call WriteString
		lea edx,welcome
		call WriteString
		lea edx,header
		call WriteString
		call CRLF
	
		lea edx,choose
		call WriteString
		call CRLF

		mov al, SPACE   ; Load tab character
		call WriteChar  ; Print tab
		lea edx,user1
		call WriteString
		call CRLF

		mov al, SPACE   ; Load tab character
		call WriteChar  ; Print tab
		lea edx,user2
		call WriteString
		call CRLF

		call WaitMsg

		call Clrscr

		;JMP LOGIN
	;RECEIPT--------------------------------
		lea edx,header
		call WriteString
		lea edx,receipt
		call WriteString
		lea edx,header
		call WriteString
		call CRLF
		
		lea edx,bookingS
		call WriteString
		mov  eax,yellow+(blue*16)
      call SetTextColor
		;call Random32
		call Randomize
		mov eax, 90000000			;since rand is from 0 to n-1 so it will generate 0 - 89999999
		call RandomRange		; EAX = random number in range 0-89999999
		add eax, 10000000			; adjust to range 10000000-99999999
		mov bookingnum,eax

		call WriteDec
		call CRLF

		



		call CRLF
		mov  eax,1000 ;delay 1 sec
		call Delay

	;REPORT---------------------------------
		;ticket price * ticket sold
		
		lea edx,header
		call WriteString
		call WriteString
		call CRLF				; new line

		mov al, SPACE   ; Load tab character
		call WriteChar  ; Print tab
		call WriteChar  ; Print tab
		call WriteChar  ; Print tab
		lea edx,report
		call WriteString
		call CRLF

		lea edx,header
		call WriteString
		call WriteString



	exit
main ENDP
END main