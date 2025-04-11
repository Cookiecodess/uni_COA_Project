; INCLUDE generalFunctions.inc

PrintTicketInfo PROTO
TimedPassConfmMenu PROTO
DrawSingleJrnyStationSelectionMenu PROTO
DrawSingleJrnyConfirmationMenu PROTO
GetInputForDestStnPage PROTO



.data
	; SPACE = 32
	;GENERAL------------------------------------------------------------------
	; MAX	= 20								; max characters to read
	; inputBuffer			byte  MAX+1 dup(?)  ; room for null character

	;TICKETING--------------------------------------------------------------------
	headerBookTicket	BYTE	"Book ticket",0

	; The byte arrays stored in this dword array are all defined in globalData.inc
	ticketTypeOptions		DWORD	OFFSET tcktSingleJourney, OFFSET tcktWeeklyPass, OFFSET tcktMonthlyPass, OFFSET backOption
	backIndex			DWORD   3		; note that this is an index. Index 3 = 4th element
	ticketTypeOptionsCount	DWORD	?
	promptChoose	BYTE	"Please select a ticket for more options.",0
	promptLocOrigin	BYTE	"SELECT ORIGIN: Where would you like to aboard the train?",0
	promptLocDest	BYTE	"SELECT DESTINATION: Where would you like to get off?",0

	; Option list for confirmation page
	optionCfmPur	BYTE	"Confirm purchase",0
	optionCancel	BYTE	"Cancel",0
	confirmationOptions	DWORD	OFFSET optionCfmPur, OFFSET optionCancel

	msgChose			BYTE	"You chose ",0
	
	

	; COLORS
	ORIGIN_COLOR = lightCyan
	DEST_COLOR = lightGreen


.code 

