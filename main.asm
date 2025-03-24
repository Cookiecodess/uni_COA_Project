INCLUDE Irvine32.inc
	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	SPACE =09h
.data
	header byte	"======================================",0
	;USER LOGIN----------------------------------------------------------------
		welcome byte " Welcome,here is a login page",0
		choose byte "Choose a number to login : ",0
		user1 byte "1. Customer",0
		user2 byte "2. Admin",0
		loginChoose byte ?

		;username byte "abcdAAA",0
		;password byte "password123",0

		loginMsg BYTE "Enter Username: ", 0
		passMsg BYTE "Enter Password: ", 0

		successMsg BYTE "Login Successful!", 0
		failMsg BYTE "Login Failed!", 0

		
		username BYTE 20 DUP(0)	;like char[20] and initialize it by 0(null)
		password BYTE 20 DUP(0)	;but user only can type 19 word since at 20 need to store the 0(to stop)
	;ADMIN LOGIN
	admin byte "ADMIN",0


	;CUSOTMER LOGIN
	customer byte "CUSTOMER",0


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

		;REPORT--------------------------------------------------------------------

.code
main PROC

	;USER LOGIN------------------------------
		LOGIN:
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

		mov al, SPACE   ; Load tab character
		call WriteChar  ; Print tab
		lea edx,choose
		call WriteString


		call ReadChar
		mov loginChoose, al
		
		cmp loginChoose, '1'	; If user chose Admin
		je 	customerLogin		;je=jump if equal to
		cmp loginChoose, '2'	; If user chose Customer
		je adminLogin

		adminLogin:
			call CRLF
			lea edx,header
			call WriteString
			lea edx,admin
			call WriteString
			lea edx,header
			call WriteString
			call CRLF

			lea edx,loginMsg
			call WriteString
			mov ecx,20	;to ensure the user only input 20 char?
			call ReadString
			call CRLF
			lea edx,username


			lea edx,passMsg
			call WriteString

			mov ecx,20	;to ensure the user only input 20 char
			call ReadString
			call CRLF
			lea edx,password


			jmp exitLogin 
		customerLogin:
				call CRLF
				lea edx,header
				call WriteString
				lea edx,customer
				call WriteString
				lea edx,header
				call WriteString
				call CRLF
				jmp exitLogin 
		
		exitLogin:;


		call WaitMsg

		call Clrscr

		;JMP LOGIN
	;RECEIPT--------------------------------
		;Receipt:
		lea edx,header
		call WriteString
		lea edx,receipt
		call WriteString
		lea edx,header
		call WriteString
		call CRLF

		mov al, SPACE 
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

		mov al, SPACE
		call WriteChar
		lea edx,personS
		call WriteString
		call CRLF

		mov al, SPACE
		call WriteChar
		lea edx,priceS
		call WriteString
		call CRLF

		mov al, SPACE
		call WriteChar
		lea edx,detailS
		call WriteString
		



		call CRLF
		mov  eax,1000 ;delay 1 sec
		call Delay

	;REPORT---------------------------------
		;Report:
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


		;call WaitMsg
		;call Clrscr
		;call LOGIN
	exit
main ENDP
END main