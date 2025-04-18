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

	

	; ASSOCIATIVE ARRAY FOR BOUGHT TICKET DATA
	key1				BYTE	"Customer name: ",0
	key2				BYTE	"Ticket type: ",0
	key3				BYTE	"Price: ",0
	key4				BYTE	"Origin station: ",0
	key5				BYTE	"Destination station: ",0
	TicketStructKeys	DWORD	OFFSET key1, OFFSET key2, OFFSET key3, OFFSET key4, OFFSET key5 
	; TICKET STRUCT DEFINITION - Stores information of a single ticket bought by a customer
	TicketStruct STRUCT
		custName		DWORD	?	; points to a customer username string
		ticketType		DWORD	?	; points to a ticket type string
		price			DWORD	?
		originStn		DWORD	?	; points to a location string (only applicable for single-journey)
		destStn			DWORD	?	; points to a location string (only applicable for single-journey)
		originStnIdx	DWORD	?
		destStnIdx		DWORD	?
	TicketStruct ENDS

; ========= Global variables =========
.data
	; ERROR MESSAGES -------------------------------------------------------------------
	errorCode				DWORD	0
	errorMessages			DWORD	OFFSET err1 
	err1					BYTE	"Invalid option: Selected number not in list!",0

	errWeird				BYTE	"Weird error! Devs, pls look into this. Prob some sneaky logic error.",0


	; TICKET STRUCT VARIABLE - Stores information of a single ticket bought by a customer
	boughtTicket			TicketStruct <>

	time1 dword 1200,1123,1124,1134,1150,1123,1024,1134,1030,1023,1024,1124,1130,1123,1124,1132,1200,1033,1023,1033
	time2 dword 1111,1124,1114,1144,1151,1124,1124,1144,1141,1124,1124,1124,1141,1124,1124,1142,1111,1144,1124,1144
	time3 dword 1099,1123,1124,1194,1150,1123,1024,1094,1110,1023,1024,1124,1090,1123,1124,1092,1200,1190,1023,1199
	totalSold DWORD 20 dup(0)
	totalCost DWORD 20 dup(0)
	totalProfit DWORD 20 dup(0)
	totalSales DWORD 20 dup(0)

	MAX_USERS   = 200

	userNameArray			byte MAX_USERS * MAX dup(0)
	passwordArray			byte MAX_USERS * MAX dup(0)
	ticketname				byte MAX_USERS * MAX dup(0)
	ticketOrigin			dword MAX_USERS * MAX dup(0)
	ticketDestination       dword MAX_USERS * MAX dup(0)
	ticketTimeslot			dword MAX_USERS * MAX dup(0)