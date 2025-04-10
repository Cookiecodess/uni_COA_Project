INCLUDE Irvine32.inc
INCLUDE globalData.inc
INCLUDE generalFunctions.inc
INCLUDE coolMenu.inc
INCLUDE coolStationMap.inc
INCLUDE TicketingPage.inc
INCLUDE ReportPage.inc
INCLUDE ReceiptPage.inc
INCLUDE calculateProfit.inc

DrawViewScheduleStationSelectionMenu PROTO
DrawViewScheduleFinalPageMenu PROTO

.data
	headerViewSchedule			byte	"View Schedule",0
	promptViewScheduleLocOrigin byte	"Select the ORIGIN (where to get ON the transit):",0
	promptViewScheduleLocDest	byte	"Select the DESTINATION (where to get OFF the transit):",0

	optionOK					byte	"OK",0
	finalPageOptions			dword	offset optionOK

	; ignore this, this is to resolve a "undefined symbol" issue due to main.asm being excluded from the project in order to run this test.asm
	cUsername		byte	MAX+1 DUP(0)

.code
main PROC

forever:
	; Reset station map state variables
	call ClearStationMapStates

	; print menu letting user choose origin station
	invoke InitMenu, 
		offset headerViewSchedule, 
		offset locations, 
		lengthof locations, 
		offset promptViewScheduleLocOrigin, 
		offset DrawViewScheduleStationSelectionMenu, 
		0

	; EAX = index of origin station
	mov originStnIdx, eax
	; Set bit 0 of stationMenuState flag, to indicate that ORIGIN has been selected
	or stationMenuState, 00000001b

	; print menu letting user choose dest station. 
	invoke InitMenu, 
		offset headerViewSchedule, 
		offset locations, 
		lengthof locations, 
		offset promptViewScheduleLocDest, 
		offset DrawViewScheduleStationSelectionMenu, 
		offset GetInputForDestStnPage 

	; EAX = index of destination station
	mov destStnIdx, eax
	; Set bit 1 of stationMenuState flag, to indicate that DESTINATION has been selected
	or stationMenuState, 00000010b

	; confirmation page with all ticket details and a route map
	invoke InitMenu, 
		offset headerViewSchedule, 
		offset finalPageOptions, 
		lengthof finalPageOptions, 
		0, 
		offset DrawViewScheduleFinalPageMenu, 
		0

	jmp forever
	exit
main ENDP



; This is a copy of the default DrawDefaultMenu function defined in coolMenu.inc, 
; except it draws the station map menu instead of the default menu.
DrawViewScheduleStationSelectionMenu PROC USES eax ebx ecx edx esi
		; Clear screen
        call Clrscr

        ; Print header
        mov eax, globalHeaderOffset
        mov ebx, globalHeaderLength
        call PrintHeader
        call CrLf

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

DrawViewScheduleStationSelectionMenu ENDP


; The DrawMenu function for the final page where you show the schedule based on selected ORIGIN and DESTINATION, I think?
DrawViewScheduleFinalPageMenu PROC
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

		; Draw station map menu based on current selection
        invoke DisplayStationMapWithHighlighting, originStnIdx, destStnIdx

		; Newline
		call CrLf

		; ========
		; HERE you can display your schedule information, etc.
		; ========

		; Set text color to the color for focused-on option
		mov eax, HIGHLIGHT_COLOR
		call SetTextColor

		; Print "    > OK"
		call CrLf
		mov al, TAB
		call WriteChar
		mov al, '>'
		call WriteChar
		mov al, ' '
		call WriteChar
		mov edx, offset optionOK
		call WriteString

		; Reset text color to normal
		mov eax, NORMAL_COLOR
		call SetTextColor

		; Wait for Enter key press
    
		; ret will return to InitMenu, where it will proceed to call GetInput. Effectively, it will only read the Enter key.
		; Once ENTER is pressed, the program will return from GetInput to InitMenu, then return to main, and loop, showing the ORIGIN station selection menu again.
        ret	
DrawViewScheduleFinalPageMenu ENDP
END main

