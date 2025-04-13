
.data
		overallSales dword ?
		overallCost dword ?
		overallProfit dword ?
		reportHeader byte "Report",0
		overallSalesW byte "Overall Sales",0
		overallCostW byte "Overall Cost",0
		overallProfitW byte "Overall Profit",0
.code

ReportPage proc

		ReportStart:
		call Profit
		call clrscr
		;ticket price * ticket sold


		mov eax, offset reportHeader
		mov ebx, lengthof reportHeader
		call PrintHeader

		 call CRLF				; new line
		
		 lea edx,overallSalesW
		 call writeString
		 mov al, TAB   
		 call WriteChar 
		 mov al,":"
		 call WriteChar 
		 mov al, TAB   
		 call WriteChar 
		 

		 mov esi,0
		 mov ecx,lengthof totalSales
		 mov eax,initialEax
		 calOSales:
			add eax,totalSales[esi]
			add esi,type totalSales
			loop calOSales

			call writeint

		 call crlf


		 lea edx,overallCostW
		 call writeString
		 mov al, TAB   
		 call WriteChar 
		 mov al,":"
		 call WriteChar 
		 mov al, TAB   
		 call WriteChar

		 		 mov esi,0
		 mov ecx,lengthof totalSales
		 mov eax,initialEax
		 calOCost:
			add eax,totalCost[esi]
			add esi,type totalCost
			loop calOCost

			call writeint




		 call crlf
		 lea edx,overallProfitW
		 call writeString
		 mov al, TAB   
		 call WriteChar 
		 mov al,":"
		 call WriteChar 
		 mov al, TAB   
		 call WriteChar

		 mov esi,0
		 mov ecx,lengthof totalSales
		 mov eax,initialEax
		 calOProfit:
			add eax,totalProfit[esi]
			add esi,type totalProfit
			loop calOProfit

			call writeint

		

		 call crlf
		call WaitMsg
		;call Clrscr
		;call LOGIN
		ret
ReportPage endp






