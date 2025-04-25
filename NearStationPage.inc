; NearStationPage.inc - Final Version
.data
    ; Strings
    headerNearStation      BYTE    "=== Nearby Stations ===",0
    promptSelectStation    BYTE    "Please Select a Station:",0
    msgPickedLocation      BYTE    "Station Selected: ",0
    transportHeader        BYTE    "Available Nearby Transport:",0
    backOption2            BYTE    "Back",0

    ; Transport options
    penangTransport        BYTE    "Airport (2.4 km), LRT (0.3 km)",0
    perakTransport         BYTE    "LRT (0.2 km), MRT (0.4 km)",0
    klTransport            BYTE    "LRT (0.1 km), MRT (0.3 km), Airport (3.1 km)",0
    melakaTransport        BYTE    "LRT (0.3 km)",0
    johorTransport         BYTE    "MRT (0.3 km)",0

    ; State tracking
    currentSelection2       DWORD   0
    returningFromSelection BYTE    0

    ; Menu options
    stationOptions         DWORD   OFFSET location1, OFFSET location2, 
                                OFFSET location3, OFFSET location4, 
                                OFFSET location5, OFFSET backOption2
    totalStations          DWORD   6
    backIndex2             DWORD   5

    ; Colors
    DEF_COLOR = white + (black SHL 4)
    SEL_COLOR = yellow + (black SHL 4)

.code
; Keyboard constants
UP_KEY = 048h
DOWN_KEY = 050h
ENTER_KEY = 1Ch

;-----------------------------------------------------------
ResetTextColor PROC
    mov eax, DEF_COLOR
    call SetTextColor
    ret
ResetTextColor ENDP

;-----------------------------------------------------------
ShowStationInfo PROC USES eax edx esi
    call ResetTextColor
    call Clrscr
    
    ; Header
    mov edx, OFFSET headerNearStation
    call WriteString
    call Crlf
    call Crlf
    
    ; Selected station
    mov edx, OFFSET msgPickedLocation
    call WriteString
    mov esi, currentSelection2
    mov edx, stationOptions[esi*4]
    call WriteString
    call Crlf
    call Crlf
    
    ; Transport header
    mov edx, OFFSET transportHeader
    call WriteString
    call Crlf
    
    ; Show specific transport options
    mov esi, currentSelection2
    cmp esi, 0      ; Penang
    jne @F
    mov edx, OFFSET penangTransport
    call WriteString
    jmp ShowBack
@@:
    cmp esi, 1      ; Perak
    jne @F
    mov edx, OFFSET perakTransport
    call WriteString
    jmp ShowBack
@@:
    cmp esi, 2      ; KL
    jne @F
    mov edx, OFFSET klTransport
    call WriteString
    jmp ShowBack
@@:
    cmp esi, 3      ; Melaka
    jne @F
    mov edx, OFFSET melakaTransport
    call WriteString
    jmp ShowBack
@@:                 ; Johor
    mov edx, OFFSET johorTransport
    call WriteString

ShowBack:
    call Crlf
    call Crlf
    
    ; Back option with highlight
    mov eax, SEL_COLOR
    call SetTextColor
    mov edx, OFFSET backOption2
    call WriteString
    
    ; Wait for Enter key only
WaitForEnter:
    call ReadChar
    cmp ah, ENTER_KEY
    jne WaitForEnter
    
    ret
ShowStationInfo ENDP

;-----------------------------------------------------------
DrawStationMenu PROC USES eax ecx edx esi
    call ResetTextColor
    call Clrscr
    
    ; Header
    mov edx, OFFSET headerNearStation
    call WriteString
    call Crlf
    call Crlf
    
    ; Prompt
    mov edx, OFFSET promptSelectStation
    call WriteString
    call Crlf
    call Crlf
    
    ; Menu items
    mov ecx, 0
    mov esi, OFFSET stationOptions
    
MenuLoop:
    ; Highlight selection
    mov eax, DEF_COLOR
    cmp ecx, currentSelection2
    jne @F
    mov eax, SEL_COLOR
    @@:
    call SetTextColor
    
    ; Print item
    mov al, ' '
    call WriteChar
    mov edx, [esi]
    call WriteString
    call Crlf
    
    ; Next item
    add esi, 4
    inc ecx
    cmp ecx, totalStations
    jb MenuLoop
    
    ret
DrawStationMenu ENDP

;-----------------------------------------------------------
NearStationPage PROC USES ebx ecx edx esi edi
    ; Initialize
    call ResetTextColor
    mov currentSelection2, 0
    mov returningFromSelection, 0
    
MainLoop:
    call DrawStationMenu
    
InputLoop:
    call ReadChar
    
    ; Handle navigation
    .IF ah == UP_KEY
        dec currentSelection2
        cmp currentSelection2, -1
        jne @F
        mov eax, totalStations
        dec eax
        mov currentSelection2, eax
    .ELSEIF ah == DOWN_KEY
        inc currentSelection2
        mov eax, currentSelection2
        cmp eax, totalStations
        jl @F
        mov currentSelection2, 0
    .ELSEIF ah == ENTER_KEY
        jmp ProcessSelection
    .ENDIF
    
    @@: ; Refresh
    jmp MainLoop
    
ProcessSelection:
    ; Handle Back selection
    mov eax, currentSelection2
    cmp eax, backIndex2
    je ExitToPrevious
    
    ; Show station info
    call ShowStationInfo
    jmp MainLoop
    
ExitToPrevious:
    call ResetTextColor
    mov eax, -1
    ret
NearStationPage ENDP