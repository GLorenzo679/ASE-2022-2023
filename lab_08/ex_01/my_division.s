				AREA 	my_division, CODE, READONLY			
                EXPORT  My_division
				IMPORT	__aeabi_fdiv
				IMPORT 	_fflt
				IMPORT	__aeabi_fmul

My_division
;				/*save R4,R5,R6,R7,LR,PC*/
;				/*obtain value of a and b and prepare for next function call*/
;				/*call __eabi_fdiv*/
;				/*results has to be returned!*/
;				/*restore R4,R5,R6,R7,LR,PC*/

				PUSH 	{R4, R5, R6, R7, LR}
				
				MOV		R5, R1			;save radius in R5
				
				BL		_fflt			;convert int area in float
				MOV 	R4, R0			;save area in variable	
				
				MOV 	R0, R5			;move radius in R0
				BL		_fflt			;convert int radius in float
				
				MOV 	R1, R0			;copy radius in R1
				
				BL		__aeabi_fmul	;do R0*R1 = radius^2
				
				MOV 	R1, R0			;move radius^2 in R1
				MOV 	R0, R4			;move area in R0
				
				BL 		__aeabi_fdiv	;R0/R1 = area/(radius^2)

				POP 	{R4, R5, R6, R7, PC}
				
				END