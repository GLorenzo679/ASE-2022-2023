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
#include "../GLCD/GLCD.h"
#include "../draw/draw.h"
#include "../TouchPanel/TouchPanel.h"
#include "../adc/adc.h"
#include "../music/music.h"
#include "../timer/timer.h"
#include "../images/triforce_zelda.c"
#include "../images/new_link_zelda.c"
#include "../images/new_link_zelda_2.c"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:	None
**
******************************************************************************/
// beat 1/4 = 1.65/4 seconds
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1

NOTE click_sound[] = {
	{g4, time_semicroma},
	{c3, time_semicroma},
};

NOTE eating_sound[] = {
	{c4, time_croma},
	{d4b, time_croma},
	{d4, time_croma},
	{e4b, time_croma},
};

NOTE death_sound[] = {
	{c4, time_biscroma},
	{b3, time_biscroma},
	{b3b, time_biscroma},
	{a3, time_biscroma},
	
	{b3, time_biscroma},
	{b3b, time_biscroma},
	{a3, time_biscroma},
	{a3b, time_biscroma},
	
	{b3b, time_biscroma},
	{a3, time_biscroma},
	{a3b, time_biscroma},
	{g3, time_biscroma},
	
	{a3, time_biscroma},
	{a3b, time_biscroma},
	{g3, time_biscroma},
	{g3b, time_biscroma},
	
	{a3b, time_biscroma},
	{g3, time_biscroma},
	{g3b, time_biscroma},
	{f3, time_biscroma},
	
	{g3, time_biscroma},
	{g3b, time_biscroma},
	{f3, time_biscroma},
};

NOTE cuddle_sound [] = {
	{g4, time_semicroma},
	{g4b, time_semicroma},
	{e4b, time_semicroma},
	{a3, time_semicroma},
	{a3b, time_semicroma},
	{e4, time_semicroma},
	{a4b, time_semicroma},
	{c5, time_semicroma},
};

#define CLICK_SOUND_DIM 2
#define EATING_SOUND_DIM 4
#define DEATH_SOUND_DIM 23
#define CUDDLE_SOUND_DIM 8

/*
	Global variables
*/
int enable_sound = 0;
int sound_selector = -1;

void playSound(void){
	static int currentNote = 0;
	static int ticks = 0;
	static int dim = 0;
	static NOTE *sound;
	
	//select sound effect to play
	//can select sound effect only if previous
	//sound finished playing(sound == NULL)
 	if(sound == NULL){
		if(sound_selector == 0){
			sound = click_sound;
			dim = CLICK_SOUND_DIM;
		}
		else if(sound_selector == 1){
			sound = eating_sound;
			dim = EATING_SOUND_DIM;
		}
		else if(sound_selector == 2){
			sound = death_sound;
			dim = DEATH_SOUND_DIM;
		}
		else if(sound_selector == 3){  
			sound = cuddle_sound;
			dim = CUDDLE_SOUND_DIM;
		}
	}
	
	//play every note of the sound effect
	if(!isNotePlaying()){
		++ticks;
		if(ticks == UPTICKS){
			ticks = 0;
 			playNote(sound[currentNote++]);
		}   
	}

	//check if the sound effect is finished
	//if so, reset all parameters
	if(currentNote == dim){
		reset_timer(2);
		disable_timer(2);
		currentNote = 0;
		ticks = 0;
		enable_sound = 0;
		sound_selector = -1;
		sound = NULL;
	}
}

