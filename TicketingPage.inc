; INCLUDE generalFunctions.inc



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
	
	boughtTicket TicketStruct <>

	; State flag for the menu for booking single journey tickets
	menuState			BYTE	0		; Bit 1 set: origin station selected
										; Bit 2 set: dest station selected
										; Bit 3 of menuState ensures that the setting of currentSelection to 1 only happens on the first draw of this menu. Bit 3 is set on first redraw.
	originStnIdx		DWORD	0
	destStnIdx		DWORD	0

	; COLORS
	ORIGIN_COLOR = lightCyan
	DEST_COLOR = lightGreen


.code 
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



CustomMenuBookSingleJourney PROC USES eax ebx ecx edx esi
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

        ; Prepare to draw menu items
        mov ecx, 0                      ; Loop counter
        mov esi, globalOptionListOffset ; Start of menu items

		; If first item has been selected as ORIGIN, set the currentSelection to 1 (index 1 = 2nd element)
		test menuState, 00000001b
		jz DrawMenuLoop		; If ORIGIN has NOT been selected, then ignore this part and proceed to the DrawMenuLoop.
		cmp originStnIdx, 0	; If the ORIGIN is NOT the first item, also ignore this section.
		jne DrawMenuLoop
		test menuState, 00000100b	; Bit 3 of menuState ensures that the setting of currentSelection to 1 only happens on the first draw of this menu.
		jnz DrawMenuLoop
		mov currentSelection, 1
		or menuState, 00000100b

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
			test menuState, 00000001b	; Check if ORIGIN station has been selected
			jz SetNormalColor
			cmp ecx, originStnIdx		; Check if the item currently being printed is the ORIGIN
			je SetColorForOriginStn

		SetNormalColor:
            ; Normal color for non-selected (but selectable) items
            mov eax, NORMAL_COLOR
            call SetTextColor
			jmp End_SetColorForOriginStn
        
		SetColorForOriginStn:
			; If current item being printed has been selected as ORIGIN, set it to a color that indicates it cannot be selected.
			mov eax, ORIGIN_COLOR
            call SetTextColor
		End_SetColorForOriginStn:

            ; Print tab
            mov al, TAB
            call WriteChar

            ; Spacing for non-selected items
            mov al, ' '
            call WriteChar
            call WriteChar			
        .ENDIF

		; Draw location box
		mov al, '['
		call WriteChar
		; If user has selected an origin station...
		test menuState, 00000001b
		jz PrintBlankBox
		; ...and current item being printed is the origin station, print "*"
		cmp ecx, originStnIdx
		jne IsItDest	; if no, check if it's the destination station
		mov al, '*'
		call WriteChar
		jmp EndPrintBox

	IsItDest:
		; If user has selected a destination station...
		test menuState, 00000010b
		jz PrintBlankBox
		; ...and current item being printed is the destination station, print "X"
		cmp ecx, destStnIdx
		jne PrintBlankBox	; if no, just print a space inside the box.
		mov al, 'X'
		call WriteChar
		jmp EndPrintBox

	PrintBlankBox:
		mov al, ' '
		call WriteChar
	EndPrintBox:
		mov al, ']'
		call WriteChar

		; Space between the box and the station name
		mov al, ' '
		call WriteChar

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
        je done 

		; Set color for the vertical line
        mov eax, DISABLED_COLOR		
        call SetTextColor

		; Print a vertical line between the boxes
		mov al, TAB
		call WriteChar
		mov al, ' '
        call WriteChar
        call WriteChar
        call WriteChar
		mov al, '|'
		call WriteChar
		call CrLf

		jmp DrawMenuLoop

    done:
        ; Reset text color
        mov eax, NORMAL_COLOR		
        call SetTextColor

        ret	
CustomMenuBookSingleJourney ENDP



