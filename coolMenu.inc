; coolMenu.inc

InitMenu PROTO STDCALL :DWORD, :DWORD, :DWORD
DrawMenu PROTO
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
;       None
; Return arg:
;       EAX - (DWORD) index of selected option
InitMenu PROC STDCALL USES edx, headerOffset:DWORD, optionListOffset:DWORD, selectionPromptOffset:DWORD

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

        ; Calculate number of options
        ; TODO: make a function that calculates number of strings in a string array
        mov totalOptions, 5

        ; Initialize console
        call Clrscr

        ; Hide cursor
        INVOKE SetCursorVisibility, 0

        ; Initial menu display
        call DrawMenu

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
            ; Show cursor on return
            INVOKE SetCursorVisibility, 1
            mov eax, currentSelection
            ret                                         ;     Returns selection index in EAX
        ;.ELSEIF al == 'q' || al == 'Q'                  ; Quit
        ;    jmp ExitProgram
        .ENDIF

        ; Redraw menu after potential changes
        call DrawMenu

        jmp InputLoop

    ; Cleanly exits the program
    ;ExitProgram:
    ;    exit
InitMenu ENDP

;---------------------------------------------
; DrawMenu Procedure
; Draws the entire menu with current selection highlighted
;---------------------------------------------
DrawMenu PROC USES eax ebx ecx edx esi

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
        mov edx, esi
        call WriteString

        ; Move to next item
        call CrLf

        ; Find null terminator of current string
    FindStringEnd:
        .IF BYTE PTR [esi] != 0
            inc esi
            jmp FindStringEnd
        .ENDIF
        inc esi    ; Move past null terminator

        ; Increment loop counter
        inc ecx

        ; Check if we've drawn all items
        .IF BYTE PTR [esi] != 0
            jmp DrawMenuLoop
        .ENDIF

        ; Reset text color
        mov eax, NORMAL_COLOR
        call SetTextColor

        ret
DrawMenu ENDP

;---------------------------------------------
; MoveCursorUp Procedure
; Moves cursor up, preventing going above first item
;---------------------------------------------
MoveCursorUp PROC
    ; Check if already at top
    .IF currentSelection > 0
        dec currentSelection
    .ENDIF
    ret
MoveCursorUp ENDP

;---------------------------------------------
; MoveCursorDown Procedure
; Moves cursor down, preventing going below last item
;---------------------------------------------
MoveCursorDown PROC USES eax
    ; Check if already at bottom
    mov eax, currentSelection
    inc eax
    .IF eax < totalOptions
        inc currentSelection
    .ENDIF
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