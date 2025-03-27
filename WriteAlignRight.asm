INCLUDE Irvine32.inc

.data
SPACE = 32
str1			BYTE	"Single Journey",0
str2			BYTE	"Daily Pass",0
strSuperLong	BYTE	"Single Journey hahahahaaskaksjaknsjdnaksdnakdsnkasjdaknsndkasndkajskndajskdnaks jdndjasdnasj asidaosjdaoisdjoaidoasodaijsodaijsodai",0

.code 
main PROC
	push offset str1
	push sizeof str1
	call WriteAlignRight

	push offset str2
	push sizeof str2
	call WriteAlignRight

	push offset strSuperLong
	push sizeof strSuperLong
	call WriteAlignRight
	

	exit
main ENDP

; [ebp+12] = strOffset
; [ebp+8]  = strLength
WriteAlignRight PROC
	push ebp
	mov ebp, esp

	; save original values of registers
	push edx
	push ecx

	; clear edx
	xor edx, edx		
	
	; get max no. of columns in console
	call GetMaxXY		; dl = number of columns in console
	movzx edx, dl		; we only want dl, fill the rest with 0's

	; edx = number of columns - (ticketTypeArr.length() - 1)
	sub edx, dword ptr [ebp+8]
	inc edx

	; if edx >= 0 (string length not larger than console width), continue
	jge continue
	; else, write string as normal and abort
	mov edx, dword ptr [ebp+12]
	call WriteString
	jmp done

continue:
	; write spaces
	mov ecx, edx
	mov al, SPACE
SpaceLoop:
	call WriteChar
	loop SpaceLoop

	; write string
	mov edx, dword ptr [ebp+12]
	call WriteString

done:
	; restore old values of registers
	pop ecx
	pop edx

	pop ebp
	ret 8
WriteAlignRight ENDP

end main