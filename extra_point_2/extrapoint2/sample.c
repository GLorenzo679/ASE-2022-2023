/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "draw/draw.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "adc/adc.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


int main(void)
{
  SystemInit();  												// System Initialization (i.e., PLL)
  LCD_Initialization();
	joystick_init();											// Joystick Initialization
	ADC_init();
	TP_Init();
	TouchPanel_Calibrate();
	LCD_Clear(White);
	init_RIT(0x000F4240);									// RIT Initialization, 10 ms
	init_timer(2, 0x001312D0);						// Timer 2 Initialization, 50 ms
	init_timer(3, 0x005F5E10);						// Timer 3 Initialization, 250 ms
	
	write_screen();												// draw default screen

	enable_RIT();
	
	LPC_SC->PCON |= 0x1;									// power-down	mode
	LPC_SC->PCON &= ~(0x2);						
	
	//enable speaker
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