PrintStationMapCfm PROC USES eax ebx ecx edx esi
	LOCAL symbolInsideBox:BYTE
	LOCAL specificTextColor:DWORD

		mov ecx, lengthof locations		; times to loop for = number of stations
		mov esi, 0						; initialize index
	PrintThemStations:
		mov al, TAB
		call WriteChar
		mov al, ' '
		call WriteChar
		call WriteChar

		mov ebx, destStnIdx		; because in comparisons at least one's gotta be a register
		; ORIGIN station
		.IF esi == originStnIdx
			mov symbolInsideBox, '*'
			mov specificTextColor, ORIGIN_COLOR
		; Arrow should go down
		.ELSEIF esi > originStnIdx && esi < destStnIdx
			mov symbolInsideBox, 'v'
			mov specificTextColor, NORMAL_COLOR
		; Arrow should go up
		.ELSEIF esi < originStnIdx && esi > destStnIdx 
			mov symbolInsideBox, '^'
			mov specificTextColor, NORMAL_COLOR
		; DESTINATION station
		.ELSEIF esi == ebx
			mov symbolInsideBox, 'X'
			mov specificTextColor, DEST_COLOR

	; IGNORE THIS SECTION... HAHA... NOTHING TO SEE HERE!
		jmp EndTeleport

	TeleportLmao:
		loop PrintThemStations
	EndTeleport:
	; YUP... KEEP ON EXECUTING, COMPILER! NOTHING OUT OF THE BLUE HERE...

		; Special case for stations not included in the route
		.ELSE
			mov eax, DISABLED_COLOR
			call SetTextColor


			mov al, '['
			call WriteChar
			mov al, ' '
			call WriteChar
			mov al, ']'
			call WriteChar
			mov al, ' '
			call WriteChar

			; Print station name
			mov edx, dword ptr locations[esi*4]
			call WriteString
			call CrLf
			jmp PrintVertLine
		.ENDIF

		

		; Print the box thingy ('[' and ']' have normal color, while the symbol inside has special color)
		mov eax, NORMAL_COLOR
		call SetTextColor
		mov al, '['
		call WriteChar

		mov eax, specificTextColor
		call SetTextColor
		mov al, symbolInsideBox
		call WriteChar

		mov eax, NORMAL_COLOR
		call SetTextColor
		mov al, ']'
		call WriteChar
		mov al, ' '
		call WriteChar

		; Print station name 
		mov eax, specificTextColor
		call SetTextColor
		mov edx, dword ptr locations[esi*4]
		call WriteString	
		call CrLf


	PrintVertLine:
		; Increase index so it points to the next item
		inc esi

		; if tihs is the last item, dont print vert line, skip this part.
		cmp esi, lengthof locations
		je donePrinting

		; is this vert line on the route? 
		; NOTE: After printing element 0, esi is 1; after printing element 1, esi is 2, and so on.
		.IF (esi > originStnIdx && esi <= destStnIdx) || (esi <= originStnIdx && esi > destStnIdx)
			mov eax, NORMAL_COLOR
		.ELSE
			mov eax, DISABLED_COLOR
		.ENDIF
		call SetTextColor

		mov al, TAB
		call WriteChar
		mov al, ' '
		call WriteChar
		call WriteChar
		call WriteChar
		mov al, '|'
		call WriteChar
		call CrLf

		jmp TeleportLmao

	donePrinting:
		ret
PrintStationMapCfm ENDP



SingleJrnyConfmMenu PROC USES eax ebx ecx edx esi
	; Clear screen
        call Clrscr

        ; Print header
        mov eax, globalHeaderOffset
        mov ebx, globalHeaderLength
        call PrintHeader
        call CrLf

		call PrintTicketInfo
		call CrLf
		call CrLf

		call PrintStationMapCfm
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
SingleJrnyConfmMenu ENDP


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



; return EAX: -1 means redraw customer menu after returning
TicketingPage PROC USES ebx edx
	start:
		; clear boughtTicket struct
		mov ecx, sizeof boughtTicket / 4
		lea edi, boughtTicket
		xor eax, eax						; EAX = 0 = the dword we want to store
		rep stosd

		; clear menu state flag
		mov menuState, 0
		
		; clear origin and destination station selection
		mov originStnIdx, 0
		mov destStnIdx, 0

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
		; print menu letting user choose origin station
		invoke InitMenu, offset headerBookTicket, offset locations, lengthof locations, offset promptLocOrigin, offset CustomMenuBookSingleJourney, 0


		; EAX = index of station
		mov originStnIdx, eax
		; Record station name (offset of the string) in boughtTicket.originStn
		mov edx, locations[eax*4]
		mov boughtTicket.originStn, edx
		; Set bit 1 of menuState flag, to indicate that ORIGIN has been selected
		or menuState, 00000001b

		; print menu letting user choose dest station. NOT PRINTING or MUTED-COLOR-HIGHLIGHT the origin station based on EAX
		; (might need a custom getInput func to handle skipping over the origin station during menu navigation)
		invoke InitMenu, offset headerBookTicket, offset locations, lengthof locations, offset promptLocDest, offset CustomMenuBookSingleJourney, offset GetInputForDestStnPage 

		mov destStnIdx, eax
		; Record station name (offset of the string) in boughtTicket.destStn based on EAX
		mov edx, locations[eax*4]
		mov boughtTicket.destStn, edx
		; Set bit 1 of menuState flag, to indicate that DESTINATION has been selected
		or menuState, 00000010b

		; confirmation page with all ticket details and a route map
		invoke InitMenu, offset headerBookTicket, offset confirmationOptions, lengthof confirmationOptions, offset promptChoose, offset SingleJrnyConfmMenu, 0

		test eax, eax		; EAX = 0 --> Selected "Confirm purchase"
		jz return_but_proceed

		; Else, user selected "Cancel", so go back to ticket type menu.
		jmp BackToTicketTypeMenu 

	ContinueForLimitedTimePass:
		invoke InitMenu, offset headerBookTicket, offset confirmationOptions, lengthof confirmationOptions, offset promptChoose, offset TimedPassConfmMenu, 0


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

