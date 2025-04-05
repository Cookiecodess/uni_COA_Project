; myprocs.inc
; Example of an .inc file that contains data and custom procedures
; An .inc file can be included in an .asm file like: INCLUDE myproc.inc

.DATA
    string     byte        "Hello proc",0
MyCustomProcedure PROTO
; You can also specify parameter types if needed
; MyCustomProcedure PROTO :DWORD, :PTR BYTE

.CODE
MyCustomProcedure PROC
    ; Your procedure implementation here
    ; For example:
    mov edx, offset string
    call WriteString
    call CrLf

    mov ecx, 21
    call ReadString

    call WriteString
    ret
MyCustomProcedure ENDP