INCLUDE Irvine32.inc
	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	SPACE =09h
.data
	;HEADER-------------------------------------------------------------------
	header1             byte "Main Menu",0
    headerLogin			byte "Login",0
    headerReceipt		byte "Receipt",0
    header3             byte "Register",0
    equalSign           byte '='
    leftRightPadding    dword 40

	;GENERAL------------------------------------------------------------------
	MAX	= 20								; max characters to read
	inputBuffer			byte  MAX+1 dup(?)  ; room for null character


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
	customer byte "CUSTOMER",0
	customerSelection byte "View Schedule",0,"Ticketing",0,"Buy Member",0,"Check Nearby Station",0,"log out",0,0

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
			mov eax, offset customer
			mov ebx, lengthof customer
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
		mov eax, offset customer
		mov ebx, lengthof customer
		call PrintHeader

		mov edx, offset successMsg
		call WriteString
		call CRLF
		push OFFSET customerSelection
		call WriteStrArr

		exit

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

;--------------------------------------------------------
; PrintHeader
;
; Description:
;   Prints a formatted header with top and bottom borders of `=` characters.
;
; Parameters:
;   EAX  - (DWORD) Address of the header string (offset)
;   EBX  - (DWORD) Length of the header string
;
; Returns:
;   None
;
; Example usage:
;   mov eax, offset header
;   mov ebx, lengthof header
;   call PrintHeader
;--------------------------------------------------------
PrintHeader proc
    push ebp        ; save old base pointer
    mov ebp, esp    ; store the base address of the stack frame
    sub esp, 8      ; allocate memory for local variables

    ; store local variables
    mov dword ptr [ebp-4], eax
    mov dword ptr [ebp-8], ebx

    ; print top border
    ; for (2*leftRightPadding + lengthof(header)) { print('='); }
    mov eax, leftRightPadding
    mov ecx, 2
    mul ecx
    add eax, dword ptr [ebp-8]  
    mov ecx, eax
PrintLoop:
    mov al, equalSign
    call WriteChar
    loop PrintLoop

    ; print newline
    call Crlf  ; Irvine32 provides this

    ; print left padding spaces
    mov ecx, leftRightPadding
SpaceLoop:
    mov al, ' '
    call WriteChar
    loop SpaceLoop

    ; print header text
    mov edx, [ebp-4]  ; Adjusted stack offset
    call WriteString

    ; print newline
    call Crlf

    ; print bottom border
    ; for (2*leftRightPadding + lengthof(header)) { print('='); }
    mov eax, leftRightPadding
    mov ebx, 2
    mul ebx
    add eax, [ebp-8]  ; Adjusted
    mov ecx, eax
PrintLoop2:
    mov al, equalSign
    call WriteChar
    loop PrintLoop2

    ; print newline
    call Crlf
    
    ; restore stack pointer
    mov esp, ebp
    ; restore base pointer
    pop ebp
    ret
PrintHeader endp


;--------------------------------------------------------
; WriteStrArr
;
; Description:
;   Prints a numbered list of an array
;
; Parameters:
;   [EBP+8]  - (DWORD) Offset address of the string array
;
; Returns:
;   EAX  - (DWORD) Length of the string array
;
; Example usage:
;   .data
;   stringArray		  BYTE  "hello",0,"world",0,"this",0,"test",0,"dope",0,0
;   stringArrayLength DWORD ?
;
;   .code
;       push offset stringArray
;       call WriteStrArr
;       mov stringArrayLength, eax		; stringArrayLength = eax = 5
;--------------------------------------------------------
WriteStrArr PROC
	push ebp		; save current base pointer first
	mov ebp, esp	; move sp to bp for us to access the parameters stored in the stack

	; save old values of ESI and EDX
	push esi
	push edx

	mov esi, [ebp+8] ; load the address of the string array to esi
	mov al, SPACE  
	call WriteChar

	; printing the index number
	mov edx, 1
	mov eax, edx
	call writeDec
	mov al, ")"
	call writeChar
	mov al, " "
	call writeChar
	
	writeDatShitOut:

		mov al, [esi]		; load character into al then compare is it 0
		cmp al, 0			; if its 0, we know to print new line and write next string
		je nextShit

		call writeChar		; write that char out if not equal
		inc esi
		jmp writeDatShitOut


	nextShit:
		call crlf
		inc esi

		cmp byte ptr [esi+1], 0		; if the next char is also 0 then we know its the end of the array
		je done

		mov al, SPACE  
		call WriteChar

		inc edx				; writing the index number
		mov eax, edx
		call writeDec
		mov al, ")"
		call writeChar
		mov al, " "
		call writeChar

		jmp writeDatShitOut



	done:

	; store return value (length of the string array) in EAX
	mov eax, edx

	; restore old ESI and EDX
	pop edx
	pop esi

	pop ebp			; restore the initial base pointer before returning back to caller
	ret 4			; clear the stack pointer before returning

WriteStrArr ENDP


END main