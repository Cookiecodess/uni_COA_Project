INCLUDE Irvine32.inc
	CR = 0Dh ; Carriage Return
	LF = 0Ah ; Line Feed

	
.data

	
	;REPORT--------------------------------------------------------------------
		header byte	"======================================",0
		report byte "             Report",0
.code
main PROC








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

