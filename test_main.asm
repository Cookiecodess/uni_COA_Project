INCLUDE Irvine32.inc
INCLUDE generalFunctions.inc
INCLUDE coolMenu.inc


.data
    testHeader BYTE "Testing",0
    testList BYTE "hi",0,"haha",0,"pls work",0,"yoyo",0,0
    testPrompt BYTE "Non-coders will never understand the sheer joy and relief of seeing the words ""Build Succeeded"".",0

    msgChose BYTE "You chose ",0
.code

main PROC
    INVOKE InitMenu, offset testHeader, offset testList, offset testPrompt      ; returns selecion index in EAX
    
    ; Print selection index
    call CrLf
    call WriteDec       
    call CrLf

    ; Print "You chose <option>"
    mov edx, offset msgChose
    call WriteString

    push offset testList
    push 3
    push eax
    call GetStrArrElem
    mov edx, eax
    call WriteString
    exit

    ; TestLoop:
        ; call ReadChar
        ; cmp ax, UP
        ; je Up
        ; cmp ax, DOWN
        ; je Down
        ; cmp ax, LEFT
        ; je Left
        ; cmp ax, RIGHT
        ; je Right
    ; Up:
        ; mov edx, offset msgUpPressed
        ; call WriteString
        ; jmp TestLoop
    ; Down:
        ; mov edx, offset msgDownPressed
        ; call WriteString
        ; jmp TestLoop
    ; Left:
        ; mov edx, offset msgLeftPressed
        ; call WriteString
        ; jmp TestLoop
    ; Right:
        ; mov edx, offset msgRightPressed
        ; call WriteString
        ; jmp TestLoop

	; exit

main ENDP
end main
