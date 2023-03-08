/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern int translate_morse(char* vett_input, int vet_input_lenght, char* vett_output, int vet_output_lenght, char change_symbol, char space, char sentence_end);

char vett_input[100] = "00002111201002013112001210210021113011112001114";
int vet_input_lenght = 0;
char vett_output[100]; 
int vet_output_lenght = 0;

volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;

void RIT_IRQHandler (void)
{	
	if(down_0 != 0){
		down_0++;
		
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
			switch(down_0){
				case 2:{
					int i = 0;
					vet_input_lenght = 0;
					vet_output_lenght = 0;
					LED_Out(0);
					
					for(i = 99; i >= 0; i--)
						vett_output[i] = '\n';
				}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_0 = 0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	else if(down_1 != 0){
		down_1++;
		
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
			switch(down_1){
				case 2:{
					int i = 0;
					char ch = '0';
					
					LED_Out(0);																								//leds all off
					
					while(ch != '4'){
						ch = vett_input[i];
						
						if (ch == '2')
							LED_Out(++vet_input_lenght);
						else if (ch == '3') {
							vet_input_lenght = vet_input_lenght + 2;
							LED_Out(vet_input_lenght);
						}
						
						i++;
					}
					
					LED_Out(++vet_input_lenght);															//sentence_end increment for last char
					
					LED_Out(255);																							//all leds on				
				}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_1 = 0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	else if(down_2 != 0){
		down_2++;		
		
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
			switch(down_2){
				case 2:{
					volatile int res;
					
					NVIC_DisableIRQ(EINT0_IRQn);
					NVIC_DisableIRQ(EINT1_IRQn);
					
					res = translate_morse(vett_input, vet_input_lenght, vett_output, vet_output_lenght, '2', '3', '4');

					LED_Out(res);
					
					NVIC_EnableIRQ(EINT0_IRQn);
					NVIC_EnableIRQ(EINT1_IRQn);				
				}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_2 = 0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}	

	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
