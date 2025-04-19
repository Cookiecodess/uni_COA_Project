INCLUDE Irvine32.inc
INCLUDELIB kernel32.lib

.const
GENERIC_READ           equ 80000000h
FILE_SHARE_READ        equ 1
OPEN_EXISTING          equ 3
FILE_ATTRIBUTE_NORMAL  equ 80h
INVALID_HANDLE_VALUE   equ -1
STD_OUTPUT_HANDLE      equ -11

.data
filename       byte     "qrcode.txt", 0
BUFSIZE = 6000
buffer         byte     BUFSIZE dup(0)
bytesRead      dword    ?
bytesWritten   dword    ?
fileHandle     dword    ?
consoleHandle  dword    ?

failMsg byte "Could not open file: ", 0

SetConsoleOutputCP PROTO :DWORD
PrintPaymentQR PROTO


.code
main PROC
    call PrintPaymentQR
    exit

main ENDP

PrintPaymentQR PROC
    ; Set console to UTF-8 (code page 65001)
    invoke SetConsoleOutputCP, 65001

    ; Open file for reading
    invoke CreateFileA, addr filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
    mov fileHandle, eax
    cmp eax, INVALID_HANDLE_VALUE
    je file_failed

    ; Read file
    invoke ReadFile, fileHandle, addr buffer, BUFSIZE, addr bytesRead, 0

    ; Get console handle
    invoke GetStdHandle, STD_OUTPUT_HANDLE
    mov consoleHandle, eax

    ; Write to console
    invoke WriteFile, consoleHandle, addr buffer, bytesRead, addr bytesWritten, 0

    ; Clean up
    invoke CloseHandle, fileHandle
    call WaitMsg
    ret

file_failed:
    mov edx, offset failMsg
    call WriteString
    mov edx, offset filename
    call WriteString
    call CrLf
    ret
PrintPaymentQR ENDP

END main


; INCLUDE Irvine32.inc

; .data
; qrcodefilename	byte	"qrcode.txt"
; fileHandle		dword	?
; BUFSIZE			=	5000
; buffer			byte	BUFSIZE dup(?)
; bytesRead		dword	?

; errorMsg		byte	"Error reading file.",0

; .code
; main PROC
	; mov edx, offset qrcodefilename
	; call OpenInputFile
	; mov fileHandle, eax

	; mov eax, fileHandle
	; mov edx, offset buffer
	; mov ecx, BUFSIZE
	; call ReadFromFile
	; jc show_error_msg
	; mov bytesRead, eax
	; call WriteDec

; print_qr:
	; mov edx, offset buffer
	; call WriteString
	; jmp done

; show_error_msg:
	; mov edx, offset errorMsg
	; call WriteString
	
; done:
	
	
	; exit
; main ENDP



; end main