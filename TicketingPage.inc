; INCLUDE generalFunctions.inc

PrintTicketInfo PROTO
TimedPassConfmMenu PROTO

DrawSingleJrnyStationSelectionMenu PROTO
IsDestStnOptionDisabled PROTO

DrawTimeSlotMenu PROTO
IsTimeSlotOptionDisabled PROTO

DrawSingleJrnyConfirmationMenu PROTO




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
	promptTimeSlot	BYTE	"SELECT TIME SLOT: What time would you like to leave ",0  ; will append ORIGIN station name to this string when printing the menu

	; Option list for confirmation page
	optionCfmPur	BYTE	"Confirm purchase",0
	optionCancel	BYTE	"Cancel",0
	confirmationOptions	DWORD	OFFSET optionCfmPur, OFFSET optionCancel

	strAvailableSeats	BYTE	"Available seats",0
	msgChose			BYTE	"You chose ",0

	availableSeats		DWORD	?,?,?	; Available seats for the 3 time slots for the selected ORIGIN-DESTINATION journey

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
		rep stosd	; stosd = Store String Doubleword
		mov boughtTicket.originStnIdx, -1
		mov boughtTicket.destStnIdx, -1
		mov boughtTicket.timeSlotIdx, -1

		; clear availableSeats array
		mov ecx, lengthof availableSeats
		lea edi, availableSeats
		xor eax, eax						; EAX = 0 = the dword we want to store
		rep stosd	; stosd = Store String Doubleword

		call ClearStationMapStates

		; Ticket type menu
		invoke InitMenu, offset headerBookTicket, offset ticketTypeOptions, lengthof ticketTypeOptions, offset promptChoose, 0, 0, 0

		; Did user select "Back"? If so, let's not print anything and just return.
		cmp eax, backIndex
		je return_and_redraw_cust_menu

		; Get address of username from userNameArray, using currentCustIdx
		mov esi, MAX+1
		imul esi, currentCustIdx
		add esi, OFFSET userNameArray	; EAX = &userNameArray + (currentCustIdx * (MAX+1))

		; Store address of the username in boughtTicket (a TicketStruct struct)
		mov boughtTicket.custName, esi
		
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
		; Print menu to let user choose ORIGIN station
		invoke InitMenu, offset headerBookTicket, offset locations, lengthof locations, offset promptLocOrigin, offset DrawSingleJrnyStationSelectionMenu, 0, 0


		; EAX = index of station
		mov originStnIdx, eax
		mov boughtTicket.originStnIdx, eax
		; Record station name (offset of the string) in boughtTicket.originStn
		mov edx, locations[eax*4]
		mov boughtTicket.originStn, edx
		; Set bit 0 of stationMenuState flag, to indicate that ORIGIN has been selected
		or stationMenuState, 00000001b

		; Print menu to let user choose DESTINATION station. NOT PRINTING or MUTED-COLOR-HIGHLIGHT the origin station based on EAX
		invoke InitMenu, offset headerBookTicket, offset locations, lengthof locations, offset promptLocDest, offset DrawSingleJrnyStationSelectionMenu, 0, offset isDestStnOptionDisabled

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

		; Copy available seats data of the 3 time slots for the particular ORIGIN-DESTINATION journey selected, from the three global arrays (time1, time2, time3) to the availableSeats array, for easier access in this function
		mov eax, boughtTicket.originStnIdx
		mov ebx, 4
		mul ebx
		add eax, boughtTicket.destStnIdx
			mov ebx, boughtTicket.destStnIdx
			cmp ebx, boughtTicket.originStnIdx	; compare between destStnIdx and originStnIdx
			jl NoNeedNormalizeIndex
			dec eax					; If destStnIdx > originStnIdx, need to minus EAX by 1. Here's why: 
									; for example my ORIGIN is index 1 and DEST is index 3. Each taken_seats array (time1, time2, time3) is made up of 5 groups of 4. 
									; First group is from Penang to 4 other locations, 2nd group is from Perak to other 4 locations...and so on. 
									; The element for location1-location3 is actually the 3rd (or index 2) element in that group, because it's 1-0, 1-2, 1-3, 1-4. 
									; Notice that to get 1-0, we can do 1*4+0 = 4, but to get 1-2, we can't do 1*4+2, which equals 6, when 1-2 is actually at index 5. 
									; So, when destIdx > originIdx, we need to minus the index by one. I know I probably don't make any sense and this is so hard to explain without visuals but just... yeah.
	NoNeedNormalizeIndex:
		mov esi, eax							; ESI = index to get available seats data in the time1, time2 and time3 arrays
 
			mov ebx, time1[esi * type time1]		; time1[esi] = no. of seats TAKEN for timeslot 1
			mov eax, MAX_SEATS
			sub eax, ebx			; EAX = MAX_SEATS - time1[esi] = available seats for timeslot 1
			mov dword ptr availableSeats[0 * type availableSeats], eax

			mov ebx, time2[esi * type time2]		; time2[esi] = no. of seats TAKEN for timeslot 2
			mov eax, MAX_SEATS
			sub eax, ebx			; EAX = MAX_SEATS - time2[esi] = available seats for timeslot 2
			mov dword ptr availableSeats[1 * type availableSeats], eax

			mov ebx, time3[esi * type time3]		; time3[esi] = no. of seats TAKEN for timeslot 3
			mov eax, MAX_SEATS
			sub eax, ebx			; EAX = MAX_SEATS - time3[esi] = available seats for timeslot 3
			mov dword ptr availableSeats[2 * type availableSeats], eax

		; Print TIME SLOT selection menu
		invoke InitMenu, offset headerBookTicket, offset timeSlotArray, lengthof timeSlotArray, offset promptTimeSlot, offset DrawTimeSlotMenu, 0, offset isTimeSlotOptionDisabled	; NOTE: timeSlotArray is defined in globalData.inc

		; Store selected time slot in ticket struct
		mov boughtTicket.timeSlotIdx, eax
		

		; confirmation page with all ticket details and a route map
		invoke InitMenu, offset headerBookTicket, offset confirmationOptions, lengthof confirmationOptions, 0, offset DrawSingleJrnyConfirmationMenu, 0, 0
		
		test eax, eax		; EAX = 0 --> Selected "Confirm purchase"
		jz return_but_proceed

		; Else, user selected "Cancel", so go back to ticket type menu.
		jmp BackToTicketTypeMenu 

	ContinueForLimitedTimePass:
		invoke InitMenu, offset headerBookTicket, offset confirmationOptions, lengthof confirmationOptions, 0, offset TimedPassConfmMenu, 0, 0


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
		; call WriteChar
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

		; Print time slot if it is not null
		mov eax, boughtTicket.timeSlotIdx
		cmp eax, -1
		jz SkipPrintingTimeSlot

		mov al, TAB
		call WriteChar
		mov edx, TicketStructKeys[5*4]
		call WriteString
		mov al, TAB
		call WriteChar
		call WriteChar
		mov esi, boughtTicket.timeSlotIdx
		mov edx, timeSlotArray[esi * type timeSlotArray]
		call WriteString

	SkipPrintingTimeSlot:
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






