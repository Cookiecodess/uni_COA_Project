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
		choose byte "Choose a number to login",0
		user1 byte "1. Customer",0
		user2 byte "2. Admin",0
		username byte "abcdAAA",0
		password byte "password123",0

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

		call WaitMsg

		call Clrscr

		;JMP LOGIN
	;RECEIPT--------------------------------
		mov eax, offset headerReceipt
		mov ebx, lengthof headerReceipt
		call PrintHeader
		; lea edx,header
		; call WriteString
		; lea edx,receipt
		; call WriteString
		; lea edx,header
		; call WriteString
		; call CRLF

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
		;ticket price * ticket sold


		mov eax, offset report
		mov ebx, lengthof report
		call PrintHeader
		; lea edx,header
		; call WriteString
		; call WriteString
		; call CRLF				; new line

		; mov al, SPACE   ; Load tab character
		; call WriteChar  ; Print tab
		; call WriteChar  ; Print tab
		; call WriteChar  ; Print tab
		; lea edx,report
		; call WriteString
		; call CRLF

		; lea edx,header
		; call WriteString
		; call WriteString


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