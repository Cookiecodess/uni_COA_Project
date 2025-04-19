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
INCLUDE test.inc
INCLUDE NearStationPage.inc
;INCLUDE seatAvailability.inc
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
	tryagain		byte	"Try Again",0
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

	adminOption1		byte	"Report",0
	adminOption2		byte	"Profit Detail",0
	adminOption3		byte	"-",0
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

	customerInOption1			byte	"Log In",0
	customerInOption2			byte	"Register",0
	goBack			byte	"Go Back",0

	customerInSelectionArr	dword	OFFSET customerInOption1, OFFSET customerInOption2, OFFSET goBack

	CloginFailSelection	dword	OFFSET tryagain, OFFSET header3, OFFSET goBack
	AloginFailSelection	dword	OFFSET tryagain, OFFSET goBack
	
	cannotBeBlank byte	"Sorry, your input cannot be blank.",0


	customerSelectionArrLength dword ?
	promptCustomerPageHead byte "Please select an action (1-",0
	promptCustomerPageTail byte "): ",0
	promptUserPage byte "Select an action to continue.",0

	;REGISTER
	register byte "REGISTER",0
	reInputNameMSG byte "Sorry, your username cannot leave blank.",0
	reInputPasswordMSG byte "Sorry, your password cannot leave blank.",0
	regSus byte "Register Suscessful! You will be redirected to the customer page in 3 seconds.",0
	currentUserCount dword 0

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
			je customerInterface		;je=jump if equal to
		cmp loginChoose, 1	; If user chose Customer
			je adminLogin
		cmp loginChoose, 1	; If user chose Customer
			exit
	
	ret

startPage endp

customerInterface proc
	call clrscr
	invoke InitMenu, offset headerCustomer, offset customerInSelectionArr, lengthof customerInSelectionArr, offset promptUserPage, 0, 0
	mov loginChoose, al
				call CRLF
		cmp loginChoose, 0	; If user chose Admin
			je customerLogin		;je=jump if equal to
		cmp loginChoose, 1	; If user chose Customer
			je registerPage
		cmp loginChoose, 2	; If user chose Customer
			
	
	ret

customerInterface endp

customerLogin proc
	clstart:
			call Clrscr
			mov eax, offset headerCustomer
			mov ebx, lengthof headerCustomer
			call PrintHeader
			call CRLF
			

		;when user is undefined
			;mov eax,offset cUsername
			;cmp byte ptr [eax],0
			;je jumpRegisterPage

			
			lea edx,loginMsg
			call WriteString

			
			mov edx, offset inputUsername
			mov ecx, MAX	;to ensure the user only input 20 char?
			call ReadString
			call CRLF

			; lea edx,username
			;if user dont have input
			cmp byte ptr [edx],0
			je noInput


			lea edx,passMsg
			call WriteString

			mov edx, offset inputPassword
			mov ecx, MAX	;to ensure the user only input 20 char
			call ReadString
			call CRLF
			; lea edx,password
			
			


			mov esi, 0 
			;jmp check_customer
	check_customer:	
			;push OFFSET inputUsername
			;push OFFSET cUsername
			;call Str_compare			
			;jne Clogin_failed

			;push OFFSET inputPassword
			;push OFFSET cPassword
			;call Str_compare	
			;jne Clogin_failed

			cmp esi, currentUserCount
				jg Clogin_failed	;out of log in number,meaning dont have this user
			;get usernamearray esi
			mov ecx, esi	;know now is check which user already
			call GetUsernameSlot	; EDI = &userArray[esi * MAX_LENGTH]
			
			
		;debug
			;mov edx,edi
			;call writeString
			;call crlf
			;lea edx,inputUsername
			;call writeString
			;call crlf	
			


			;compare inpurUsername with userarray[esi]
			invoke Str_compare, EDI, ADDR inputUsername

			jne next_user		;not same username, just skip        
			


			;check password
				mov ecx, esi
				call GetPasswordSlot


		;debug
			;mov edx,edi
			;call writeString
			;call crlf
			;lea edx,inputPassword
			;call writeString
			;call crlf


				invoke Str_compare, EDI, ADDR inputPassword
				je Clogin_success



			;if not ,check next
				inc esi
				 jmp check_customer
			
			

		Clogin_success:
			; at this point, customer is successfully authenticated
			mov showLoginSuccessMsg, 1
			call customerPage


		call WaitMsg

		call Clrscr

		;JMP LOGIN
noInput:
mov  eax,red+(black*16)
		call SetTextColor
		mov edx, OFFSET cannotBeBlank
		call WriteString
		call Crlf
		mov  eax,lightGray+(black*16)
		call SetTextColor

		; clear user-input username, password
		mov edi, OFFSET inputUsername
		mov ecx, MAX
		mov al, 0
		rep stosb

		mov edi, OFFSET inputPassword
		mov ecx, MAX
		mov al, 0
		rep stosb
		call waitmsg
		jmp clstart
