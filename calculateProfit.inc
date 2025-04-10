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
	cost DWORD ?
	distanceWord byte "Distance",0
	totalSoldWod byte "Total Sold",0
	costWord byte "Cost",0
	profitWord byte "Profit",0
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
			mov al,TAB
			call writechar
			mov edx,locations[edi*TYPE DWORD]
			call writeString


	; IGNORE THIS SECTION
		jmp EndTeleportForOuter
	TeleportForOuter:
		loop outterLoop
		jmp done
	EndTeleportForOuter:
	; THIS IS JUST TO SOLVE THE "JUMP TOO FAR" ISSUE


	; IGNORE THIS SECTION
		jmp EndTeleportForInner
	TeleportForInner:
		loop innerLoop
		jmp doneInnerLoop
	EndTeleportForInner:
	; THIS IS JUST TO SOLVE THE "JUMP TOO FAR" ISSUE
				
			call crlf
			mov al,TAB
			call writechar
			call writechar
			lea edx,distanceWord
			call writeString
			mov al,':'
			call writechar
	Teleport:
		loop outterLoop
		jmp done
	EndTeleport:

			mov eax,distances[esi*TYPE DWORD]	
						;call writeDec
						;call crlf
			mov edx,distances[edi*TYPE DWORD]
			sub eax,edx			;eax=distance1-distance2
			cmp eax, 0
			jge not_negative
			neg eax
		not_negative:
						call writeDec
						call crlf
			
			
			; --- calculate cost then moving it to 'cost', to be used to output then stored by subsequent loops again
			push ebx
			push eax
			call crlf
			mov al,TAB
			call writechar
			call writechar

			lea edx,totalSoldWod
			call writeString
			mov al,':'
			call writechar

			pop eax

			;mov eax, totalSold[(esi*4 + ebx)*TYPE DWORD]	
			push eax
			lea eax, [esi*4 + ebx]
			mov temp1, 4
			mul temp1
			mov edx, eax
			pop eax
			
			mul totalSold[edx]	;eax=((distance1-distance2)/2?)*totalSold
				
			call writeint




		; IGNORE THIS SECTION
			jmp EndTeleport
	
		; THIS IS JUST TO SOLVE THE "JUMP TOO FAR" ISSUE

					
			; --- calculate cost then moving it to 'cost', to be used to output then stored by subsequent loops again
			;push ebx
			;push eax
			;push edx
			;mov ebx, 20
			;mul ebx
			;mov cost, eax
			;pop ebx
			;pop eax
			;pop edx
			;inc ebx


			call crlf

		.ENDIF
			;add esi, type locations
			inc edi
			jmp TeleportForInner

	doneInnerLoop:
		;pop esi	
		pop ecx	;resume outterLoop

		;add esi, type locations
		inc esi
		call crlf
		jmp TeleportForOuter ; LOOP OUTER LOOP

		
		
	done:
		call waitmsg

		ret
profit endp

 