;/**************************************************************************//**
; * @file     startup_LPC17xx.s
; * @brief    CMSIS Cortex-M3 Core Device Startup File for
; *           NXP LPC17xx Device Series
; * @version  V1.10
; * @date     06. April 2011
; *
; * @note
; * Copyright (C) 2009-2011 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/

; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WDT_IRQHandler            ; 16: Watchdog Timer
                DCD     TIMER0_IRQHandler         ; 17: Timer0
                DCD     TIMER1_IRQHandler         ; 18: Timer1
                DCD     TIMER2_IRQHandler         ; 19: Timer2
                DCD     TIMER3_IRQHandler         ; 20: Timer3
                DCD     UART0_IRQHandler          ; 21: UART0
                DCD     UART1_IRQHandler          ; 22: UART1
                DCD     UART2_IRQHandler          ; 23: UART2
                DCD     UART3_IRQHandler          ; 24: UART3
                DCD     PWM1_IRQHandler           ; 25: PWM1
                DCD     I2C0_IRQHandler           ; 26: I2C0
                DCD     I2C1_IRQHandler           ; 27: I2C1
                DCD     I2C2_IRQHandler           ; 28: I2C2
                DCD     SPI_IRQHandler            ; 29: SPI
                DCD     SSP0_IRQHandler           ; 30: SSP0
                DCD     SSP1_IRQHandler           ; 31: SSP1
                DCD     PLL0_IRQHandler           ; 32: PLL0 Lock (Main PLL)
                DCD     RTC_IRQHandler            ; 33: Real Time Clock
                DCD     EINT0_IRQHandler          ; 34: External Interrupt 0
                DCD     EINT1_IRQHandler          ; 35: External Interrupt 1
                DCD     EINT2_IRQHandler          ; 36: External Interrupt 2
                DCD     EINT3_IRQHandler          ; 37: External Interrupt 3
                DCD     ADC_IRQHandler            ; 38: A/D Converter
                DCD     BOD_IRQHandler            ; 39: Brown-Out Detect
                DCD     USB_IRQHandler            ; 40: USB
                DCD     CAN_IRQHandler            ; 41: CAN
                DCD     DMA_IRQHandler            ; 42: General Purpose DMA
                DCD     I2S_IRQHandler            ; 43: I2S
                DCD     ENET_IRQHandler           ; 44: Ethernet
                DCD     RIT_IRQHandler            ; 45: Repetitive Interrupt Timer
                DCD     MCPWM_IRQHandler          ; 46: Motor Control PWM
                DCD     QEI_IRQHandler            ; 47: Quadrature Encoder Interface
                DCD     PLL1_IRQHandler           ; 48: PLL1 Lock (USB PLL)
                DCD     USBActivity_IRQHandler    ; 49: USB Activity interrupt to wakeup
                DCD     CANActivity_IRQHandler    ; 50: CAN Activity interrupt to wakeup


                IF      :LNOT::DEF:NO_CRP
                AREA    |.ARM.__at_0x02FC|, CODE, READONLY
CRP_Key         DCD     0xFFFFFFFF
                ENDIF
					
					
                AREA    MY_DATA, DATA, READWRITE, align=3			
Calories_ordered		SPACE   6*4					
Quantity_ordered		SPACE	6*4


                AREA    |.text|, CODE, READONLY, align=3

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]      

				; your code here	

;				Algorithm:
;				1) 	I save in the readwrite memory only the calories and the quantity of the vectors
;				2) 	I sort the 2 vectors in decreasing order of calorie/quantity
;				3) 	I find the corresponding calorie/quantity in the original vector (I account for 
;					duplicate values of calorie/quantity), and i replace that calorie/quantity with
;					the corresponding ID
;				4)	I use some of the code of ex1, to compute the maxium product of calories and quantity
;					and I save it in R11
;				
;				I have structured my code using "functions". In fact I have the same part of code, that does
;				the sorting and then the corresponding ID finding, for the 2 distinct vectors.
;				However, I think that the code is greatly optimizable, maybe putting togheter the 3 distinct
;				operations (Sort, order ID, and find max).


				LDR 	R0, =Ingredient_calories
				LDR		R1, =Ingredient_quantity
				LDR		R2, =Num_ingredients
				LDRB	R2, [R2]
				LDR		R4,	=Calories_ordered
				LDR		R5, =Quantity_ordered
				MOV 	R6, #4			;index of elements to save
				MOV		R8, R2			;loop coutner
				MOV		R9, #0			;index of destination vector
				MOV 	R11, #2
				
;store only calories in vector				
store_calories
				LDR		R7, [R0, R6]
				STRB	R7, [R4, R9]
				ADD		R6, R6, #8
				ADD		R9, R9, #4
				SUBS	R8, R8, #1
				BNE		store_calories
				
				MOV 	R6, #4
				MOV		R8, R2
				MOV		R9, #0
				
;store only quantity in vector						
store_quantity
				LDR		R7, [R1, R6]
				STR		R7, [R5, R9]
				ADD		R6, R6, #8
				ADD		R9, R9, #4
				SUBS	R8, R8, #1
				BNE		store_quantity				
				
				MOV 	R0, R4			;sort calories vector	
				MOV 	R1, R5

				SUB		R2, R2, #1
sorting
				MOV		R3, #0			;x
				MOV 	R10, #4			;vector element dimension
				
loop1
				MOV		R4, #0			;y

loop2
				MUL		R9, R10, R4
				LDR		R6, [R0, R9]
				ADD		R7, R9, #4
				LDR		R7, [R0, R7]
				
				CMP		R6, R7
				BGT		update
swap				
				MOV		R8, R6
				MOV		R6, R7
				MOV		R7, R8

				STR		R6, [R0, R9]
				ADD		R8, R9, #4
				STR		R7, [R0, R8]			
				
update
				ADD		R4, R4, #1
				SUB		R5, R2, R3
				CMP		R5, R4
				BNE		loop2
				
				ADD		R3, R3, #1
				CMP		R2, R3
				BNE		loop1
				
				MOV 	R0, R1			;sort calories vector
				SUBS 	R11, R11, #1
				BNE 	sorting
				
;----------------------------------------------------------------------------			
				;now i have the sorted vectors of calories and quantity
				;I select the ID of the corresponding calorie/quantity and save in the same vector

				LDR 	R0, =Ingredient_calories
				LDR		R1, =Calories_ordered
				LDR		R2, =Num_ingredients
				LDRB	R2, [R2]
				MOV 	R10, #2
find_id				
				MOV		R4, #0			;R1 index
				MOV		R8, R2			;loop coutner
loop3
				MOV 	R3, #0			;R0 index
loop4				
				ADD		R7, R3, #4

				LDR		R5, [R0, R7]
				LDR		R6, [R1, R4]
				
				CMP		R5, R6
				BNE		update2
				
				LDR		R9, [R0, R3]
				
				;--	find duplicate function --
				PUSH	{R0-R12}
				
				CMP		R4, #0
				BEQ		store
				
				MOV 	R5, #0
				MOV 	R11, #0

find_duplicate			
				LDR		R12, [R1, R5]
				CMP		R9, R12
				BNE		next
				POP		{R0-R12}
				BEQ 	update2
next				
				ADD		R11, R11, #4
				ADD		R5, R5, #4
				CMP		R4, R11
				BNE		find_duplicate
store			
				STR		R9, [R1, R4]			
				POP		{R0-R12}
				;--

				ADD		R4, R4, #4
				SUBS	R8, R8, #1
				BEQ		end_find_id
				B		loop3
				
update2			
				ADD 	R3, R3, #8
				B	 	loop4

end_find_id				
				LDR 	R0, =Ingredient_quantity
				LDR		R1, =Quantity_ordered
				SUBS	R10, R10, #1
				BNE		find_id
				
;----------------------------------------------------------------------------
find_max
				LDR 	R0, =Ingredient_calories
				LDR 	R1, =Ingredient_quantity
				LDR 	R2, =Num_ingredients
				LDRB 	R2, [R2]					;loop counter
				MOV		R3, #0						;current max

				MOV 	R7, #0						;external loop vector index
loop5
				MOV 	R8, #0						;internal loop vector index					
				LDR 	R5, [R0, R7]				;load ID of element in Ingredient_calories
loop6
				LDR 	R6, [R1, R8]				;load ID of element in Ingredient_quantity			
				CMP		R5, R6
				ADDNE 	R8, R8, #8					;if IDs are not equal pass to next element in Ingredient_quantity
				BNE		loop6						;then repeat loop_6
				ADD		R7, R7, #4
				ADD		R8, R8, #4
				LDR		R10, [R0, R7]				;load ingredient calories
				LDR		R4, [R1, R8]				;load ingredient quantity
				MUL		R9, R10, R4					;multiply calories and quantity
				CMP		R9, R3
				BLT		not_max
				MOV		R3, R9						;update new max
				SUB		R7, R7, #4					;get ID address
				LDR		R11, [R0, R7]				;load max address in R11
				ADD		R7, R7, #4
not_max				
				ADD		R7, R7, #4					;skip to next ID in Ingredient_calories
				SUBS	R2, R2, #1					;decrease loop index
				BNE		loop5		
				
exit
				nop									;most caloric ingredient is saved in R11,
													;and IDs vector are sorted by calorie/quantity
                B		.
                ENDP
			
Ingredient_calories 	DCD 0x01, 30, 0x02, 70, 0x03, 200, 0x04, 42, 0x05, 81
						DCD 0x06, 20
							
Ingredient_quantity 	DCD 0x02, 50, 0x05, 3, 0x03, 10, 0x01, 5, 0x04
						DCD 8, 0x06, 30
							
Num_ingredients 		DCB 6

						LTORG

 
; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  TIMER0_IRQHandler         [WEAK]
                EXPORT  TIMER1_IRQHandler         [WEAK]
                EXPORT  TIMER2_IRQHandler         [WEAK]
                EXPORT  TIMER3_IRQHandler         [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  UART2_IRQHandler          [WEAK]
                EXPORT  UART3_IRQHandler          [WEAK]
                EXPORT  PWM1_IRQHandler           [WEAK]
                EXPORT  I2C0_IRQHandler           [WEAK]
                EXPORT  I2C1_IRQHandler           [WEAK]
                EXPORT  I2C2_IRQHandler           [WEAK]
                EXPORT  SPI_IRQHandler            [WEAK]
                EXPORT  SSP0_IRQHandler           [WEAK]
                EXPORT  SSP1_IRQHandler           [WEAK]
                EXPORT  PLL0_IRQHandler           [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  EINT0_IRQHandler          [WEAK]
                EXPORT  EINT1_IRQHandler          [WEAK]
                EXPORT  EINT2_IRQHandler          [WEAK]
                EXPORT  EINT3_IRQHandler          [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  BOD_IRQHandler            [WEAK]
                EXPORT  USB_IRQHandler            [WEAK]
                EXPORT  CAN_IRQHandler            [WEAK]
                EXPORT  DMA_IRQHandler            [WEAK]
                EXPORT  I2S_IRQHandler            [WEAK]
                EXPORT  ENET_IRQHandler           [WEAK]
                EXPORT  RIT_IRQHandler            [WEAK]
                EXPORT  MCPWM_IRQHandler          [WEAK]
                EXPORT  QEI_IRQHandler            [WEAK]
                EXPORT  PLL1_IRQHandler           [WEAK]
                EXPORT  USBActivity_IRQHandler    [WEAK]
                EXPORT  CANActivity_IRQHandler    [WEAK]

WDT_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
PWM1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
SPI_IRQHandler
SSP0_IRQHandler
SSP1_IRQHandler
PLL0_IRQHandler
RTC_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
EINT2_IRQHandler
EINT3_IRQHandler
ADC_IRQHandler
BOD_IRQHandler
USB_IRQHandler
CAN_IRQHandler
DMA_IRQHandler
I2S_IRQHandler
ENET_IRQHandler
RIT_IRQHandler
MCPWM_IRQHandler
QEI_IRQHandler
PLL1_IRQHandler
USBActivity_IRQHandler
CANActivity_IRQHandler

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit                

                END