next_user:
    inc esi
    jmp check_customer

	Clogin_failed:		;==============================
		mov  eax,red+(black*16)
		call SetTextColor
		mov edx, OFFSET failMsg
		call WriteString
		call Crlf
		mov  eax,lightGray+(black*16)
		call SetTextColor

		; clear user-input username, password
		mov edi, OFFSET inputUsername
		mov ecx, MAX
		mov al, 0
		rep stosb

		mov edi, OFFSET inputPassword
		mov ecx, MAX
		mov al, 0
		rep stosb
		call waitmsg


		INVOKE InitMenu,0,offset CloginFailSelection,lengthof CloginFailSelection,0,0,0

			mov loginChoose, al
				call CRLF
		cmp loginChoose, 0	
			je clstart		
		cmp loginChoose, 1	
			je registerPage
		cmp loginChoose, 2	; If user chose Customer
		je startPage
		ret
customerLogin endp

JumpRegisterPage proc
		mov  eax,red+(black*16)
		call SetTextColor


		lea edx,needToReg
		call WriteString
			mov  eax,lightGray+(black*16)
			call SetTextColor
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
		mov ecx, currentUserCount
		call GetUsernameSlot	;return edi= username[]
		mov edx, edi  ; save username place
		mov ecx, MAX-1	;to ensure the user only input 19 char
		call ReadString
		call CRLF
		cmp eax, 0
			je reInputUserName

				;debug
		;mov edx,edi
		;call writeString
		
		invoke Str_copy,edi,ADDR cUsername	;save the name so that after that other can use it
			;debug
		;lea edx,cUsername
		;call writeString

;password
		lea edx,passMsg
		call WriteString
		mov ecx, currentUserCount
		call GetPasswordSlot      	;return edi= password[]
		mov edx, edi  ; save password place
		mov ecx, MAX-1	;to ensure the user only input 19 char
		call ReadString
		call CRLF
		cmp eax, 0
			je reInputPassword

		invoke Str_copy,edi,ADDR cPassword	;save string
		;mov cPassword,dh
		lea edx,regsus
		mov  eax,green+(black*16)
			 call SetTextColor
			 call writeString
			 mov  eax,lightGray+(black*16)
			 call SetTextColor

			 				mov  eax,3000 ;delay 3 sec
				call Delay
		call customerLogin 

		reInputUserName:
			call reUsername
			jmp rStart

		reInputPassword:
			call rePassword
			jmp rStart



		ret

registerPage endp

adminLogin proc

	alstart:
	call clrscr
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
		mov  eax,red+(black*16)
		call SetTextColor
		mov edx, OFFSET failMsg
		call WriteString
		mov  eax,lightGray+(black*16)
		 call SetTextColor
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

		call waitmsg


		INVOKE InitMenu,0,offset AloginFailSelection,lengthof AloginFailSelection,0,0,0

			mov loginChoose, al
				call CRLF
		cmp loginChoose, 0	
			je alstart		

		cmp loginChoose, 1	; If user chose Customer
		je startPage




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
			call show
			jmp backToCustomerPage

		Ticketing:
			call TicketingPage ; return a dword representing whether to simply go back to customer page or proceed to receipt?
			cmp eax, -1	; If EAX = -1, redraw customer menu
			je backToCustomerPage
			; Else, proceed to receipt module
		ProceedToReceipt:
			; TODO: Integrate the receipt printing module here
			;       and THEN remove `jmp backToCustomerPage`.
			jmp backToCustomerPage

		NearStation:
			;call NearStationPage
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
			je callProfit

			cmp eax,2
			je callTest
			cmp eax, 3		; logOut
			je logOut		
		
			jmp adminStartPage		; TEMP: redraw adminPage if user selects an option that hasn't been implemented
		
		
		callReport:

			call ReportPage
			jmp backToAdminPage


		callProfit:
			call Profit
			call waitmsg
			jmp backToAdminPage

		callTest:
			call GenerateReceipt
			jmp backToAdminPage

		backToAdminPage:
			jmp adminStartPage

		logOut:
		call startPage

			ret
adminPage endp

reUsername proc
			mov  eax,red+(black*16)
			 call SetTextColor
			lea edx,reInputNameMSG
			call writeString
			mov  eax,lightGray+(black*16)
			call SetTextColor
			call CRLF
			ret
reUsername endp

rePassword proc

			mov  eax,red+(black*16)
			call SetTextColor
			lea edx,reInputPasswordMSG
			call writeString
			mov  eax,lightGray+(black*16)
			call SetTextColor
			call CRLF
			ret
rePassword endp


getUsernameSlot PROC


		 mov eax, ecx       ; EAX = index
		 mov ebx, MAX     ;each user have 21 lenght
		 mul ebx	;eax=index* MAX
		 mov edi, OFFSET userNameArray	
		add edi, eax            ; EDI = userNameArray + index * MAX_LENGTH
		ret

;first user
	;OFFSET userNameArray + 0 * MAX 
	;= OFFSET userNameArray + 0 * 20 
	;= OFFSET userNameArray + 0

;fourth user
	;OFFSET userNameArray + 3 * MAX 
	;;= OFFSET userNameArray + 3 * 20 
	;= OFFSET userNameArray + 60
getUsernameSlot endp

GetPasswordSlot PROC


		 mov eax, ecx       ; EAX = index
		 mov ebx, MAX     ;each user have 20 lenght
		 mul ebx	;eax=index* MAX
		 mov edi, OFFSET passwordArray	
		add edi, eax            ; EDI = passwordArray + index * MAX_LENGTH
		ret


GetPasswordSlot endp


END main
