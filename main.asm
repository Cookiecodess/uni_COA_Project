INCLUDE Irvine32.inc
	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	SPACE =09h
.data
	header1             byte "Main Menu",0
    headerLogin			byte "Login",0
    headerReceipt		byte "Receipt",0
    header3             byte "Register",0
    equalSign           byte '='
    leftRightPadding    dword 10

	; header byte	"======================================",0
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
		mov eax, offset headerLogin
		mov ebx, lengthof headerLogin
		call PrintHeader
		; lea edx,header

		; call WriteString
		; lea edx,welcome
		; call WriteString
		; lea edx,header
		; call WriteString
		; call CRLF
	
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
				call CRLF
		cmp loginChoose, '1'	; If user chose Admin
		je 	customerLogin		;je=jump if equal to
		cmp loginChoose, '2'	; If user chose Customer
		je adminLogin

		adminLogin:
		mov eax, offset admin
		mov ebx, lengthof admin
		call PrintHeader

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
		mov eax, offset customer
		mov ebx, lengthof customer
		call PrintHeader
				call CRLF
				jmp exitLogin 
		
		exitLogin:;


		call WaitMsg

		call Clrscr

		;JMP LOGIN
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


END main