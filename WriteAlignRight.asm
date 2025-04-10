INCLUDE Irvine32.inc

.data
SPACE = 20h
str1			BYTE	"Single Journey",0
str2			BYTE	"Daily Pass",0
strSuperLong	BYTE	"Single Journey hahahahaaskaksjaknsjdnaksdnakdsnkasjdaknsndkasndkajskndajskdnaks jdndjasdnasj asidaosjdaoisdjoaidoasodaijsodaijsodai",0

.code 
main PROC
	lea edx, str1
	mov ecx, sizeof str1
	call WriteAlignRight

	lea edx, str2
	mov ecx, sizeof str2
	call WriteAlignRight

	lea edx, strSuperLong
	mov ecx, sizeof strSuperLong
	call WriteAlignRight
	

	exit
main ENDP



; EDX = strOffset
; ECX = strLength
WriteAlignRight PROC
		push ebp
		mov ebp, esp

		; Allocate memory for 2 local variables
		sub esp, 8

		; Store arguments in local variables
		mov dword ptr [ebp-4], edx
		mov dword ptr [ebp-8], ecx

		; save original values of registers
		push edx
		push ecx

		; clear edx
		xor edx, edx		
	
		; get max no. of columns in console
		call GetMaxXY		; dl = number of columns in console
		movzx edx, dl		; we only want dl, fill the rest with 0's

		; edx = number of columns - (ticketTypeArr.length() - 1)
		sub edx, dword ptr [ebp-8]
		inc edx

		; if edx >= 0 (string length not larger than console width), continue
		jge continue
		; else, write string as normal and abort
		mov edx, dword ptr [ebp-4]
		call WriteString
		jmp done

	continue:
		; Left-pad with spaces
		mov ecx, edx		; ECX = number of columns - (ticketTypeArr.length() - 1)
		mov al, SPACE		; load SPACE character
	SpaceLoop:				
		call WriteChar
		loop SpaceLoop

		; write string
		mov edx, dword ptr [ebp-4]
		call WriteString

	done:
		; restore old values of registers
		pop ecx
		pop edx

		; restore stack pointer
		mov esp, ebp
		; restore base pointer
		pop ebp
		ret
WriteAlignRight ENDP

end main