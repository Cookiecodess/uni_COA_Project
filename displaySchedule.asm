include irvine32.inc

.data
	locations BYTE "Damansara", 0,"Bukit Bintang", 0, "Maluri", 0, 0
	promptAsk BYTE "Please Enter Your Current Location: ", 0
	promptBad BYTE "Invalid input, please enter again", 0
	showChoice BYTE "Choice picked: ", 0
	choice DWORD ?

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

	read:
		mov edx, OFFSET promptAsk
		call writestring
		call ReadDec		;  asking user for input
		cmp eax, 3			; if the choice is above 3 then invalid
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
		mov choice, eax
		mov edx, OFFSET showChoice
		call writestring
		call writedec




	ret
schedule endp



end main