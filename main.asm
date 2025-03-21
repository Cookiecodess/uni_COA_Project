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
	
	;REPORT--------------------------------------------------------------------

		report byte "Report",0

.code
main PROC
	;USER LOGIN------------------------------
		
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
	;RECEIPT--------------------------------
		lea edx,receipt
		




		mov  eax,1000 ;delay 1 sec
		call Delay

	;REPORT---------------------------------
		
		
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