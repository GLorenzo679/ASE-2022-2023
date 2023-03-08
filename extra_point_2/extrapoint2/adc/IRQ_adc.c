/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../timer/timer.h"
#include "../draw/draw.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     									// Last converted value

int volume = 3;																			// Default volume level

void ADC_IRQHandler(void) {

	AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);				//Read Conversion Result
	
	//added hysteresis to discard readings that are too similar to last reading
	//ADC reads oscillating values even if not used, we try to discard those
	if(AD_current < AD_last-0.2*AD_last || AD_current > AD_last+0.2*AD_last){ 
		volume = AD_current*4/0xFFF;										//4 levels of volume
		AD_last = AD_current;
  }	
}
