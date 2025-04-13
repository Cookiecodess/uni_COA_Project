	SPACE = 20h
.data

	distance DWORD ?
	distanceWord byte "Distance",0
	totalSoldWod byte "Total Sales Amount",0
	totalTicketSoldWod byte "Total Ticket Sold",0
	morning byte "Morning",0
	noon byte "Noon",0
	evening byte "Evening",0
	sumWord byte "Sum",0
	costWord byte "Cost",0
	profitWord byte "Profit",0
	initialEax dword 0
.code


profit proc
	LOCAL temp1:DWORD
	LOCAL temp2:DWORD
	LOCAL temp3:DWORD
	call Clrscr
			mov eax, offset profitWord
		mov ebx, lengthof profitWord
		call PrintHeader


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
		
	
			; IGNORE THIS SECTION
			jmp EndTeleport
	Teleport:
		loop outterLoop
		jmp done
	EndTeleport:
		; THIS IS JUST TO SOLVE THE "JUMP TOO FAR" ISSUE




			call crlf
			mov al,TAB
			call writechar
			call writechar
			lea edx,distanceWord
			call writeString
			mov al,':'
			call writechar


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
						mov distance,eax
			
			
			push eax
				;push ebx
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
						mov temp3, eax
					

						;morning
						
						call crlf
						mov al,TAB
						call writechar
						call writechar
						call writechar
						call writechar
						lea edx,morning
						call writeString
						mov al,':'
						call writechar
						mov edx,temp3
					
						mov eax,time1[edx]
						call writedec
						mov totalSold[edx],eax


					 ;noon
						
						call crlf
						mov al,TAB
						call writechar
						call writechar
						call writechar
						call writechar
						lea edx,noon
						call writeString
						mov al,':'
						call writechar
						mov edx,temp3
					
						mov eax,time2[edx]	;eax=time2[edx]
						call writedec
						add eax,totalSold[edx]	;eax=time1+time2
						mov totalSold[edx],eax	;totalsold[edx]=eax


						;evening
						
						call crlf
						mov al,TAB
						call writechar
						call writechar
						call writechar
						call writechar
						lea edx,evening
						call writeString
						mov al,':'
						call writechar
						mov edx,temp3
					
						mov eax,time3[edx]	;eax=time3[edx]
						call writedec
						add eax,totalSold[edx]	;eax=time1+time2+time3
						mov totalSold[edx],eax	;totalsold[edx]=eax
						call crlf

						;total ticket sold 
							mov al,TAB
							call writechar
							call writechar
							call writechar
							call writechar
							lea edx,totalTicketSoldWod
							call writeString
							mov al,':'
							call writechar
							mov edx,temp3
							mov eax,totalSold[edx]
							call writedec
							call crlf

						;total sales
						mov al,TAB
						call writechar
						call writechar
						lea edx,totalSoldWod
						call writeString
						mov al,':'
						call writechar

						pop eax
							mov edx,temp3
					mul totalSold[edx]	;eax=((distance1-distance2)/4)*totalSold
					div temp1	

					mov totalSales[edx], eax
					mov eax,totalSales[edx]
					call writeInt
				;pop ebx

			pop eax
			push eax

			
			
			call crlf
			mov al,TAB
			call writechar
			call writechar



			lea edx,costWord
			call writeString
			mov al,':'
			call writechar



			mov eax, distance
			mov edx, 20
			mul edx                     
			mov edx, 3				;for now, multiply by 3: 1 day 3 times.
			mul edx
			call writeint                       
			mov edx,temp3	;save the profit
			mov totalCost[edx],eax
			


			; Calculate and display PROFIT (earnings - cost)
			call crlf
			mov al,TAB
			call writechar
			call writechar
			lea edx,profitWord
			call writeString
			mov al,':'
			call writechar

		  
			mov edx,temp3
			mov eax,totalSales[edx]
			call writeint
			sub eax, totalCost[edx]   
			
			call writeint               
				;save the profit
			mov totalProfit[edx],eax

			pop eax
			call crlf
			inc ebx
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


		ret
profit endp

 