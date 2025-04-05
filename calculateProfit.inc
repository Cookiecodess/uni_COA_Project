	SPACE = 20h
.data
	locations BYTE "Penang", 0,"Perak", 0, "Kuala Lumpur", 0, "Melaka", 0, "Johor", 0, 0
	location1 BYTE "Penang",0 
	distances DWORD 1000, 580, 350, 200, 0
.code


profit proc
	
	mov esi, 0
	mov ecx,5
	outterLoop:
		push ecx	;save outterLoop
		push esi
		lea edx,locations[esi]
		call writeString
		mov ecx, 5	;inner
		call crlf
		innerLoop:
		
			lea edx,locations[esi]
			call writeString
			call crlf
			inc esi
			loop innerLoop

		pop esi	
		pop ecx	;resume outterLoop
		inc esi

		loop outterLoop
		

		call waitmsg

		ret
profit endp

 