; INCLUDE generalFunctions.inc



.data
	; SPACE = 32
	;GENERAL------------------------------------------------------------------
	; MAX	= 20								; max characters to read
	; inputBuffer			byte  MAX+1 dup(?)  ; room for null character

	;TICKETING--------------------------------------------------------------------
	headerNearStation	BYTE	"Nearby Stations",0

	; The byte arrays stored in this dword array are all defined in globalData.inc
	stationTypeOptions		DWORD	OFFSET location1, OFFSET location2, OFFSET location3, OFFSET location4, OFFSET location5, OFFSET backOption
	backinIndex				DWORD   6		; note that this is an index. Index 3 = 4th element

	;ticketTypeOptionsCount	DWORD	? ;???

	promptSelect	BYTE	"Please select a station for inquiry.",0


	; Option list for confirmation page
	optionCfmTest	BYTE	"Confirm test",0
	optionCancelling	BYTE	"Cancelling",0
	confirmationOptions2	DWORD	OFFSET optionCfmTest, OFFSET optionCancelling

	msgChoose			BYTE	"You chose ",0
	

	; State flag for the menu for booking single journey tickets
	menuState2			BYTE	0		; Bit 1 set: origin station selected
										; Bit 2 set: dest station selected
										; Bit 3 of menuState2 ensures that the setting of currentSelection to 1 only happens on the first draw of this menu. Bit 3 is set on first redraw.

	; COLORS
	ORIGIN_COLOR = lightCyan
	DEST_COLOR = lightGreen


.code 


TimedPassedConfmMenu PROC USES eax ebx ecx edx esi
	; Clear screen
        call Clrscr

        ; Print header
        mov eax, globalHeaderOffset
        mov ebx, globalHeaderLength
        call PrintHeader
        call CrLf


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
TimedPassedConfmMenu ENDP


NearStationPage PROC USES ebx edx
	start:

		; clear menu state flag
		mov menuState2, 0

		; Ticket type menu
		invoke InitMenu, offset headerNearStation, offset stationTypeOptions, lengthof stationTypeOptions, offset promptSelect, 0, 0

		; Did user select "Back"? If so, let's not print anything and just return.
		cmp eax, backinIndex
		je return_and_redraw_cust_menu

		
		; Actions for each selection
		cmp eax, PENANG		
		je SelectPenang
		cmp eax, PERAK		
		je SelectPerak
		cmp eax, KUALA_LUMPUR		
		je SelectKL
		cmp eax, MELAKA		
		je SelectMelaka
		cmp eax, JOHOR		
		je SelectJohor
		
		; User should NEVER reach this point. If they do, maybe you accidentally added an option to the menu but forgot to account for it here.
		mov edx, offset errWeird
		call WriteString
		call WaitMsg
		jmp start

	SelectPenang:

		jmp ContinuePenang
	SelectPerak:

		jmp ContinuePerak

	SelectKL:

		jmp ContinueKL

	SelectMelaka:

		jmp ContinueMelaka

	SelectJohor:

		jmp ContinueJohor
	
	ContinuePenang:
		invoke InitMenu, offset headerNearStation, offset confirmationOptions2, lengthof confirmationOptions2, offset promptSelect, offset TimedPassedConfmMenu, 0

		test eax, eax		; EAX = 0 --> Selected "Confirm purchase"
		jz return_but_proceed

		; Else, user selected "Cancel", so go back to ticket type menu.
		jmp BackToStationMenu 

	ContinuePerak:
		invoke InitMenu, offset headerNearStation, offset confirmationOptions2, lengthof confirmationOptions2, offset promptSelect, offset TimedPassedConfmMenu, 0

		test eax, eax		; EAX = 0 --> Selected "Confirm purchase"
		jz return_but_proceed

		; Else, user selected "Cancel", so go back to ticket type menu.
		jmp BackToStationMenu 
	ContinueKL:
		invoke InitMenu, offset headerNearStation, offset confirmationOptions2, lengthof confirmationOptions2, offset promptSelect, offset TimedPassedConfmMenu, 0

		test eax, eax		; EAX = 0 --> Selected "Confirm purchase"
		jz return_but_proceed

		; Else, user selected "Cancel", so go back to ticket type menu.
		jmp BackToStationMenu 

	ContinueMelaka:
		invoke InitMenu, offset headerNearStation, offset confirmationOptions2, lengthof confirmationOptions2, offset promptSelect, offset TimedPassedConfmMenu, 0

		test eax, eax		; EAX = 0 --> Selected "Confirm purchase"
		jz return_but_proceed

		; Else, user selected "Cancel", so go back to ticket type menu.
		jmp BackToStationMenu 

	ContinueJohor:
		invoke InitMenu, offset headerNearStation, offset confirmationOptions2, lengthof confirmationOptions2, offset promptSelect, offset TimedPassedConfmMenu, 0

		test eax, eax		; EAX = 0 --> Selected "Confirm purchase"
		jz return_but_proceed

		; Else, user selected "Cancel", so go back to ticket type menu.
		jmp BackToStationMenu 
	
	BackToStationMenu:
		; go back to Ticket Type menu.
		jmp start

		; mov edx, offset msgChoose
		; call WriteString
		;Write the string from the array
		; mov edx, offset stationTypeOptions
		; mov eax, currentSelection
		; call GetStrArrElem
		; call WriteString
		
		; exit	; TEMP. TO-DO: Implement actions for each selection.
	
	return_and_redraw_cust_menu:
		mov eax, -1		; EAX = 0 acts as a signal to redraw customer menu after returning, instead of proceed to receipt module.
	return_but_proceed:
		ret
NearStationPage ENDP

