include irvine32.inc

.data
	promptAsk BYTE "Please Enter Your Current Location: ", 0
	promptAsk2 BYTE "Please Enter Your Target Location: ", 0

	promptBad BYTE "Invalid input, please enter again", 0

	msgCurrentLocation BYTE "Current Location: ", 0
	msgTargetLocation BYTE "Target Location: ", 0
	msgDistance BYTE "Distance to Travel: ", 0
	msgDistanceUnit BYTE "KM", 0

	; === NOTE: locations and distances data will have to be commented
	; ===       once this file becomes an .inc to prevent redefinition
	; ===       since these data are already defined in globalData.inc
	; STATION LOCATIONS AND DISTANCES (FROM JOHOR)
	location1				BYTE	"Penang",0
	location2				BYTE	"Perak",0
	location3				BYTE	"Kuala Lumpur",0
	location4				BYTE	"Melaka",0
	location5				BYTE	"Johor",0
	locations				DWORD	OFFSET location1, OFFSET location2, OFFSET location3, OFFSET location4, OFFSET location5
	;locations BYTE "Penang", 0,"Perak", 0, "Kuala Lumpur", 0, "Melaka", 0, "Johor", 0, 0

	distances DWORD 1000, 580, 350, 200, 0
	distanceToTravel DWORD ?
	speed DWORD 320	


	currentLocationAddress DWORD ?
	targetLocationAddress DWORD ?
	currentChoice DWORD 0
	currentChoice2 DWORD 0

	;----Cost Variables----
	msgCost	BYTE "Total Cost: RM", 0
	msgCurrency BYTE ".00", 0
	cost DWORD ?
    
    ; ---Time Variables---
    msgTime BYTE "Travel Time: ", 0
    msgHours BYTE " hours ", 0
    msgMinutes BYTE " minutes", 0

	TimeHours DWORD ? 
    TimeMinutes DWORD ? 

	; ---Fee Variables---

	feeAmount DWORD ?
	msgFee BYTE "Travel Fee: RM", 0
	msgDecimal BYTE ".00", 0


.code
main proc
	

	call schedule
	exit
main endp


