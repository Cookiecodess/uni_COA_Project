INCLUDE Irvine32.inc

.data
	; string	byte	"you: hello assembly :) i hope we become great friends", 0
	; string2 byte	"me:  hello :) im gonna be ur worst nightmare ;)", 0""
.code
test PROC
	mov eax, 1
	mov ecx, 0
	
	; l1:
		; inc eax
		; call dumpregs
		; loop l1
	; lea edx,string
	; call WriteString
	; call CrLf

	; lea edx, string2
	; call WriteString

	ret
test ENDP
END

