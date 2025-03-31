include irvine32.inc
INCLUDE generalFunctions.inc
INCLUDE coolMenu.inc


	CR = 0Dh	; Carriage Return
	LF = 0Ah	; Line Feed
	TAB = 09h
	SPACE = 20h
.data
		totalprice byte ?
		totalticketsold byte ?

		report byte "Report",0
.code
main proc
	call generateReport


exit

main endp

generateReport proc

		ReportStart:
		;ticket price * ticket sold


		mov eax, offset report
		mov ebx, lengthof report
		call PrintHeader

		 call CRLF				; new line

		 mov al, TAB   ; Load tab character
		 call WriteChar  ; Print tab
		 call WriteChar  ; Print tab
		 call WriteChar  ; Print tab
		 



		;call WaitMsg
		;call Clrscr
		;call LOGIN
		ret
generateReport endp

end main