schedule proc
	start:
	; display all the locations
	mov ebx, 1
	mov esi, OFFSET locations
	mov ecx, LENGTHOF locations

	print_locations:
		;generate index number
		mov eax, ebx
		call writedec
		mov al, ' '    ; Print a space after the number
		call writechar

		mov edx, dword ptr [esi]
		call WriteString
		call CrLf

		; increment location pointer
		add esi, type locations

		; increment index number
		inc ebx

		loop print_locations

	; done:
	call crlf
	cmp dword ptr [currentChoice], 0	; comparing if currentChoice is 0
	jne read2							; if not equals 0 means its set and this is the second time we're reading input

	read:
		mov edx, OFFSET promptAsk
		call writestring
		call ReadDec		;  asking user for input
		cmp eax, 5			; if the choice is above 5 then invalid
		ja invalid
		cmp eax, 1			; if the choice is less than 1
		jb invalid

		jmp valid

	invalid:
		mov edx, OFFSET promptBad
		call clrscr
		call writestring
		call crlf
		call crlf
		jmp start

	valid:
		; print what the user chose
		call clrscr
		mov currentChoice, eax
		mov edx, OFFSET msgCurrentLocation
		call writestring


		
	; lea esi, locations			; points esi to the first location's string offset'
	dec eax		
	mov esi, dword ptr locations[eax * type locations]
	; findChoice:
		
		; jz writeChoice							; if we're at the right choice now'

		; add esi, type locations
		; jmp findChoice

		; moveNextString:							; point to the next location starting address
			; add esi, type locations
			; cmp dword ptr [esi-1], 0
			; je findChoice
			; jmp moveNextString

		
	
	writeChoice:
		mov currentLocationAddress, esi			; save the address of the current Station
		mov edx, esi
		call writestring
		call crlf
		call crlf

	
	askTargetLocation:
		; display all the locations
		mov ebx, 1
		mov esi, OFFSET locations
		mov ecx, LENGTHOF locations

		;generate index number
		; mov eax, ebx
		; call writedec
		; mov al, ' '    ; Print a space after the number
		; call writechar

		jmp print_locations

	read2:
		mov edx, OFFSET promptAsk2
		call writestring
		call ReadDec		;  asking user for input
		cmp eax, 5			; if the choice is above 5 then invalid
		ja invalid2
		cmp eax, 1			; if the choice is less than 1
		jb invalid2

		jmp valid2

	invalid2:
		call clrscr

		mov edx, OFFSET msgCurrentLocation	; print current location at the top
		call writestring
		mov edx, currentLocationAddress
		call writestring
		call crlf
		call crlf
		
		mov edx, OFFSET promptBad		; print error msg
		call writestring
		call crlf
		call crlf
		jmp askTargetLocation

	valid2:
		; print what the user chose
		call clrscr
		mov currentChoice2, eax

	; lea esi, locations			; points esi to the first location's string offset
	dec eax		
	mov esi, dword ptr locations[eax * type locations]

	; findChoice2:
		; dec eax
		; jz writeChoice2						; if we're at the right choice now'

		; moveNextString2:							; point to the next location starting address
			; inc esi
			; cmp byte ptr [esi-1], 0
			; je findChoice2
			; jmp moveNextString2

		
	
	writeChoice2:
		mov targetLocationAddress, esi			; save the address of the target Station

		; -------TO PRINT--------
		; Current Location: ...
		; Target Location: ...
		; Distance to Travel: ...

		mov edx, OFFSET msgCurrentLocation
		call writestring
		mov edx, currentLocationAddress
		call writestring
		call crlf


		mov edx, OFFSET msgTargetLocation
		call writestring
		mov edx, targetLocationAddress
		call writestring
		call crlf

		

		;finding which choice has the higher km
		lea esi, distances
		mov eax, dword ptr currentChoice	; point to choice1 km
		mov ebx, dword ptr currentChoice2	; point to choice2 km
		cmp eax, ebx
		jna choice2Smaller		; if choice2 km is smaller we use choice1 km - choice2 km

		dec eax					; choice2km - choice1km
		dec ebx
		mov ecx, 4
		mul ecx
		xchg eax, ebx
		mul ecx
		xchg eax, ebx
		mov ecx, eax
		mov eax, [esi+ebx]
		sub eax, [esi+ecx]
		jmp writeDistance

		choice2Smaller:
			dec eax
			dec ebx
			mov ecx, 4
			mul ecx
			xchg eax, ebx
			mul ecx
			xchg eax, ebx
			mov eax, [esi+eax]
			sub eax, [esi+ebx]

			jmp writeDistance

		writeDistance:
			mov distanceToTravel, eax  ;IMPORTANT!!!! STORES DISTANCE TO distanceToTravel
			mov edx, OFFSET msgDistance
			call writestring
			call writedec
			mov edx, OFFSET msgDistanceUnit
			call writestring
			call crlf

	; --- for now display all calculations after calculating distance ---
			call CalculateTrainCost
			call CalculateTravelTime
			call CalculateFee
			ret

	; --- calculate cost: distance x 20, stores in a variable (Cost) for future use ---
		CalculateTrainCost PROC
			mov ebx, 20
			mul ebx
			mov cost, eax

			mov	edx, OFFSET msgCost
			call writestring
			call writedec
			mov edx, OFFSET	msgCurrency
			call writestring
			call crlf

			ret
		CalculateTrainCost ENDP


	; --- calculate time: distance / speed(320), everything multiplied by x100, divided, then added 50, then divide by 100, to round up
	;stackoverflow.com/questions/12828166/
		CalculateTravelTime PROC


			mov eax, distanceToTravel
			pushad                   ; save registers

			; Calculate total minutes (distance Å~ 60 Å~ 100 + 50) / 320
			mov edi, 6000          ; EDI = 60*100 
			mul edi 
			mov edi, 320 
			div edi 
			add eax, 50 
			xor edx, edx
			mov edi, 100
			div edi 

			; Split to hours/minutes
			xor edx, edx
			mov edi, 60
			div edi                ; EAX = hours, EDX = minutes

			mov TimeHours, eax
			mov TimeMinutes, edx 

			mov esi, edx
			mov edx, OFFSET msgTime
			call WriteString        ; "Time: "
			mov eax, TimeHours           ; Hours (EAX)
			call WriteDec
			mov edx, OFFSET msgHours
			call WriteString        ; " hours "
			mov eax, TimeMinutes        
			call WriteDec           ; Minutes
			mov edx, OFFSET msgMinutes
			call WriteString        ; " minutes"
			call Crlf

			popad 
			ret
		CalculateTravelTime ENDP


	; --- calculate fee: distance / 2, will add more cases in the future (EX: member price is distance / 2.1 or smth)
		CalculateFee PROC
    
			push edx
    
			;calculate fee (distance / 2)
			mov eax, distanceToTravel
			mov ebx, 2
			xor edx, edx
			div ebx

			mov feeAmount, eax
    
			mov edx, OFFSET msgFee
			call WriteString
			call WriteDec
			mov edx, OFFSET msgDecimal
			call WriteString 
			call Crlf
    
			pop edx ;restore edx
			ret

		CalculateFee ENDP

			
schedule endp



end main