; return EAX: -1 means redraw customer menu after returning
TicketingPage PROC USES ebx edx
	start:
		; clear boughtTicket struct
		mov ecx, sizeof boughtTicket / 4
		lea edi, boughtTicket
		xor eax, eax						; EAX = 0 = the dword we want to store
		rep stosd

		call ClearStationMapStates

		; Ticket type menu
		invoke InitMenu, offset headerBookTicket, offset ticketTypeOptions, lengthof ticketTypeOptions, offset promptChoose, 0, 0

		; Did user select "Back"? If so, let's not print anything and just return.
		cmp eax, backIndex
		je return_and_redraw_cust_menu

		; User selected a ticket. Record their username
		mov boughtTicket.custName, OFFSET cUsername 
		
		; Actions for each selection
		cmp eax, SINGLE_JOURNEY		; Single journey ticket chosen
		je SelectSingleJourney
		cmp eax, WEEKLY_PASS		; Weekly pass selected
		je SelectWeeklyPass
		cmp eax, MONTHLY_PASS		; Weekly pass selected
		je SelectMonthlyPass
		
		; User should NEVER reach this point. If they do, maybe you accidentally added an option to the menu but forgot to account for it here.
		mov edx, offset errWeird
		call WriteString
		call WaitMsg
		jmp start

	SelectSingleJourney:
		mov edx, ticketTypes[SINGLE_JOURNEY * 4]
		mov boughtTicket.ticketType, edx
		jmp ContinueForSingleJourney
	SelectWeeklyPass:
		mov edx, ticketTypes[WEEKLY_PASS * 4]
		mov boughtTicket.ticketType, edx
		mov edx, ticketPrices[WEEKLY_PASS * 4]
		mov boughtTicket.price, edx
		jmp ContinueForLimitedTimePass
	SelectMonthlyPass:
		mov edx, ticketTypes[MONTHLY_PASS * 4]
		mov boughtTicket.ticketType, edx
		mov edx, ticketPrices[MONTHLY_PASS * 4]
		mov boughtTicket.price, edx
		jmp ContinueForLimitedTimePass
	
	ContinueForSingleJourney:
		; Print menu to let user choose origin station
		invoke InitMenu, offset headerBookTicket, offset locations, lengthof locations, offset promptLocOrigin, offset DrawSingleJrnyStationSelectionMenu, 0


		; EAX = index of station
		mov originStnIdx, eax
		mov boughtTicket.originStnIdx, eax
		; Record station name (offset of the string) in boughtTicket.originStn
		mov edx, locations[eax*4]
		mov boughtTicket.originStn, edx
		; Set bit 0 of stationMenuState flag, to indicate that ORIGIN has been selected
		or stationMenuState, 00000001b

		; Print menu to let user choose dest station. NOT PRINTING or MUTED-COLOR-HIGHLIGHT the origin station based on EAX
		invoke InitMenu, offset headerBookTicket, offset locations, lengthof locations, offset promptLocDest, offset DrawSingleJrnyStationSelectionMenu, offset GetInputForDestStnPage 

		mov destStnIdx, eax
		mov boughtTicket.destStnIdx, eax
		; Record station name (offset of the string) in boughtTicket.destStn based on EAX
		mov edx, locations[eax*4]
		mov boughtTicket.destStn, edx
		; Record ticket price
		push boughtTicket.originStnIdx
		push boughtTicket.destStnIdx
		call CalculateFeeNew
		mov boughtTicket.price, eax

		; Set bit 1 of stationMenuState flag, to indicate that DESTINATION has been selected
		or stationMenuState, 00000010b

		; confirmation page with all ticket details and a route map
		invoke InitMenu, offset headerBookTicket, offset confirmationOptions, lengthof confirmationOptions, 0, offset DrawSingleJrnyConfirmationMenu, 0

		
		
		test eax, eax		; EAX = 0 --> Selected "Confirm purchase"
		jz return_but_proceed

		; Else, user selected "Cancel", so go back to ticket type menu.
		jmp BackToTicketTypeMenu 

	ContinueForLimitedTimePass:
		invoke InitMenu, offset headerBookTicket, offset confirmationOptions, lengthof confirmationOptions, 0, offset TimedPassConfmMenu, 0


		test eax, eax		; EAX = 0 --> Selected "Confirm purchase"
		jz return_but_proceed

		; Else, user selected "Cancel", so go back to ticket type menu.
		jmp BackToTicketTypeMenu 
	
	BackToTicketTypeMenu:
		; go back to Ticket Type menu.
		jmp start

		; mov edx, offset msgChose
		; call WriteString
		;Write the string from the array
		; mov edx, offset ticketTypeOptions
		; mov eax, currentSelection
		; call GetStrArrElem
		; call WriteString
		
		; exit	; TEMP. TO-DO: Implement actions for each selection.
	
	return_and_redraw_cust_menu:
		mov eax, -1		; EAX = 0 acts as a signal to redraw customer menu after returning, instead of proceed to receipt module.
	return_but_proceed:
		ret
TicketingPage ENDP



; Prints out information contained in the TicketStruct struct variable boughtTicket.
; Excluding null values.
PrintTicketInfo PROC USES eax ecx edx esi 
		; Print customer name
		mov al, TAB
		call WriteChar
		mov edx, TicketStructKeys[0*4]
		call WriteString
		mov al, TAB
		call WriteChar
		call WriteChar
		mov edx, boughtTicket.custName
		test edx, edx
		jz SkipPrintingName
		call WriteString
	SkipPrintingName:
		call CrLf
	

		; Print ticket type
		mov al, TAB
		call WriteChar
		mov edx, TicketStructKeys[1*4]
		call WriteString
		mov al, TAB
		call WriteChar
		call WriteChar
		mov edx, boughtTicket.ticketType
		test edx, edx
		jz SkipPrintingType
		call WriteString
	SkipPrintingType:
		call CrLf


		; Print price
		mov al, TAB
		call WriteChar
		mov edx, TicketStructKeys[2*4]
		call WriteString
		mov al, TAB
		call WriteChar
		call WriteChar
		call WriteChar
		mov edx, offset RINGGIT
		call WriteString
		mov eax, boughtTicket.price
		call WriteDec
		call CrLf


		; IF boughtTicket.originStn NOT NULL, print it
		mov eax, boughtTicket.originStn
		test eax, eax
		jz SkipPrintingStations
		
		mov al, TAB
		call WriteChar
		mov edx, TicketStructKeys[3*4]
		call WriteString
		mov al, TAB
		call WriteChar
		call WriteChar
		mov edx, boughtTicket.originStn
		call WriteString
		call CrLf

		; IF boughtTicket.destStn NOT NULL, print it
		mov eax, boughtTicket.destStn
		test eax, eax
		jz SkipPrintingStations
		
		mov al, TAB
		call WriteChar
		mov edx, TicketStructKeys[4*4]
		call WriteString
		mov al, TAB
		call WriteChar
		mov edx, boughtTicket.destStn
		call WriteString

	SkipPrintingStations:
		call CrLf


		ret
