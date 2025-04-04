
.data
		totalprice byte ?
		totalticketsold byte ?

		reportHeader byte "Report",0
.code

ReportPage proc

		ReportStart:
		;ticket price * ticket sold


		mov eax, offset reportHeader
		mov ebx, lengthof reportHeader
		call PrintHeader

		 call CRLF				; new line

		 mov al, TAB   ; Load tab character
		 call WriteChar  ; Print tab
		 call WriteChar  ; Print tab
		 call WriteChar  ; Print tab
		 



		call WaitMsg
		;call Clrscr
		;call LOGIN
		ret
ReportPage endp






