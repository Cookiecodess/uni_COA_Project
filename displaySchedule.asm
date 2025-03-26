include irvine32.inc

.data
	promptAsk BYTE "Please Enter Your Current Location: ", 0
	promptAsk2 BYTE "Please Enter Your Target Location: ", 0

	promptBad BYTE "Invalid input, please enter again", 0

	msgCurrentLocation BYTE "Current Location: ", 0
	msgTargetLocation BYTE "Target Location: ", 0
	msgDistance BYTE "Distance to Travel: ", 0
	msgDistanceUnit BYTE "KM", 0

	locations BYTE "Penang", 0,"Perak", 0, "Kuala Lumpur", 0, "Melaka", 0, "Johor", 0, 0
	distances DWORD 1000, 580, 350, 200, 0
	distanceToTravel DWORD ?
	speed DWORD 320


	currentLocationAddress DWORD ?
	targetLocationAddress DWORD ?
	currentChoice DWORD 0
	currentChoice2 DWORD 0

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

	;generate index number
	mov eax, ebx
	call writedec
	mov al, ' '    ; Print a space after the number
	call writechar

	print_locations:
		mov al, [esi]
		call writechar
		inc esi
		cmp byte ptr [esi], 0
		je nextString

		loop print_locations
		jmp done

	nextString:
		call crlf		; new line
		inc esi
		cmp byte ptr [esi],0
		je done
		inc ebx

		;generate index number
		mov eax, ebx
		call writedec
		mov al, ' '    ; Print a space after the number
		call writechar

		jmp print_locations



	done: 
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


		
	lea esi, locations			; points esi to the first location
	findChoice:
		dec eax
		jz writeChoice							; if we're at the right choice now

		moveNextString:							; point to the next location starting address
			inc esi
			cmp byte ptr [esi-1], 0
			je findChoice
			jmp moveNextString

		
	
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
		mov eax, ebx
		call writedec
		mov al, ' '    ; Print a space after the number
		call writechar

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

	lea esi, locations			; points esi to the first location
	findChoice2:
		dec eax
		jz writeChoice2						; if we're at the right choice now

		moveNextString2:							; point to the next location starting address
			inc esi
			cmp byte ptr [esi-1], 0
			je findChoice2
			jmp moveNextString2

		
	
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
			mov edx, OFFSET msgDistance
			call writestring
			call writedec
			mov edx, OFFSET msgDistanceUnit
			call writestring
			call crlf



	ret
schedule endp



end main