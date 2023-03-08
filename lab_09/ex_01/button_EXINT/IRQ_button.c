#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"

extern int translate_morse(char* vett_input, int vet_input_lenght, char* vett_output, int vet_output_lenght, char change_symbol, char space, char sentence_end);

char vett_input[100] = "00002111201002013112001210210021113011112001114";
int vet_input_lenght = 0;
char vett_output[100]; 
int vet_output_lenght = 0;


void EINT0_IRQHandler (void)	  
{
	int i = 0;
	vet_input_lenght = 0;
	vet_output_lenght = 0;
	LED_Out(0);
	
	for(i = 99; i >= 0; i--)
		vett_output[i] = '\n';
	
  LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt */
}

void EINT1_IRQHandler (void)	 	
{
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
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt */
}

void EINT2_IRQHandler (void)	  
{
	volatile int res;
	
	NVIC_DisableIRQ(EINT0_IRQn);
	NVIC_DisableIRQ(EINT1_IRQn);
	
	res = translate_morse(vett_input, vet_input_lenght, vett_output, vet_output_lenght, '2', '3', '4');

	LED_Out(res);
	
	NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(EINT1_IRQn);
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt */    
}


