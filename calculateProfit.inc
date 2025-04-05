	SPACE = 20h
.data
	; The LOCATIONS array and DISTANCES array are defined in globalData.inc
	; location1 BYTE "Penang",0
	; location2 BYTE "Perak",0
	; location3 BYTE "Kuala Lumpur",0
	; location4 BYTE "Melaka",0
	; location5 BYTE "Johor",0
	; locations DWORD OFFSET location1, OFFSET location2, OFFSET location3, OFFSET location4, OFFSET location5
	; distances DWORD 1000, 580, 350, 200, 0
	totalSold DWORD 100,223,324,134,150,223,324,134,130,323,124,224,130,523,224,232,120,233,323,233
.code


profit proc
	mov ebx,0
	mov esi, 0
	mov ecx, lengthof locations
	outterLoop:

		mov edx, locations[esi]
		call writeString
		call crlf
		mov edi,ecx
		push ecx	;save outterLoop
		push esi

		mov ecx, lengthof locations	;inner
		mov esi, 0

		innerLoop:
		.IF ecx != edi
			mov edx,locations[esi]
			call writeString
			
			mov al,':'
			call writechar


			mov eax,distances[edi*TYPE DWORD]
						call writeDec
						call crlf
			mov edx,distances[ecx*TYPE DWORD]
			sub eax,edx
						call writeDec
						call crlf
			mul totalSold[ebx*TYPE DWORD]
			call writeint
			add ebx,type totalSold

			call crlf
		.ENDIF
			add esi, type locations
			loop innerLoop

		pop esi	
		pop ecx	;resume outterLoop

		add esi, type locations
		call crlf
		loop outterLoop
		

		call waitmsg

		ret
profit endp

 