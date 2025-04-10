; COOL STATION MAP
; Collection of graphical station map UI components
; Used along with menu wrapper components defined in coolMenu.inc

.data
	; State flag for the menu for booking single journey tickets
	stationMenuState	BYTE	0	; bit 0 set: origin station selected
									; bit 1 set: dest station selected
									; bit 2 of stationMenuState ensures that the setting of currentSelection to 1 only happens on the first draw of this menu. bit 2 is set on first redraw.
	; indexes of selected ORIGIN and DESTINATION stations
	originStnIdx		DWORD	0
	destStnIdx			DWORD	0

.code
;=============================================================================================
; Draw a graphical menu of transit stations for user's selection
;
; Call args:
;     stationListOffset		(DWORD) - pointer to an array of station names
;
; Return args:
;     NONE
;=============================================================================================
DrawStationSelectionMap PROC USES eax ecx esi, stationListOffset:DWORD

	; Prepare to draw menu items
        mov ecx, 0                      ; Loop counter
        mov esi, stationListOffset ; Start of menu items

		; If first item has been selected as ORIGIN, set the currentSelection to 1 (index 1 = 2nd element)
		test stationMenuState, 00000001b
		jz DrawMenuLoop		; If ORIGIN has NOT been selected, then ignore this part and proceed to the DrawMenuLoop.
		cmp originStnIdx, 0	; If the ORIGIN is NOT the first item, also ignore this section.
		jne DrawMenuLoop
		test stationMenuState, 00000100b	; bit 2 of stationMenuState ensures that the setting of currentSelection to 1 only happens on the first draw of this menu.
		jnz DrawMenuLoop
		mov currentSelection, 1
		or stationMenuState, 00000100b

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
			test stationMenuState, 00000001b	; Check if ORIGIN station has been selected
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
		test stationMenuState, 00000001b
		jz PrintBlankBox
		; ...and current item being printed is the origin station, print "*"
		cmp ecx, originStnIdx
		jne IsItDest	; if no, check if it's the destination station
		mov al, '*'
		call WriteChar
		jmp EndPrintBox

	IsItDest:
		; If user has selected a destination station...
		test stationMenuState, 00000010b
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
DrawStationSelectionMap ENDP


;=============================================================================================
; Display a static map of transit stations that highlights the ORIGIN and DESTINATION stations
; based on the passed-in indexes of the (global constant) `locations` array.
;
; Call args:
;     originIdx	(DWORD) - index of origin station
;     destIdx	(DWORD) - index of destination station
;
; Return args:
;     NONE
;=============================================================================================
DisplayStationMapWithHighlighting PROC USES eax ebx ecx edx esi, originIdx:DWORD, destIdx:DWORD
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

		mov ebx, destIdx		; because in comparisons at least one's gotta be a register
		; ORIGIN station
		.IF esi == originIdx
			mov symbolInsideBox, '*'
			mov specificTextColor, ORIGIN_COLOR
		; Arrow should go down
		.ELSEIF esi > originIdx && esi < destIdx
			mov symbolInsideBox, 'v'
			mov specificTextColor, NORMAL_COLOR
		; Arrow should go up
		.ELSEIF esi < originIdx && esi > destIdx 
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
		.IF (esi > originIdx && esi <= destIdx) || (esi <= originIdx && esi > destIdx)
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
DisplayStationMapWithHighlighting ENDP


;=============================================================================================
; Clear the states of the station map.
; In other words, reset the station map to before any user selection.
;
; Call args:
;     NONE
;
; Return args:
;     NONE
;=============================================================================================
ClearStationMapStates PROC
	; clear menu state flag
	mov originStnIdx, 0

	; clear origin and destination station selection
	mov destStnIdx, 0
	mov stationMenuState, 0

	ret
ClearStationMapStates ENDP