IsDestStnOptionDisabled PROC	
		cmp eax, originStnIdx
		je disabled
	no:
		mov ebx, 0
		ret
	disabled:
		mov ebx, 1
		ret
IsDestStnOptionDisabled ENDP









DrawTimeSlotMenu PROC
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

		; Display graphical station map that highlights the selected ORIGIN and DESTINATION stations
		invoke DisplayStationMapWithHighlighting, originStnIdx, destStnIdx
		call CrLf
		call CrLf

		; Print selection prompt ("SELECT TIME SLOT: When would you like to leave <ORIGIN_STN>?")
        mov edx, globalSelectionPromptOffset 
        call WriteString

		mov eax, ORIGIN_COLOR
		call SetTextColor
		mov edx, boughtTicket.originStn			; 
		call WriteString
		mov eax, NORMAL_COLOR
		call SetTextColor

		mov al, '?'
		call WriteChar
        call CrLf
        call CrLf

		mov al, TAB					; print a lot of tabs...
		call WriteChar
		call WriteChar
		call WriteChar
		call WriteChar
		call WriteChar
		mov edx, offset strAvailableSeats	; print "Available seats"
		call WriteString
		call CrLf					; newline

		mov al, TAB					; again, print a lot of tabs...
		call WriteChar
		call WriteChar
		call WriteChar
		call WriteChar
		call WriteChar
		mov ecx, lengthof strAvailableSeats
		dec ecx						; minus one because the length includes the null terminator
		mov al, '-'
	PrintLine:						; print dashes (-) for that many times to draw a horizontal line right below "Available seats"
		call WriteChar
		loop PrintLine

		call CrLf


	; Draw menu options, with a ">" at the front of the current selection

	
		; Reset text color
        mov eax, NORMAL_COLOR
        call SetTextColor

        ; Prepare to draw menu items
        mov ecx, 0                      ; Loop counter
        mov esi, globalOptionListOffset ; Start of menu items

		; If first item (time slot) is unavailable (no more seats), set the currentSelection to 1 (index 1 = 2nd element)
		cmp dword ptr availableSeats[0], 0
		jnz DrawMenuLoop		; If first time slot is available, then ignore this part and proceed to the DrawMenuLoop.
		; cmp originStnIdx, 0	; If the ORIGIN is NOT the first item, also ignore this section.
		; jne DrawMenuLoop
		test stationMenuState, 00001000b	; bit 3 of stationMenuState ensures that the setting of currentSelection to 1 only happens on the first draw of this menu.
		jnz DrawMenuLoop
		mov currentSelection, 1
		or stationMenuState, 00001000b

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
			.IF dword ptr availableSeats[ecx * type availableSeats] == 0
				; If the timeslot currently being printed has NO MORE SEATS, print it in gray
				mov eax, DISABLED_COLOR
				call SetTextColor
			.ELSE
				; Normal color for non-selected time slots (that also still has seats available)
				mov eax, NORMAL_COLOR
				call SetTextColor
			.ENDIF           
        
            ; Print tab
            mov al, TAB
            call WriteChar

            ; Spacing for non-selected items
            mov al, ' '
            call WriteChar
            call WriteChar
        .ENDIF

        ; Write time slot item
        mov edx, dword ptr [esi]
        call WriteString

		; Write available seats
		mov al, TAB
		call WriteChar
		call WriteChar
		call WriteChar
		call WriteChar
		mov eax, dword ptr availableSeats[ecx * type availableSeats]
		call WriteDec		

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
DrawTimeSlotMenu ENDP






; returns a boolean in EAX (1 for true, 0 for false)
IsTimeSlotOptionDisabled PROC 
		cmp dword ptr availableSeats[eax * type availableSeats], 0
		je disabled
	no:
		mov ebx, 0
		ret
	disabled:
		mov ebx, 1
		ret
	
IsTimeSlotOptionDisabled ENDP






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



















