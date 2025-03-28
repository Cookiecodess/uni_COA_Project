INCLUDE Irvine32.inc
INCLUDE generalFunctions.inc
INCLUDE TicketingPage.inc

	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	SPACE =09h
.data
	;STATUS FLAGS--------------------------------------------------------------
	showLoginSuccessMsg	byte 0
	;HEADERS-------------------------------------------------------------------
	header1             byte "Main Menu",0
    headerLogin			byte "Login",0
    headerReceipt		byte "Receipt",0
    header3             byte "Register",0
	; NOTE: To modify the border pattern and left-and-right padding,
	;       go to the .DATA segment in generalFunctions.asm
	;       and look for "FOR PrintHeader".

	;GENERAL------------------------------------------------------------------
	; MAX	= 20								; max characters to read
	; inputBuffer			byte  MAX+1 dup(?)  ; room for null character


	;USER LOGIN----------------------------------------------------------------
	welcome			byte	" Welcome,here is a login page",0
	choose			byte	"Choose a number to login : ",0
	user			byte	"Customer",0,"Admin",0,"Back",0,0
	loginChoose		byte	?

	cUsername		byte	"aaa",0
	cPassword		byte	"123",0

	aUsername		byte	"bbb",0
	aPassword		byte	"zzz",0


	loginMsg		byte	"Enter Username: ", 0
	passMsg			byte	"Enter Password: ", 0

	successMsg		byte	"Login Successful!", 0
	failMsg			byte	"Login Failed!", 0

		
	inputUsername	byte	MAX+1 DUP(?)	;like char[20] and initialize it by 0(null)
	inputPassword	byte	MAX+1 DUP(?)	;but user only can type 19 word since at 20 need to store the 0(to stop)
	
	;ADMIN LOGIN
	admin byte "ADMIN",0
	adminSelection byte "View Today Earning Report",0,"View Month Earning Report",0,"View Year Earning Report",0,"log out",0,0

	;CUSOTMER LOGIN
	headerCustomer byte "CUSTOMER",0
	customerSelectionArr byte "View Schedule",0,"Ticketing",0,"Buy Member",0,"Check Nearby Station",0,"log out",0,0
	customerSelectionArrLength dword ?
	promptCustomerPageHead byte "Please select an action (1-",0
	promptCustomerPageTail byte "): ",0

	;REGISTER
	register byte "REGISTER",0
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
		mov eax, offset headerLogin
		mov ebx, lengthof headerLogin
		call PrintHeader

		lea edx,choose
		call WriteString
		call CRLF

		push OFFSET user
		call WriteStrArr

		mov al, SPACE   ; Load tab character
		call WriteChar  ; Print tab
		lea edx,choose
		call WriteString


		call ReadChar
		mov loginChoose, al
				call CRLF
		cmp loginChoose, '1'	; If user chose Admin
		je 	customerLogin		;je=jump if equal to
		cmp loginChoose, '2'	; If user chose Customer
		je adminLogin

		adminLogin:		;=======================================================
		mov eax, offset admin
		mov ebx, lengthof admin
		call PrintHeader

		lea edx,loginMsg
		call WriteString
		mov edx, OFFSET inputUsername  ; use username as buffer
		mov ecx, MAX	;to ensure the user only input 20 char
		call ReadString
		call CRLF
		

		lea edx,passMsg
		call WriteString
		mov edx, OFFSET inputPassword
		mov ecx, 20	;to ensure the user only input 20 char
		call ReadString
		call CRLF
		; lea edx,password


		jmp check_admin 

		check_admin:	;============================================================
			push OFFSET inputUsername
			push OFFSET aUsername
			call Str_compare	;to check/compare the username is true or false
			jne Alogin_failed

			push OFFSET inputPassword
			push OFFSET aPassword
			call Str_compare
			jne Alogin_failed

			jmp adminPage
		
		
		
		customerLogin:		;===================================
			mov eax, offset headerCustomer
			mov ebx, lengthof headerCustomer
			call PrintHeader
			call CRLF
			
			lea edx,loginMsg
			call WriteString

			mov edx, offset inputUsername
			mov ecx, MAX	;to ensure the user only input 20 char?
			call ReadString
			call CRLF
			; lea edx,username

			lea edx,passMsg
			call WriteString

			mov edx, offset inputPassword
			mov ecx, MAX	;to ensure the user only input 20 char
			call ReadString
			call CRLF
			; lea edx,password
			
			jmp check_customer


		check_customer:		;====================================
			push OFFSET inputUsername
			push OFFSET cUsername
			call Str_compare			
			jne Clogin_failed

			push OFFSET inputPassword
			push OFFSET cPassword
			call Str_compare		
			jne Clogin_failed

			; at this point, customer is successfully authenticated
			mov showLoginSuccessMsg, 1
			jmp customerPage


		call WaitMsg

		call Clrscr

		;JMP LOGIN


	Alogin_failed:		;==============================

		mov edx, OFFSET failMsg
		call WriteString
		call Crlf

		; clear user-input username and password by using rep stosb by fill it by 0
		mov edi, OFFSET inputUsername
		mov ecx, MAX
		mov al, 0
		rep stosb		; Fill buffer (inputUsername) with MAX number of zeroes
						; Note: stosb = store string byte
						;		rep = repeat for ECX times

		mov edi, OFFSET inputPassword
		mov ecx, MAX
		mov al, 0
		rep stosb		; Fill buffer (inputPassword) with MAX number of zeroes


		jmp adminLogin   ; Retry Admin login

	Clogin_failed:		;==============================
		mov edx, OFFSET failMsg
		call WriteString
		call Crlf
		jmp customerLogin  ; Retry Customer login

		; clear user-input username, password
		mov edi, OFFSET inputUsername
		mov ecx, 20
		mov al, 0
		rep stosb

		mov edi, OFFSET inputPassword
		mov ecx, 20
		mov al, 0
		rep stosb

	;CUSTOMER PAGE--------------------------
	customerPage:
		call ClrScr						; Clear screen	

		mov eax, offset headerCustomer		; Print header
		mov ebx, lengthof headerCustomer
		call PrintHeader

		cmp showLoginSuccessMsg, 0
		je displayMenuLoop				; If the showLoginSuccessMsg flag is set to 0, 
										; skip the code that prints login success msg.

		mov edx, offset successMsg		; Print login success message
		call WriteString
		mov showLoginSuccessMsg, 0		; Set the showLoginSuccessMsg flag is set to 0,
										; so it won't show up the subsequent times this page is redrawn.

		; push OFFSET customerSelection	; Print list of options
		; call WriteStrArr

		; mov al, SPACE   ; Load tab character
		; call WriteChar  ; Print tab
		; lea edx,choose
		; call WriteString
		; call ReadChar
		; mov loginChoose, al
				; call CRLF
		; exit

		displayMenuLoop:
			call CRLF						; newline

			; print list of options and a selection prompt
			push offset customerSelectionArr
			push offset promptCustomerPageHead
			push offset promptCustomerPageTail
			call WriteMenu
			mov customerSelectionArrLength, ebx
			; returned: selected index in EAX, length of passed string array in EBX
			;           if selected number is out-of-bounds, EAX = -1
			; length of string array is needed to call GetStrArrElem

			; push offset customerSelectionArr
			; push customerSelectionArrLength		; length of ticketTypeArr. This is the 2nd return value of WriteMenu
			; push eax		; selected index. This is the 1st return value of WriteMenu
			; call GetStrArrElem
			; returned: offset of string in EAX

			; Check for index-out-of-bounds error
			cmp eax, -1
			jne index_is_good
	
			; index is out of bounds!
			call ClrScr							; Clear screen

			lea eax, headerCustomer				; Print header again
			mov ebx, lengthof headerCustomer
			call PrintHeader

			mov edx, offset errorOutOfBounds	; Print error message!
			call WriteError
			call CrLf							; print newlines (x2)
			call CrLf							
			jmp displayMenuLoop					; reprint menu and selection prompt

		index_is_good:
			; At this point, EAX = selected index
			cmp eax, 1		; selection: Ticketing
			je Ticketing
			
			jmp customerPage		; TEMP: redraw customerPage if user selects an option that hasn't been implemented

		Ticketing:
			call TicketingPage
			jmp backToCustomerPage

		backToCustomerPage:
			jmp customerPage

	adminPage:
		mov eax, offset admin
		mov ebx, lengthof admin
		call PrintHeader
		call CRLF
		mov edx, offset successMsg
		call WriteString
		call CRLF
		push OFFSET adminSelection
		call WriteStrArr

		mov al, SPACE   ; Load tab character
		call WriteChar  ; Print tab
		lea edx,choose
		call WriteString
		call ReadChar
		mov loginChoose, al
				call CRLF
		exit

	;REGISTER
		;register:
		mov eax,offset register
		mov ebx, lengthof register
		call PrintHeader
		call CRLF

		lea edx,loginMsg
		call WriteString
		mov edx, OFFSET inputUsername  ; use username as buffer
		mov ecx, MAX	;to ensure the user only input 20 char
		call ReadString
		call CRLF
		

		lea edx,passMsg
		call WriteString
		mov edx, OFFSET inputPassword
		mov ecx, 20	;to ensure the user only input 20 char
		call ReadString
		call CRLF
		; lea edx,password


		jmp check_admin 





	;RECEIPT--------------------------------
		mov eax, offset headerReceipt
		mov ebx, lengthof headerReceipt
		call PrintHeader

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


		mov eax, offset report
		mov ebx, lengthof report
		call PrintHeader

		 call CRLF				; new line

		 mov al, SPACE   ; Load tab character
		 call WriteChar  ; Print tab
		 call WriteChar  ; Print tab
		 call WriteChar  ; Print tab
		 



		;call WaitMsg
		;call Clrscr
		;call LOGIN
	exit
main ENDP

END main