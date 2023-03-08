						AREA asm_functions, CODE, READONLY, align = 3			
					
translate_morse  		PROC
						EXPORT  translate_morse 

						MOV 	R12, SP

						PUSH 	{R4-R11, LR}			;REVIEW

						LDR		R4, [R12]				;load change symbol char
						LDR		R5, [R12, #4]			;load space char
						LDR		R6, [R12, #8]			;load sentence end char
						
loop					
						MOV		R10, #0XFFFFFFFF
next
						LDRB	R7, [R0], #1			;load next char
						
						CMP		R7, #0x30				;if char=='0'
						BNE		is_1
						LSL		R10, R10, #4
						B		next
is_1														
						CMP		R7, #0x31				;if char=='1'
						BNE		not_01
						LSL		R10, R10, #4
						EOR		R10, #1
						B		next												
not_01						
						CMP		R7, R4					;if char=='2' -> change_symbol
						BNE		is_space
						BL		convert
						ADD 	R3, R3, #1				;add one to character counter
						STRB	R8, [R2], #1			;save translated char
						B		loop
is_space						
						CMP		R7, R5					;if char=='3' -> space
						BNE		is_end
						BL		convert
						STRB	R8, [R2], #1			;save translated char
						MOV		R8, #0X20				;save ascii code for space in R8		
						STRB	R8, [R2], #1			;save space in translated vector
						ADD 	R3, R3, #2				;add two to character counter
						B		loop	
is_end						
						CMP		R7, R6					;if char=='4' -> end of sentence
						BNE		loop
						BL		convert					;conversion of last char
						ADD 	R3, R3, #1				;add one to character counter
						STRB	R8, [R2], #1
						
						SUB		R2, R2, R3				;go to first char of translated vector (its address)
						MOV		R0, R3					;move translated vector lenght value to R0
						POP		{R4-R11, PC}

						ENDP




							
convert					PROC
						
						PUSH	{R0-R7, R9-R12, LR}		
						
						LDR		R0, =_Code				;load code table
						LDR		R1, =_Symbol			;load symbol table
						
loop_2						
						LDR		R2, [R0], #4			;load code
						LDRB	R8, [R1], #1			;load corresponding symbol
						CMP		R10, R2					;if code equal to the one that we have to convert -> finish loop, char found
						BNE 	loop_2					;corresponding symbol will be saved in R8
					
						POP		{R0-R7, R9-R12, PC}
						ENDP
													
							
							
							
							
						AREA _data, DATA, READONLY
							
_Code					DCD 0XFFFFFF01, 0XFFFF1000, 0XFFFF1010, 0XFFFFF100, 0XFFFFFFF0, 0XFFFF0010, 0XFFFFF110, 0XFFFF0000, 0XFFFFFF00
						DCD 0XFFFF0111, 0XFFFFF101, 0XFFFF0100, 0XFFFFFF11, 0XFFFFFF10, 0XFFFFF111, 0XFFFF0110, 0XFFFF1101, 0XFFFFF010
						DCD 0XFFFFF000, 0XFFFFFFF1, 0XFFFFF001, 0XFFFF0001, 0XFFFFF011, 0XFFFF1001, 0XFFFF1011, 0XFFFF1100, 0XFFF01111
						DCD 0XFFF00111, 0XFFF00011, 0XFFF00001, 0XFFF00000, 0XFFF10000, 0XFFF11000, 0XFFF11100, 0XFFF11110, 0XFFF11111
							
_Symbol
						DCB	"A", "B", "C", "D", "E", "F", "G", "H", "I" 
						DCB	"J", "K", "L", "M", "N", "O", "P", "Q", "R" 
						DCB	"S", "T", "U", "V", "W", "X", "Y", "Z", "1"
						DCB	"2", "3", "4", "5", "6", "7", "8", "9", "0"

_ROWS		 			DCB 11		

						END
						
