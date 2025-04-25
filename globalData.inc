; globalData.inc
; All global variables/constants/structures are here

; ========= Constant literals =========

; ENUM for TICKET TYPES
SINGLE_JOURNEY	EQU 0	; Single journey
WEEKLY_PASS		EQU 1	; Weekly pass
MONTHLY_PASS	EQU 2	; Monthly pass

; ENUM for STATION LOCATIONS
PENANG			EQU 0
PERAK			EQU 1
KUALA_LUMPUR	EQU 2
MELAKA			EQU 3
JOHOR			EQU 4

CR		= 0Dh	; Carriage Return
LF		= 0Ah	; Line Feed
TAB		= 09h
SPACE	= 20h

; ========= Structures =========


CONSOLE_CURSOR_INFO STRUCT
	dwSize DWORD ?
	bVisible DWORD ?
CONSOLE_CURSOR_INFO ENDS

; Console screen buffer info Struct

COORD STRUCT
    X WORD ?
    Y WORD ?
COORD ENDS

SMALL_RECT STRUCT
	Left     WORD ?  ;left-most column
	Top      WORD ?  ;upper-most row
	Right    WORD ?  ;right-most column
	Bottom   WORD ?  ;lowest row
SMALL_RECT ENDS

; Seems like this struct has already been defined. Maybe by irvine32.lib, maybe by a windows-related library in Irvine folder; I'm not sure.
; CONSOLE_SCREEN_BUFFER_INFO STRUCT
	; dwSize        COORD <>
	; dwCursorPos   COORD <>
	; wAttributes   WORD ?
	; srWindow      SMALL_RECT <>
	; dwMaxWinSize  COORD <>
; CONSOLE_SCREEN_BUFFER_INFO ENDS

CHAR_INFO STRUCT
    UNION Char
        UnicodeChar WORD ?    ; WCHAR is 2 bytes (16-bit)
        AsciiChar   BYTE ?    ; CHAR is 1 byte (8-bit)
    ENDS
    Attributes WORD ?         ; WORD is 2 bytes (16-bit)
CHAR_INFO ENDS

.const
	MAX	= 20								; max characters to read
	inputBuffer				byte	MAX+1 dup(?)  ; room for null character
	RINGGIT					byte	"RM ",0

	; ------------ Menu options ------------
	backOption				BYTE	"Back",0

	; ------------ Arrays ------------
	; STATION LOCATIONS AND DISTANCES (FROM JOHOR)
	location1				BYTE	"Penang",0
	location2				BYTE	"Perak",0
	location3				BYTE	"Kuala Lumpur",0
	location4				BYTE	"Melaka",0
	location5				BYTE	"Johor",0
	locations				DWORD	OFFSET location1, OFFSET location2, OFFSET location3, OFFSET location4, OFFSET location5

	distances				DWORD	1000, 580, 350, 200, 0
	speed					DWORD	320

	; TICKET TYPES
	tcktSingleJourney		BYTE	"Single Journey",0
	tcktWeeklyPass			BYTE	"Weekly Pass",0
	tcktMonthlyPass			BYTE	"Monthly Pass",0
	ticketTypes				DWORD	OFFSET tcktSingleJourney, OFFSET tcktWeeklyPass, OFFSET tcktMonthlyPass
	ticketPrices			DWORD	?, 40, 120

	;MAX SEATS TO COUNT available seats
	MAX_SEATS = 1200
	

	; ASSOCIATIVE ARRAY FOR BOUGHT TICKET DATA
	key1				BYTE	"Customer name: ",0
	key2				BYTE	"Ticket type: ",0
	key3				BYTE	"Price: ",0
	key4				BYTE	"Origin station: ",0
	key5				BYTE	"Destination station: ",0
	key6				BYTE	"Time slot: ",0
	TicketStructKeys	DWORD	OFFSET key1, OFFSET key2, OFFSET key3, OFFSET key4, OFFSET key5, OFFSET key6
	; TICKET STRUCT DEFINITION - Stores information of a single ticket bought by a customer
	TicketStruct STRUCT
		custName		DWORD	?	; points to a customer username string
		ticketType		DWORD	?	; points to a ticket type string
		price			DWORD	?
		originStn		DWORD	?	; points to a location string (only applicable for single-journey)
		destStn			DWORD	?	; points to a location string (only applicable for single-journey)
		originStnIdx	DWORD	-1	; must be initialized as -1, as anything starting from 0 indicates the presence of a value (index starts from 0)
		destStnIdx		DWORD	-1	
		timeSlotIdx		DWORD	-1	
	TicketStruct ENDS

	; Enums for accessing members of a TicketStruct struct.
	T_CUST_NAME			= 0
	T_TICKET_TYPE		= 1
	T_PRICE				= 2
	T_ORIGIN_STN		= 3
	T_DEST_STN			= 4
	T_ORIGIN_STN_IDX	= 5
	T_DEST_STN_IDX		= 6
	T_TIME_SLOT_IDX		= 7


; ========= Global variables =========
.data

	; USER CREDENTIALS -----------------------------------------------------------------------
	cUsername		byte	MAX+1 DUP(0)
	cPassword		byte	MAX+1 DUP(0)

	currentCustIdx	DWORD ? ; index of current user in userNameArray and passwordArray

	aUsername		byte	"1",0
	aPassword		byte	"1",0


	; TICKET DATA ----------------------------------------------------------------------------
	ticketStructArray	TicketStruct	20 dup(<>)	; Array storing tickets
	ticketCount			DWORD			0			; Keeps track of how many tickets have been stored in the array
	
; ERROR MESSAGES -------------------------------------------------------------------
	errorCode				DWORD	0
	errorMessages			DWORD	OFFSET err1 
	err1					BYTE	"Invalid option: Selected number not in list!",0

	errWeird				BYTE	"Weird error! Devs, pls look into this. Prob some sneaky logic error.",0


	; TICKET STRUCT VARIABLE - Stores information of a single ticket bought by a customer
	boughtTicket			TicketStruct <>


	MAX_SEATS = 1200

	time1 dword 1200,1123,1124,1134,1150,1123,1024,1134,1030,1023,1024,1124,1130,1123,1124,1132,1200,1033,1023,1033
	time2 dword 1111,1124,1114,1144,1151,1124,1124,1144,1141,1124,1124,1124,1141,1124,1124,1142,1111,1144,1124,1144
	time3 dword 1099,1123,1124,1194,1150,1123,1024,1094,1110,1023,1024,1124,1090,1123,1124,1092,1200,1190,1023,1199
	totalSold DWORD 20 dup(0)
	totalCost DWORD 20 dup(0)
	totalProfit DWORD 20 dup(0)
	totalSales DWORD 20 dup(0)

	MAX_USERS   = 200

	; Store user data
	userNameArray			byte MAX_USERS * (MAX+1) dup(0)
	passwordArray			byte MAX_USERS * (MAX+1) dup(0)

	; Store ticket data
	ticketCustName			byte MAX_USERS * (MAX+1) dup(0)		; each element is a 20-byte string: the name of the customer who bought the ticket
	ticketOriginIdx			dword MAX_USERS dup(0)
	ticketDestinationIdx    dword MAX_USERS dup(0) 
	ticketTimeSlotIdx		dword MAX_USERS dup(0)	; index of timeSlotArray

	; time slot array
	timeSlot1				byte "10 AM",0
	timeSlot2				byte "2 PM",0
	timeSlot3				byte "6 PM",0
	timeSlotArray			dword offset timeSlot1, offset timeSlot2, offset timeSlot3