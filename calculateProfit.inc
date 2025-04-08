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
	LOCAL temp1:DWORD
	LOCAL temp2:DWORD

	;mov ebx,0
	mov esi, 0
	mov ecx, lengthof locations
	outterLoop:

		mov edx, locations[esi*TYPE DWORD]
		call writeString
		call crlf
		;mov edi,esi
		push ecx	;save outterLoop
		;push esi

		mov ecx, lengthof locations	;inner
		mov edi, 0
		mov ebx, 0

		innerLoop:
		.IF esi != edi
			mov edx,locations[edi*TYPE DWORD]
			call writeString
			
			mov al,':'
			call writechar


			mov eax,distances[esi*TYPE DWORD]	
						call writeDec
						call crlf
			mov edx,distances[edi*TYPE DWORD]
			sub eax,edx			;eax=distance1-distance2
			cmp eax, 0
			jge not_negative
			neg eax
		not_negative:
						call writeDec
						call crlf
			;mov eax, totalSold[(esi*4 + ebx)*TYPE DWORD]	
			push eax
			lea eax, [esi*4 + ebx]
			mov temp1, 4
			mul temp1
			mov edx, eax
			pop eax
			mul totalSold[edx]	;eax=(distance1-distance2)*totalSold
			inc ebx

		; IGNORE THIS SECTION
			jmp EndTeleport
	Teleport:
		loop outterLoop
		jmp done
	EndTeleport:
		; THIS IS JUST TO SOLVE THE "JUMP TOO FAR" ISSUE

			call writeint
			;add ebx,type totalSold

			call crlf

		

		.ENDIF
			;add esi, type locations
			inc edi
			loop innerLoop

		;pop esi	
		pop ecx	;resume outterLoop

		;add esi, type locations
		inc esi
		call crlf
		jmp Teleport ; LOOP OUTER LOOP

		
		
	done:
		call waitmsg

		ret
profit endp

 