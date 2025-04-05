	SPACE = 20h
.data
	location1 BYTE "Penang",0 
	location2 BYTE "Perak",0 
	location3 BYTE "Kuala Lumpur",0 
	location4 BYTE "Melaka",0 
	location5 BYTE "Johor",0 
	locations DWORD OFFSET location1, OFFSET location2, OFFSET location3, OFFSET location4, OFFSET location5
	distances DWORD 1000, 580, 350, 200, 0
.code


profit proc
	
	mov esi, 0
	mov ecx, lengthof locations
	outterLoop:
		mov edx, locations[esi]
		call writeString
		call crlf

		push ecx	;save outterLoop
		push esi
		mov ecx, lengthof locations	;inner
		mov esi, 0
		innerLoop:
		
			mov edx,locations[esi]
			call writeString
			call crlf
			add esi, type locations
			loop innerLoop

		pop esi	
		pop ecx	;resume outterLoop

		add esi, type locations

		loop outterLoop
		

		call waitmsg

		ret
profit endp

 