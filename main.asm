INCLUDE Irvine32.inc
INCLUDE generalFunctions.inc
INCLUDE coolMenu.inc
INCLUDE TicketingPage.inc

	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	TAB = 09h
	SPACE = 20h

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
	; NOTE: These are defined in generalFunctions.inc
	; MAX	= 20								; max characters to read
	; inputBuffer			byte  MAX+1 dup(?)  ; room for null character


	;USER LOGIN----------------------------------------------------------------
	welcome			byte	" Welcome,here is a login page",0
	choose			byte	"Please select your role to continue : ",0
	user			byte	"Customer",0,"Admin",0,"Back",0,0
	loginChoose		byte	?

	cUsername		byte	MAX+1 DUP(0)
	cPassword		byte	MAX+1 DUP(0)

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
	needToReg byte "Sorry you need to register first",0
	customerSelectionArr byte "View Schedule",0,"Ticketing",0,"Buy Member",0,"Check Nearby Station",0,"log out",0,0
	customerSelectionArrLength dword ?
	promptCustomerPageHead byte "Please select an action (1-",0
	promptCustomerPageTail byte "): ",0
	promptCustomerPage byte "Select an action to continue.",0

	;REGISTER
	register byte "REGISTER",0
	



.code
main PROC
	call startPage

	
	exit
main ENDP


startPage proc
	start:
		mov eax, offset headerLogin
		mov ebx, lengthof headerLogin
		call PrintHeader

		lea edx,choose
		call WriteString
		call CRLF

		push OFFSET user
		call WriteStrArr

		mov al, TAB   ; Load tab character
		call WriteChar  ; Print tab
		lea edx,choose
		call WriteString


		call ReadChar
		mov loginChoose, al
				call CRLF
		cmp loginChoose, '1'	; If user chose Admin
		call 	customerLogin		;je=jump if equal to
		cmp loginChoose, '2'	; If user chose Customer
		call adminLogin

	
	ret

startPage endp



customerLogin proc
	clstart:
			mov eax, offset headerCustomer
			mov ebx, lengthof headerCustomer
			call PrintHeader
			call CRLF
			

		;when user is undefined
			mov eax,offset cUsername
			cmp byte ptr [eax],0
			je jumpRegisterPage


			
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




	check_customer:	
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
			call customerPage


		call WaitMsg

		call Clrscr

		;JMP LOGIN

	Clogin_failed:		;==============================
		mov edx, OFFSET failMsg
		call WriteString
		call Crlf
		jmp clstart  ; Retry Customer login

		; clear user-input username, password
		mov edi, OFFSET inputUsername
		mov ecx, MAX
		mov al, 0
		rep stosb

		mov edi, OFFSET inputPassword
		mov ecx, MAX
		mov al, 0
		rep stosb



customerLogin endp

JumpRegisterPage proc
		
		lea edx,needToReg
		call WriteString
		call Crlf
		call registerPage

JumpRegisterPage endp
registerPage proc
	rStart:
		call WaitMsg
		call Clrscr
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
		invoke Str_copy,ADDR inputUsername,ADDR cUsername

		lea edx,passMsg
		call WriteString
		mov edx, OFFSET inputPassword
		mov ecx, 20	;to ensure the user only input 20 char
		call ReadString
		call CRLF
		
		invoke Str_copy,ADDR inputPassword,ADDR cPassword	;save string
		;mov cPassword,dh

		call customerLogin 


		ret

registerPage endp

adminLogin proc

	alstart:
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

			call adminPage
	

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


		jmp alstart   ; Retry Admin login


adminLogin endp






customerPage proc
	customerPageStart:
			invoke InitMenu, offset headerCustomer, offset customerSelectionArr, offset promptCustomerPage, 0, 0
			; EAX = index of selected option

			cmp eax, 1		; selection: Ticketing
			je Ticketing
			
			jmp customerPageStart		; TEMP: redraw customerPage if user selects an option that hasn't been implemented

		Ticketing:
			call TicketingPage
			jmp backToCustomerPage

		backToCustomerPage:
			jmp customerPageStart

			ret

customerPage endp

adminPage proc
		adminStartPage:
		mov eax, offset admin
		mov ebx, lengthof admin
		call PrintHeader
		call CRLF
		mov edx, offset successMsg
		call WriteString
		call CRLF
		push OFFSET adminSelection
		call WriteStrArr

		mov al, TAB   ; Load tab character
		call WriteChar  ; Print tab
		lea edx,choose
		call WriteString
		call ReadChar
		mov loginChoose, al
				call CRLF


		backToAdminPage:
			jmp adminStartPage

			ret
adminPage endp




END main