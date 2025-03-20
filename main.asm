INCLUDE Irvine32.inc
	CR = 0Dh ; Carriage Return
	LF = 0Ah ; Line Feed

	
.data
	; string	byte	"you: hello assembly :) i hope we become great friends", 0
	; string2 byte	"me:  hello :) im gonna be ur worst nightmare ;)", 0""
	
	
	
	
;REPORT--------------------------------------------------------------------
	header byte	"-------------------------------",0
	report byte "            Report",0
.code
main PROC
	mov eax, 1
	mov ecx, 0
	call dumpregs
	
	; l1:
		; inc eax
		; call dumpregs
		; loop l1
	; lea edx,string
	; call WriteString
	; call CrLf

	; lea edx, string2
	; call WriteString














;REPORT---------------------------------
	lea edx,header
	call WriteString
	call CrLf				; new line

	lea edx,report
	call WriteString
	call CrLf

	lea edx,header
	call WriteString




	exit
main ENDP
END main

