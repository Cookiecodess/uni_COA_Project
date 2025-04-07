INCLUDE Irvine32.inc

.data
string byte "TESTING",0

.code
main PROC
	lea edx, string

	; black, white, brown, yellow, blue, green, cyan, red, magenta, gray, lightBlue, lightGreen, lightCyan, lightRed, lightMagenta, and lightGray.
	mov eax, white
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, lightGray
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, gray
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, yellow
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, brown
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, green
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, lightGreen
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, blue
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, lightBlue
	call SetTextColor
	call WriteString
	call CrLf
	
	mov eax, cyan
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, lightCyan
	call SetTextColor
	call WriteString
	call CrLf
	
	mov eax, red
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, lightRed
	call SetTextColor
	call WriteString
	call CrLf

	mov eax, magenta
	call SetTextColor
	call WriteString
	call CrLf
	
	mov eax, lightMagenta
	call SetTextColor
	call WriteString
	call CrLf
	exit
main ENDP
end main