; PaymentPage.inc
; Some code in this file depends on kernel32.lib, which is included in main.asm before including this file, so this file has access to kernel32.lib when main.asm is run.

SetConsoleOutputCP PROTO :DWORD
PrintPaymentQR PROTO
PaymentPage PROTO

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
fileHandle2     dword    ?
consoleHandle  dword    ?

failOpenFileMsg byte "Could not open file: ", 0

headerPayment byte "Time to Pay Up!",0
promptPayment byte "SELECT PAYMENT METHOD:",13,10,"How would you like to pay for your ticket?",0

paymentMethod1 byte "Credit/Debit Card",0
paymentMethod2 byte "Touch 'n Go",0
paymentMethods dword offset paymentMethod1, offset paymentMethod2

promptCardDetails byte "Please enter your card details.",0
tipCancel byte "[Hint: Enter 'c' in any field to cancel and go back to the Payment page.]",0
cancelStr byte "c",0

askCardNo byte "Credit No. (e.g. 1234567890123456): ",0
askExpireDate byte "Expiration date (MM/YY): ",0
askCvv byte "CVV/CVC: ",0

errorInvalid byte "Invalid format. Please try again.",0

MIN_CARD_NO = 13
MAX_CARD_NO = 19
MAX_EXPIRE = 5
MAX_CVV = 3
inputCardNo byte MAX_CARD_NO+1 dup(?)
inputExpire byte MAX_EXPIRE+1 dup(?)
inputCvv byte MAX_CVV+1 dup(?)


.code
; main PROC
    ; call PrintPaymentQR
    ; exit

; main ENDP

PaymentPage PROC
start:
    INVOKE InitMenu, offset headerPayment, offset paymentMethods, lengthof paymentMethods, offset promptPayment, 0, 0, 0
    
    cmp eax, 0
    je CreditDebit

    cmp eax, 1
    ; je TnG

    jmp start

CreditDebit:
    call ClrScr
    lea edx, promptCardDetails
    call CrLf
    call CrLf

    mov eax, yellow
    call SetTextColor
    lea edx, tipCancel
    call WriteString
    mov eax, NORMAL_COLOR
    call SetTextColor

    call CrLf
    call CrLf

GetCardNo:
    lea edx, askCardNo
    call WriteString

    lea edx, inputCardNo
    mov ecx, MAX_CARD_NO
    call ReadString

    ; Validate card no.
    call StrLength
    ; Check if user input 'c', which is for cancelling
    INVOKE Str_compare, edx, ADDR cancelStr ; edx now holds the offset to the input string
    je start
    ; card no. is invalid if shorter than minimum length
    cmp eax, MIN_CARD_NO
    jl invalidCardNo
    ; invalid if not all digits
    mov ecx, eax
    mov esi, edx
    CheckDigit:
        mov al, byte ptr [esi]
        call IsDigit
        jnz invalidCardNo
        inc esi
        loop CheckDigit
    
    jmp GetExpire

invalidCardNo:
    lea edx, errorInvalid
    call WriteStringInRed
    call CrLf
    call CrLf
    jmp GetCardNo

GetExpire:
    lea edx, askExpireDate
    call WriteString

    lea edx, inputExpire
    mov ecx, MAX_EXPIRE
    call ReadString

    call StrLength
    cmp eax, MIN_CARD_NO
    jl invalidCardNo
    INVOKE Str_compare, edx, ADDR cancelStr ; edx now holds the offset to the input string
    je start
    jmp GetExpire
    
    

PaymentPage ENDP

CheckIfCancel PROC
    
CheckIfCancel ENDP


PrintPaymentQR PROC
    ; Set console to UTF-8 (code page 65001) to print the characters that make up the ASCII qr code
    invoke SetConsoleOutputCP, 65001

    ; Open file for reading
    invoke CreateFileA, addr filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
    mov fileHandle2, eax
    cmp eax, INVALID_HANDLE_VALUE
    je file_failed

    ; Read file
    invoke ReadFile, fileHandle2, addr buffer, BUFSIZE, addr bytesRead, 0

    ; Get console handle
    invoke GetStdHandle, STD_OUTPUT_HANDLE
    mov consoleHandle, eax

    ; Write to console
    invoke WriteFile, consoleHandle, addr buffer, bytesRead, addr bytesWritten, 0

    ; Clean up
    invoke CloseHandle, fileHandle2
    call WaitMsg
    ret

file_failed:
    mov edx, offset failOpenFileMsg
    call WriteString
    mov edx, offset filename
    call WriteString
    call CrLf
    ret
PrintPaymentQR ENDP



; INCLUDE Irvine32.inc

; .data
; qrcodefilename	byte	"qrcode.txt"
; fileHandle2		dword	?
; BUFSIZE			=	5000
; buffer			byte	BUFSIZE dup(?)
; bytesRead		dword	?

; errorMsg		byte	"Error reading file.",0

; .code
; main PROC
	; mov edx, offset qrcodefilename
	; call OpenInputFile
	; mov fileHandle2, eax

	; mov eax, fileHandle2
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