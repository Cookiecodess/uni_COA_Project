INCLUDE Irvine32.inc
INCLUDE globalData.inc
INCLUDE generalFunctions.inc
INCLUDE coolMenu.inc
INCLUDE coolStationMap.inc
INCLUDE displayScheduleAndCalculations.inc
INCLUDE TicketingPage.inc
INCLUDE ReportPage.inc
INCLUDE ReceiptPage.inc
INCLUDE calculateProfit.inc
INCLUDE NearStationPage.inc
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

	userOption1		byte	"Customer",0
	userOption2		byte	"Admin",0
	userOption3		byte	"Exit",0
	userOptions		dword	OFFSET userOption1, OFFSET userOption2, OFFSET userOption3

	loginChoose		byte	?

	cUsername		byte	MAX+1 DUP(0)
	cPassword		byte	MAX+1 DUP(0)

	aUsername		byte	"1",0
	aPassword		byte	"1",0


	loginMsg		byte	"Enter Username: ", 0
	passMsg			byte	"Enter Password: ", 0

	successMsg		byte	"Login Successful!", 0
	failMsg			byte	"Login Failed!", 0

		
	inputUsername	byte	MAX+1 DUP(?)	;like char[20] and initialize it by 0(null)
	inputPassword	byte	MAX+1 DUP(?)	;but user only can type 19 word since at 20 need to store the 0(to stop)
	
	;ADMIN LOGIN
	headerAdmin byte "ADMIN",0

	adminOption1		byte	"View Today Earning Report",0
	adminOption2		byte	"View Month Earning Report",0
	adminOption3		byte	"View Year Earning Report",0
	adminOption4		byte	"Log out",0

	adminSelectionArr	dword	OFFSET adminOption1, OFFSET adminOption2, OFFSET adminOption3, OFFSET adminOption4

	;CUSOTMER LOGIN
	headerCustomer byte "CUSTOMER",0
	needToReg byte "Sorry you need to register first",0

	customerOption1			byte	"View Schedule",0
	customerOption2			byte	"Ticketing",0
	customerOption3			byte	"Check Nearby Station",0
	customerOption4			byte	"log out",0

	customerSelectionArr	dword	OFFSET customerOption1, OFFSET customerOption2, OFFSET customerOption3, OFFSET customerOption4

	customerSelectionArrLength dword ?
	promptCustomerPageHead byte "Please select an action (1-",0
	promptCustomerPageTail byte "): ",0
	promptUserPage byte "Select an action to continue.",0

	;REGISTER
	register byte "REGISTER",0

	

.code
main PROC
	call startPage

	
	exit
main ENDP




startPage proc
	start:
	invoke InitMenu, offset headerLogin, offset userOptions, lengthof userOptions, offset choose, 0, 0

		mov loginChoose, al
				call CRLF
		cmp loginChoose, 0	; If user chose Admin
			je customerLogin		;je=jump if equal to
		cmp loginChoose, 1	; If user chose Customer
			je adminLogin
		cmp loginChoose, 1	; If user chose Customer
			exit
	
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
		mov eax, offset headerAdmin
		mov ebx, lengthof headerAdmin
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
			invoke InitMenu, offset headerCustomer, offset customerSelectionArr, lengthof customerSelectionArr, offset promptUserPage, 0, 0
			; EAX = index of selected option

			cmp eax, 0
				je ViewSchedule

			cmp eax, 1		; selection: Ticketing
				je Ticketing

			
			cmp eax, 2		; selection: NearStation
				je NearStation

			cmp eax, 3		; logOut
				je logOut	

			jmp customerPageStart		; TEMP: redraw customerPage if user selects an option that hasn't been implemented

		ViewSchedule:
			call ViewSchedulePage
			jmp backToCustomerPage

		Ticketing:
			call TicketingPage ; return a dword representing whether to simply go back to customer page or proceed to receipt?
			cmp eax, -1	; If EAX = -1, redraw customer menu
			je backToCustomerPage
			; Else, proceed to receipt module
		ProceedToReceipt:

		NearStation:
			call NearStationPage
			cmp eax, -1	; If EAX = -1, redraw customer menu
			je backToCustomerPage

		backToCustomerPage:
			jmp customerPageStart

		logOut:
			call startPage

			ret

customerPage endp

adminPage proc
		adminStartPage:
		; mov eax, offset admin
		; mov ebx, lengthof admin
		; call PrintHeader
		; call CRLF
		; mov edx, offset successMsg
		; call WriteString
		; call CRLF
		; push OFFSET adminSelection
		; push LENGTHOF adminSelection
		; call WriteStrArr

		; mov al, TAB   ; Load tab character
		; call WriteChar  ; Print tab
		; lea edx,choose
		; call WriteString
		; call ReadChar
		; mov loginChoose, al
				; call CRLF

			invoke InitMenu, offset headerAdmin, offset adminSelectionArr, lengthof adminSelectionArr, offset promptUserPage, 0, 0
			; EAX = index of selected option

			cmp eax, 0		; selection: report
			je callReport
			
			cmp eax, 1		; test
			je callReceipt

			cmp eax, 3		; logOut
			je logOut		
		
			jmp adminStartPage		; TEMP: redraw adminPage if user selects an option that hasn't been implemented
		
		
		callReport:
			call ReportPage
			jmp backToAdminPage


		callReceipt:
			call Profit
			jmp backToAdminPage


		backToAdminPage:
			jmp adminStartPage

		logOut:
		call startPage

			ret
adminPage endp




END main
