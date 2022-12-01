				AREA 	check_square, CODE, READONLY			
                EXPORT  Check_square
					
Check_square
				PUSH 	{R4, R5, R6, R7, LR}
				MOV 	R4, R0
				MOV 	R5, R1
				MOV 	R6, R2
				
				MUL		R4, R4, R4
				MUL		R5, R5, R5				
				MUL		R6, R6, R6
			
				ADD		R4, R4, R5
				CMP		R6, R4
				
				MOVGE	R0, #1
				MOVLT	R0, #0

				POP 	{R4, R5, R6, R7, PC}
				
				END