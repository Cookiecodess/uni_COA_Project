; coolMenu.inc

InitMenu PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
DrawDefaultMenu PROTO
GetInput PROTO
MoveCursorUp PROTO
MoveCursorDown PROTO
SelectItem PROTO

.data
    ; Menu navigation variables
    globalHeaderOffset DWORD ?
    globalHeaderLength DWORD ?
    globalOptionListOffset DWORD ?
    globalSelectionPromptOffset DWORD ?

    currentSelection DWORD 0       ; Currently selected menu item
    totalOptions DWORD ?             ; Total number of menu items

    ; Color constants
    NORMAL_COLOR = lightGray
    HIGHLIGHT_COLOR = yellow

    ; Arrow keys scan code (2 bytes)
    UP = 4800h
    LEFT = 4B00h
    RIGHT = 4D00h
    DOWN = 5000h

    ; Messages for testing
    msgUpPressed    BYTE    "Up pressed",13,10,0
    msgDownPressed    BYTE    "Down pressed",13,10,0
    msgLeftPressed    BYTE    "Left pressed",13,10,0
    msgRightPressed    BYTE    "Right pressed",13,10,0

.code

; Call args:
;       (Lazy to list out rn, maybe next time)
; Return arg:
;       EAX - (DWORD) index of selected option
InitMenu PROC STDCALL USES edx, headerOffset:DWORD, optionListOffset:DWORD, optionListLength:DWORD, selectionPromptOffset:DWORD, customDrawMenuFunc:DWORD, customGetInputFunc:DWORD
        LOCAL drawMenuFunc :DWORD
        LOCAL getInputFunc :DWORD

        ; Store data in global variables so they're accessible by all functions in this file
        mov eax, headerOffset
        mov globalHeaderOffset, eax

        mov eax, optionListOffset
        mov globalOptionListOffset, eax

        mov eax, selectionPromptOffset
        mov globalSelectionPromptOffset, eax

        ; Calculate string lengths
        mov edx, globalHeaderOffset
        call StrLength
        mov globalHeaderLength, eax

        ; Initialize current selection
        mov currentSelection, 0

        ; Store number of options
        mov eax, optionListLength
        mov totalOptions, eax

        ; Set draw menu function
        cmp customDrawMenuFunc, 0
        jne UseCustomMenu
        mov eax, offset DrawDefaultMenu
        jmp StoreFunc
    UseCustomMenu:
        mov eax, customDrawMenuFunc
    StoreFunc:
        mov drawMenuFunc, eax

        ; Set get input function
        cmp customGetInputFunc, 0
        jne UseCustomGetInput
        mov eax, offset GetInput
        jmp StoreGetInputFunc
    UseCustomGetInput:
        mov eax, customGetInputFunc
    StoreGetInputFunc:
        mov getInputFunc, eax

        ; Initialize console
        call Clrscr

        ; Hide cursor
        INVOKE SetCursorVisibility, 0

        ; Initial menu display
        call drawMenuFunc

        ; Get input
        push drawMenuFunc
        call getInputFunc

        ret
InitMenu ENDP

; Default function for reading input
GetInput PROC
        push ebp
        mov ebp, esp
        
    ; Input handling loop
    InputLoop:
        ; Get keyboard input
        call ReadChar

        ; Check for specific keys
        .IF al == 'k' || al == 'K' || ax == UP          ; Move up
            call MoveCursorUp
        .ELSEIF al == 'j' || al == 'J' || ax == DOWN    ; Move down
            call MoveCursorDown
        .ELSEIF al == 13                                ; Enter key
            jmp EnterPressed
        ;.ELSEIF al == 'q' || al == 'Q'                  ; Quit
        ;    jmp ExitProgram
        .ENDIF

        ; Redraw menu after potential changes
        call dword ptr [ebp+8]

        jmp InputLoop

    EnterPressed:
        ; Show cursor on return
        INVOKE SetCursorVisibility, 1
        mov eax, currentSelection     ; Returns selection index in EAX

        mov esp, ebp
        pop ebp
        ret 4
GetInput ENDP

;---------------------------------------------
; DrawDefaultMenu Procedure
; Draws the entire menu with current selection highlighted
;---------------------------------------------
DrawDefaultMenu PROC USES eax ebx ecx edx esi

        ; Clear screen
        call Clrscr

        ; Print header
        mov eax, globalHeaderOffset
        mov ebx, globalHeaderLength
        call PrintHeader
        call CrLf

        ; Print selection prompt
        mov edx, globalSelectionPromptOffset
        call WriteString
        call CrLf
        call CrLf

        ; Print error message IF errorCode != 0
        call PrintErrorMessage

        ; Reset text color
        mov eax, NORMAL_COLOR
        call SetTextColor

        ; Prepare to draw menu items
        mov ecx, 0                      ; Loop counter
        mov esi, globalOptionListOffset ; Start of menu items

    DrawMenuLoop:
        ; Position cursor
        ;mov dh, cl                  ; Row = counter
        ;mov dl, 0                   ; Column 0
        ;call Gotoxy

        ; Compare current item with current selection
        .IF ecx == currentSelection
            ; Highlight selected item
            mov eax, HIGHLIGHT_COLOR
            call SetTextColor

            ; Print tab
            mov al, TAB
            call WriteChar
        
            ; Draw cursor indicator
            mov al, '>'
            call WriteChar
            mov al, ' '
            call WriteChar
        .ELSE
            ; Normal color for non-selected items
            mov eax, NORMAL_COLOR
            call SetTextColor
        
            ; Print tab
            mov al, TAB
            call WriteChar

            ; Spacing for non-selected items
            mov al, ' '
            call WriteChar
            call WriteChar
        .ENDIF

        ; Write menu item
        mov edx, dword ptr [esi]
        call WriteString

        ; Move to next item
        call CrLf

        ; Move pointer to next string's offset address
        add esi, 4
        ; Increment loop counter
        inc ecx

        ; Check if we've drawn all items
        cmp ecx, totalOptions       ; When all items have been drawn, ecx = totalOptions
        jne DrawMenuLoop 

    done:
        ; Reset text color
        mov eax, NORMAL_COLOR
        call SetTextColor

        ret
DrawDefaultMenu ENDP

;---------------------------------------------
; MoveCursorUp Procedure
; Moves cursor up, preventing going above first item
;---------------------------------------------
MoveCursorUp PROC USES eax
        mov eax, currentSelection
        dec eax
        cmp eax, 0
        jge no_wrapping_to_bottom
        add eax, totalOptions
    no_wrapping_to_bottom:
        mov currentSelection, eax
        ret
MoveCursorUp ENDP

;---------------------------------------------
; MoveCursorDown Procedure
; Moves cursor down, preventing going below last item
;---------------------------------------------
MoveCursorDown PROC USES eax
        mov eax, currentSelection
        inc eax
        cmp eax, totalOptions
        jl no_wrapping_to_top
        xor eax, eax
    no_wrapping_to_top:
        mov currentSelection, eax
        ret
MoveCursorDown ENDP

;---------------------------------------------
; SelectItem Procedure
; Handles selection of current menu item
;---------------------------------------------
SelectItem PROC
    ; TODO: Implement specific action for selected item
    ; For now, just exit
    exit
SelectItem ENDP