void RIT_IRQHandler (void)
{	
	static int J_left = 0;
	static int J_right = 0;
	static int J_select = 0;
	
	static int right_flag = 0;										//when 1 right box is selected
	static int left_flag = 0;											//when 1 left box is selected
	
	static int enable_food_selection = 1;					//when 1 food selection is enabled
	
	static int enable_default_animation = 1;			//when 1 default animation is enabled
	static int defualt_animation_counter = 0;			//soft counter for default animation
	
	static int enable_age = 1;										//when 1 age animation is enabled
	static int age_counter = 0;										//soft counter for age update animation
	
	static int enable_decrease_cell = 0;					//when 1 cell of battery can decrease
	
	static int reset_flag = 0;										//when 1 trigger reset sequence
	
	static int enable_touch = 1;									//when 1 touch function is enabled
	static int enable_adc = 1;										//when 1 adc conversion is enabled
	static int adc_counter = 0;										//soft counter for ADC

	/*
		Volume selection
	*/
	if(enable_adc){
		if(adc_counter == 5){
			ADC_start_conversion();
			adc_counter = 0;
			
			draw_speaker();
		}
		else
			adc_counter++;
	}
	
	/*
		Touch animation
	*/
	if(enable_touch){
		if(getDisplayPoint(&display, Read_Ads7846(), &matrix )){
			if(display.y > 101 && display.y < 200 && display.x > 55 && display.x < 175){
				if(happiness < 5){
					sound_selector = 3;								//select cuddle sound effect
					enable_sound = 1;
					enable_timer(2);
					playSound();
					enable_timer(3);
					cuddle_animation();
					
					//wait until cuddle animation is finished
					while(LPC_TIM3->TCR != 0);
					
					increase_happiness();
				}
			}
		}		
	}

	
	/*
		Default animation with 1s interval
	*/
	if(enable_default_animation){
		if(defualt_animation_counter == 100){			//1s = 100 * 10ms
			draw_image(55, 99, 120, 99, 3, new_link_zelda_2.pixel_data);
			defualt_animation_counter++;
		}
		else if(defualt_animation_counter == 200){	//2s = 200 * 10ms
			draw_image(55, 101, 120, 99, 3, new_link_zelda.pixel_data);
			defualt_animation_counter = 0;
		}
		else
			defualt_animation_counter++;
	}
	
	/*
		Age update
	*/
	if(enable_age){
		if(age_counter == 100){			//1s = 100 * 10ms
			age++;
			draw_age(age);
			
			if(age%5 == 0)
				enable_decrease_cell = 1;		//5s passed by, enable decrease battery cell routine
			
			age_counter = 0;
		}
		else
			age_counter++;
	}
	
	/*
		Decrease Happiness and Satiety
	*/
	if(enable_decrease_cell){
		satiety--;
		happiness--;
		
		if(happiness == 0 || satiety == 0){ //inititate reset procedure
			draw_charge_bar(35 + 1 + 2 + 12 * happiness , 45 + 2 + 1, 10, 20, White);		//eliminate one charge bar for happiness
			draw_charge_bar(135 + 1 + 2 + 12 * satiety , 45 + 2 + 1, 10, 20, White);		//eliminate one charge bar for satiety

			//death sound
			sound_selector = 2;								//select death sound effect
			enable_sound = 1;
			enable_timer(2);
			playSound();
			
			death_animation(16);
			GUI_Text(100, 170, (uint8_t *) " DEAD ", Red, White);
			
			draw_rectangle(0, 260, 240, 60, White);
			
			draw_box(0, 260, 237, 57, Red, 3);
			GUI_Text(90, 282, (uint8_t *) " Reset ", Black, White);
			
			reset_flag = 1;
			enable_age = 0;
			enable_decrease_cell = 0;
			enable_default_animation = 0;
			enable_touch = 0;
			enable_adc = 0;
		}
		else{
			draw_charge_bar(35 + 1 + 2 + 12 * happiness , 45 + 2 + 1, 10, 20, White);		//eliminate one charge bar for happiness
			draw_charge_bar(135 + 1 + 2 + 12 * satiety , 45 + 2 + 1, 10, 20, White);		//eliminate one charge bar for satiety
			enable_decrease_cell = 0;
		}
	}
	
	/*
		Joystick controller
	*/
	if(enable_food_selection){
		if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
			/* Joytick Select pressed p1.25 */
			
			J_select++;
			switch(J_select){
				case 1:
					if(reset_flag){
						draw_box(0, 260, 237, 57, Black, 3);					
						draw_box(0, 260, 237, 57, Red, 3);
						
						//reset all parameters
						age = 0;
						happiness = 5;
						satiety = 5;
						reset_flag = 0;
						enable_age = 1;
						enable_default_animation = 1;
						defualt_animation_counter = 0;
						age_counter = 0;
						right_flag = 0;
						left_flag = 0;
						enable_touch = 1;
						enable_adc = 1;
						
						//draw default ui
						draw_rectangle(0, 260, 240, 60, White);
						write_screen();
					}
					else if(left_flag && happiness < 5){
						draw_box(0, 260, 119, 58, Black, 3);			//draw left black box
						draw_box(0, 260, 119, 58, Red, 3);				//draw left red box
						
						enable_food_selection = 0;								//food selection disabled when animation is playing
						
						//snack food animation
						sound_selector = 0;											//select click sound effect
						enable_sound = 1;
						enable_timer(2);
						meal_snack_animation(2, 0); 						//speed must be always smaller than pixel_dim used in draw_image()	
						increase_happiness();
						
						enable_food_selection = 1;
					}
					else if(right_flag && satiety < 5){
						draw_box(119, 260, 119, 58, Black, 3);		//draw right black box	
						draw_box(119, 260, 119, 58, Red, 3);			//draw right red box	
						
						enable_food_selection = 0;								//food selection disabled when animation is playing
						
						//meal animation
						sound_selector = 0;											//select click sound effect
						enable_sound = 1;
						enable_timer(2);
						meal_snack_animation(2, 1); 						//speed must be always smaller than pixel_dim used in draw_image()				
						increase_satiety();							
						
						enable_food_selection = 1;		
					}
					break;
				default:
					break;
			}
		}
		else{
			J_select = 0;
		}
		
		if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
			/* Joytick Left pressed p1.27 */
			
			J_left++;
			switch(J_left){
				case 1:
					right_flag = 0;
					draw_box(119, 260, 119, 58, Black, 3);		//clean right red box
					left_flag = 1;
					draw_box(0, 260, 119, 58, Red, 3);				//draw left red box
					break;
				default:
					break;
			}
		}
		else{
			J_left=0;
		}

		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
			/* Joytick Right pressed p1.28 */
			
			J_right++;
			switch(J_right){
				case 1:
					left_flag = 0;
					draw_box(0, 260, 119, 58, Black, 3);		//clean left red box
					right_flag = 1;
					draw_box(119, 260, 119, 58, Red, 3);		//draw right red box
					break;
				default:
					break;
			}
		}
		else{
			J_right = 0;
		}
	}	

	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	// clear interrupt flag
}

/******************************************************************************
**                            End Of File
******************************************************************************/
