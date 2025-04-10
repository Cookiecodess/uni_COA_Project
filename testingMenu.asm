TITLE Interactive Menu Navigation     (MenuNavigation.asm)

; Win32 MASM implementation with Irvine32 library
; Interactive menu navigation for ticket system

INCLUDE Irvine32.inc
INCLUDE generalFunctions.inc

.data
    header     BYTE "Just a little header :)",0
    ; Menu items
    menu_items BYTE "Single Journey Ticket", 0
               BYTE "Day Pass", 0
               BYTE "Weekly Pass", 0
               BYTE "Monthly Pass", 0
               BYTE "Concession Ticket", 0

    ; Menu navigation variables
    current_selection DWORD 0       ; Currently selected menu item
    total_items DWORD 5             ; Total number of menu items

    ; Color constants
    NORMAL_COLOR = lightGray
    HIGHLIGHT_COLOR = white

    ; Test
    msgUpPressed    BYTE    "Up pressed",0
    msgDownPressed    BYTE    "Down pressed",0

.code
main PROC

    call ReadChar
    call DumpRegs
    call ReadChar
    call DumpRegs
    call ReadChar
    call DumpRegs
    call ReadChar
    call DumpRegs

TestLoop:
    call ReadChar
    cmp ax, 4800h
    je Up
    cmp ax, 5000h
    je Down
Up:
    mov edx, offset msgUpPressed
    call WriteString
    jmp TestLoop
Down:
    mov edx, offset msgDownPressed
    call WriteString
    jmp TestLoop
    

    ; Initialize console
    call Clrscr

    ; Initial menu display
    call DrawMenu

    ; Input handling loop
InputLoop:
    ; Get keyboard input
    call ReadChar

    ; Check for specific keys
    .IF al == 'k' || al == 'K'      ; Move up
        call MoveCursorUp
    .ELSEIF al == 'j' || al == 'J'  ; Move down
        call MoveCursorDown
    .ELSEIF al == 13                ; Enter key
        call SelectItem
    .ELSEIF al == 'q' || al == 'Q'  ; Quit
        jmp ExitProgram
    .ENDIF

    ; Redraw menu after potential changes
    call DrawMenu

    jmp InputLoop

;---------------------------------------------
; ExitProgram Procedure
; Cleanly exits the program
;---------------------------------------------
ExitProgram:
    exit
main ENDP

;---------------------------------------------
; DrawMenu Procedure
; Draws the entire menu with current selection highlighted
;---------------------------------------------
DrawMenu PROC USES eax ebx ecx edx esi
    ; Clear screen
    call Clrscr

    ; Print header
    lea eax, header
    mov ebx, lengthof header
    call PrintHeader
    call CrLf

    ; Reset text color
    mov eax, NORMAL_COLOR
    call SetTextColor

    ; Prepare to draw menu items
    mov ecx, 0                  ; Loop counter
    mov esi, OFFSET menu_items  ; Start of menu items

DrawMenuLoop:
    ; Position cursor
    ;mov dh, cl                  ; Row = counter
    ;mov dl, 0                   ; Column 0
    ;call Gotoxy

    ; Compare current item with current selection
    .IF ecx == current_selection
        ; Highlight selected item
        mov eax, HIGHLIGHT_COLOR
        call SetTextColor
        
        ; Draw cursor indicator
        mov al, '>'
        call WriteChar
        mov al, ' '
        call WriteChar
    .ELSE
        ; Normal color for non-selected items
        mov eax, NORMAL_COLOR
        call SetTextColor
        
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
    .IF ecx < total_items
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
    .IF current_selection > 0
        dec current_selection
    .ENDIF
    ret
MoveCursorUp ENDP

;---------------------------------------------
; MoveCursorDown Procedure
; Moves cursor down, preventing going below last item
;---------------------------------------------
MoveCursorDown PROC
    ; Check if already at bottom
    mov eax, current_selection
    inc eax
    .IF eax < total_items
        inc current_selection
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



END main