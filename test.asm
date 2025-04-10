INCLUDE Irvine32.inc
INCLUDE globalData.inc
INCLUDE generalFunctions.inc
INCLUDE coolMenu.inc
INCLUDE TicketingPage.inc
INCLUDE ReportPage.inc
INCLUDE ReceiptPage.inc
INCLUDE calculateProfit.inc

.data
	headerViewSchedule			byte	"View Schedule",0
	promptViewScheduleLocOrigin byte	"Select the ORIGIN (where to get ON the transit):",0
	promptViewScheduleLocDest	byte	"Select the DESTINATION (where to get OFF the transit):",0

	fpOption1					byte	"OK",0
	finalPageOptions			dword	offset fpOption1

	; ignore this, this is to resolve a "undefined symbol" issue due to main.asm being excluded from the project in order to run this test.asm
	cUsername		byte	MAX+1 DUP(0)

.code
main PROC
	; reset originStnIdx (index of origin station), destStnIdx (index of dest station), and stationMenuState
	; these are defined in TicketingPage.inc as of 10/4/2025. Might move them to a separate .inc for all data and functions related to printing graphical station menus.
	mov originStnIdx, 0
	mov destStnIdx, 0
	mov stationMenuState, 0

	; print menu letting user choose origin station
	invoke InitMenu, offset headerViewSchedule, offset locations, lengthof locations, offset promptViewScheduleLocOrigin, offset DrawStationMenuCanSelectOrigAndDest, 0

	; EAX = index of origin station
	mov originStnIdx, eax
	; Set bit 0 of stationMenuState flag, to indicate that ORIGIN has been selected
	or stationMenuState, 00000001b

	; print menu letting user choose dest station. 
	invoke InitMenu, offset headerViewSchedule, offset locations, lengthof locations, offset promptViewScheduleLocDest, offset DrawStationMenuCanSelectOrigAndDest, offset GetInputForDestStnPage 

	; EAX = index of destination station
	mov destStnIdx, eax
	; Set bit 1 of stationMenuState flag, to indicate that DESTINATION has been selected
	or stationMenuState, 00000010b

	; confirmation page with all ticket details and a route map
	invoke InitMenu, offset headerViewSchedule, offset finalPageOptions, lengthof finalPageOptions, 0, offset SingleJrnyConfmMenu, 0


	exit
main ENDP
END main

