INCLUDE Irvine32.inc
INCLUDELIB kernel32.lib

CONSOLE_CURSOR_INFO STRUCT
	dwSize DWORD ?
	bVisible DWORD ?
CONSOLE_CURSOR_INFO ENDS

; Console screen buffer info Struct

COORD STRUCT
    X WORD ?
    Y WORD ?
COORD ENDS

SMALL_RECT STRUCT
	Left     WORD ?  ;left-most column
	Top      WORD ?  ;upper-most row
	Right    WORD ?  ;right-most column
	Bottom   WORD ?  ;lowest row
SMALL_RECT ENDS

; Seems like this struct has already been defined. Maybe by irvine32.lib, maybe by a windows-related library in Irvine folder -- Im not sure.
; CONSOLE_SCREEN_BUFFER_INFO STRUCT
	; dwSize        COORD <>
	; dwCursorPos   COORD <>
	; wAttributes   WORD ?
	; srWindow      SMALL_RECT <>
	; dwMaxWinSize  COORD <>
; CONSOLE_SCREEN_BUFFER_INFO ENDS

CHAR_INFO STRUCT
    UNION Char
        UnicodeChar WORD ?    ; WCHAR is 2 bytes (16-bit)
        AsciiChar   BYTE ?    ; CHAR is 1 byte (8-bit)
    ENDS
    Attributes WORD ?         ; WORD is 2 bytes (16-bit)
CHAR_INFO ENDS

.data
csbi CONSOLE_SCREEN_BUFFER_INFO <> ; Store the console info
scrollRect SMALL_RECT <>         ; Scroll rectangle
scrollTarget COORD <>             ; Scroll target
fill CHAR_INFO <>                ; Character fill for clearing
hConsole DWORD ?                ; Handle for the console


.code
main PROC
	; Get the handle to the console output
    invoke GetStdHandle, STD_OUTPUT_HANDLE
    mov hConsole, eax

    ; Get the console screen buffer info
    invoke GetConsoleScreenBufferInfo, hConsole, ADDR csbi
    
	; Get buffer height
	; movzx eax, csbi.dwSize.Y
	; call WriteDec

	

	mov ecx, 100
	mov eax, 0
	; add ecx, 5
	PrintStuff:
		call WriteDec
		call CrLf
		inc eax
		loop PrintStuff

	; Get the console screen buffer info
    invoke GetConsoleScreenBufferInfo, hConsole, ADDR csbi
    
	; Get buffer height
	movzx eax, csbi.dwSize.Y
	call WriteDec

	exit
		
main ENDP
end main