PrintTicketInfo ENDP


TimedPassConfmMenu PROC USES eax ebx ecx edx esi
	; Clear screen
        call Clrscr

        ; Print header
        mov eax, globalHeaderOffset
        mov ebx, globalHeaderLength
        call PrintHeader
        call CrLf

		; Print ticket info
		call PrintTicketInfo

        ; Print selection prompt
        ; mov edx, globalSelectionPromptOffset
        ; call WriteString
        ; call CrLf
        ; call CrLf

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
TimedPassConfmMenu ENDP







DrawSingleJrnyStationSelectionMenu PROC USES eax ebx ecx edx esi
	; Clear screen
        call Clrscr

        ; Print header
        mov eax, globalHeaderOffset
        mov ebx, globalHeaderLength
        call PrintHeader
        call CrLf

		; Print ticket info
		call PrintTicketInfo

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

		; Draw station map menu based on current selection
        invoke DrawStationSelectionMap, globalOptionListOffset

        ret	
DrawSingleJrnyStationSelectionMenu ENDP



DrawSingleJrnyConfirmationMenu PROC USES eax ebx ecx edx esi
	; Clear screen
        call Clrscr

        ; Print header
        mov eax, globalHeaderOffset
        mov ebx, globalHeaderLength
        call PrintHeader
        call CrLf

		; Print ticket information
		call PrintTicketInfo
		call CrLf
		call CrLf

		; Display graphical station map that highlights the ORIGIN and DESTINATION stations
		invoke DisplayStationMapWithHighlighting, originStnIdx, destStnIdx
		call CrLf
		call CrLf

        ; Print selection prompt
        ; mov edx, globalSelectionPromptOffset
        ; call WriteString
        ; call CrLf
        ; call CrLf

        ; Print error message IF errorCode != 0
		; WARNING: EDX is not 0 nor any meaningful value at this point, and thus will cause illegal memory access, as PrintErrorMessage tries to find a string at location EDX if EDX != 0. Not sure why the other instances of this DrawMenu function that also have this line doesn't cause an illegal memory access too. Perhaps need to look into this in future.
        ; call PrintErrorMessage

        ; Reset text color
        mov eax, NORMAL_COLOR
        call SetTextColor

        ; Prepare to draw menu items
        mov ecx, 0                      ; Loop counter
        mov esi, globalOptionListOffset ; Start of menu items

    DrawMenuLoop:
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
DrawSingleJrnyConfirmationMenu ENDP


GetInputForDestStnPage PROC
        push ebp
        mov ebp, esp
        
    ; Input handling loop
    InputLoop:
        ; Get keyboard input
        call ReadChar

        ; Check for specific keys
        .IF al == 'k' || al == 'K' || ax == UP          ; Move up
				mov eax, currentSelection
				dec eax
				cmp eax, originStnIdx
				jne no_need_to_skip_over1
				dec eax		; skip over disabled option
			no_need_to_skip_over1:
				cmp eax, 0
				jge no_wrapping_to_bottom
				add eax, totalOptions		; wrap to bottom
			no_wrapping_to_bottom:
				mov currentSelection, eax
        .ELSEIF al == 'j' || al == 'J' || ax == DOWN    ; Move down
				mov eax, currentSelection
				inc eax
				cmp eax, originStnIdx
				jne no_need_to_skip_over2
				inc eax		; skip over disabled option
			no_need_to_skip_over2:
				cmp eax, totalOptions		; wrap to top
				jl no_wrapping_to_top
				xor eax, eax
			no_wrapping_to_top:
				mov currentSelection, eax
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
GetInputForDestStnPage ENDP




