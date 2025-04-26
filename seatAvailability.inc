; seatAvailability.inc
; Handles seat availability display by time slot

.data
    ; Display strings
    seatHeader       BYTE "Seat Availability",0
    timeSlotPrompt   BYTE "Select time slot:",0
    slotOptionFormat BYTE "%d) %s",0
    back3       BYTE "Back",0
    invalidSelection BYTE "Invalid selection! Please try again.",0
    toPrefix         BYTE "To ",0
    colonSpace       BYTE ": ",0

    ; Menu options array
    timeSlotOptions  DWORD OFFSET timeSlot1, OFFSET timeSlot2, OFFSET timeSlot3, OFFSET back3
    timeSlotNumbers  DWORD 1, 2, 3, 4
    getInputMsg byte "Please choose time slot to view the available seat : ",0

.code

;-------------------------------------------------------
; DisplaySeatAvailability: Main procedure to show seat availability
; Returns: EAX = -1 if user selected Back, otherwise 0
;-------------------------------------------------------
DisplaySeatAvailability PROC USES ebx ecx edx esi edi
    LOCAL selectedSlot:DWORD
    LOCAL timeArrayPtr:DWORD

menu_start:
    ; Clear screen and display header
    call Clrscr
    mov eax, OFFSET seatHeader
    mov ebx, LENGTHOF seatHeader
    call PrintHeader
    call Crlf

    ; Display time slot prompt
    mov eax, yellow + (black SHL 4)
    call SetTextColor
    mov edx, OFFSET timeSlotPrompt
    call WriteString
    mov eax, lightGray + (black SHL 4)
    call SetTextColor
    call Crlf

    ; Display menu options
    mov ecx, 4  ; 4 options (3 time slots + Back)
    mov esi, 0  ; Index counter
display_options:
    ; Display option number
    mov eax, timeSlotNumbers[esi*4]
    call WriteDec

    ; Display ") " separator
    mov al, ')'
    call WriteChar
    mov al, ' '
    call WriteChar

    ; Display time slot text
    mov edx, timeSlotOptions[esi*4]
    call WriteString
    call Crlf

    inc esi
    loop display_options

get_selection:
    mov eax, green + (black SHL 4)
    call SetTextColor
    lea edx,getInputMsg
    call writestring
    mov eax, lightGray + (black SHL 4)
    call SetTextColor

    ; Get user input
    call ReadInt
    mov selectedSlot, eax

    ; Validate input (1-4)
    cmp eax, 1
    jl invalid_input
    cmp eax, 4
    jg invalid_input

    ; Handle Back selection (option 4)
    cmp eax, 4
    je return_back

    ; Determine which time array to use (0=time1, 1=time2, 2=time3)
    dec eax
    mov timeArrayPtr, OFFSET time1
    cmp eax, 0
    je display_availability
    mov timeArrayPtr, OFFSET time2
    cmp eax, 1
    je display_availability
    mov timeArrayPtr, OFFSET time3

display_availability:
    ; Display all routes for selected time slot
    call Clrscr
    mov eax, OFFSET seatHeader
    mov ebx, LENGTHOF seatHeader
    call PrintHeader
    call Crlf

    ; Display selected time slot header
    mov eax, lightBlue + (black SHL 4)
    call SetTextColor
    mov eax, selectedSlot
    dec eax
    mov edx, timeSlotOptions[eax*4]
    call WriteString
    mov eax, lightGray + (black SHL 4)
    call SetTextColor
    call Crlf
    call Crlf

    ; Loop through all origin stations (5 total)
    mov ecx, 0  ; Origin station index (0-4)
    mov esi, timeArrayPtr  ; Pointer to time1/time2/time3 array

display_origin_loop:
    ; Display origin station name
    mov eax, yellow + (black SHL 4)
    call SetTextColor
    mov edx, locations[ecx*4]
    call WriteString
    mov eax, lightGray + (black SHL 4)
    call SetTextColor
    call Crlf

    ; Loop through all destination stations (4 per origin)
    mov edx, 0  ; Destination station index (0-4)

display_destination_loop:
    ; Skip if origin == destination
    cmp edx, ecx
    je skip_destination

    ; Display "To "
    push edx                 ; Preserve destination index
    mov edx, OFFSET toPrefix
    call WriteString

    ; Display destination name
    pop edx                  ; Restore destination index
    push edx                 ; Save it again
    mov edx, locations[edx*4] ; Get destination name
    call WriteString

    ; Display ": "
    mov edx, OFFSET colonSpace
    call WriteString

    ; Calculate array index: originIndex * 4 + (destIndex adjusted)
    mov eax, ecx             ; origin index
    mov ebx, 4
    mul ebx                  ; EAX = originIndex * 4
    pop edx                  ; Restore destination index
    push eax                 ; Save originIndex*4
    mov ebx, edx             ; EBX = destIndex
    cmp ebx, ecx
    jle no_adjust
    dec ebx                  ; Adjust if dest > origin
no_adjust:
    pop eax                  ; Restore originIndex*4
    add eax, ebx             ; EAX = final index (0-19)

    ; Calculate and display available seats (1200 - booked)
    mov ebx, [esi + eax*4]   ; Get booked seats count
    mov eax, MAX_SEATS
    sub eax, ebx
    call WriteDec
    call Crlf

skip_destination:
    inc edx
    cmp edx, 5  ; Total stations
    jl display_destination_loop

    call Crlf  ; Add extra line between origin groups
    inc ecx
    cmp ecx, 5  ; Total stations
    jl display_origin_loop

    ; Wait for user to continue
    call WaitMsg
    jmp menu_start  ; Return to time slot selection

invalid_input:
    mov edx, OFFSET invalidSelection
    call WriteString
    call Crlf
    call WaitMsg
    jmp menu_start

return_back:
    mov eax, -1  ; Signal to return to previous menu
    ret

DisplaySeatAvailability ENDP