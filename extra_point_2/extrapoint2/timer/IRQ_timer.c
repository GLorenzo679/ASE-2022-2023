/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../music/music.h"
#include "../draw/draw.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

uint16_t SinTable[45] =                                      
{
		205, 233, 261, 288, 313, 336, 357, 374, 389,
		399, 406, 409, 408, 403, 394, 382, 366, 347,
		325, 301, 275, 247, 219, 190, 162, 135, 108,
		84, 62, 43, 27, 15, 6, 1, 0, 3, 10, 20, 35,
		52, 73, 96, 121, 148, 176
};

void TIMER0_IRQHandler(void)
{
	static int sineticks = 0;
	static int currentValue; 
	
	currentValue = SinTable[sineticks];
	currentValue = currentValue * volume;		//modify sin amplitude according to volume level
	
	/* DAC management */
	
	LPC_DAC->DACR = currentValue<<6;
	sineticks++;
	
	if(sineticks == 45)
		sineticks=0;
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler(void)
{
	disable_timer(0);
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler(void)
{
	playSound();
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler(void)
{
	cuddle_animation();
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
