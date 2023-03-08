/*******************************************************************************************************                          
**--------------File Info-------------------------------------------------------------------------------
** File name:				draw.c
** Descriptions:		
**------------------------------------------------------------------------------------------------------
** Created by:			Gabriele Lorenzo
** Created date:		29/12/2022
** Version:					1.0
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "../GLCD/GLCD.h"
#include "../music/music.h"
#include "../timer/timer.h"
#include "../images/triforce_zelda.c"
#include "../images/rupee_zelda.c"
#include "../images/new_link_zelda.c"
#include "../images/speaker_3.c"
#include "../images/heart.c"

int age = 0;
int happiness = 5;
int satiety = 5;

/*******************************************************************************
* Function Name  : draw_pixel
* Description    : Draw a single pixel of pixel_dim times pixel_dim dimension.
* Input          : Xpos: upper right corner of pixel x coordinate
									 Ypos: upper right corner of pixel y coordinate
									 dim_x: width of the pixel in pixels
									 dim_y: height of the pixel in pixels
									 Color: color of the pixel to draw 
* Output         : None
* Return         : None
*******************************************************************************/
void draw_pixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color, uint16_t pixel_dim){
	uint16_t i;
	uint16_t j;
	
	for(i = Ypos; i < Ypos + pixel_dim; i++)
		for(j = Xpos; j < Xpos + pixel_dim; j++)
			LCD_SetPoint(j, i, Color);
}

/*******************************************************************************
* Function Name  : draw_box
* Description    : Draw perimeter of a rectangle.
* Input          : Xpos: upper right corner of box x coordinate
									 Ypos: upper right corner of box y coordinate
									 dim_x: width of the box in pixels
									 dim_y: height of the box in pixels
									 Color: color of the cell to draw 
									 border_width: width of border in pixels
* Output         : None
* Return         : None
*******************************************************************************/
void draw_box(uint16_t Xpos, uint16_t Ypos, uint16_t dim_x, uint16_t dim_y, uint16_t Color, uint16_t border_width){
	uint16_t i;
	
	//draw x-axis lines
	for(i = 0; i < border_width; i++)
		LCD_DrawLine(Xpos, Ypos + i, Xpos + dim_x, Ypos + i, Color);
	
	for(i = 0; i < border_width; i++)
		LCD_DrawLine(Xpos, Ypos + dim_y + i, Xpos + dim_x + border_width - 1, Ypos + dim_y + i, Color);
	
	//draw y-axis lines
	for(i = 0; i < border_width; i++)
		LCD_DrawLine(Xpos + i, Ypos, Xpos + i, Ypos + dim_y, Color);
	
	for(i = 0; i < border_width; i++)
		LCD_DrawLine(Xpos + dim_x + i, Ypos, Xpos + dim_x + i, Ypos + dim_y, Color);
}

/*******************************************************************************
* Function Name  : draw_rectangle
* Description    : Draw a full rectangle.
* Input          : Xpos: upper right corner of rectangle x coordinate
									 Ypos: upper right corner of rectangle y coordinate
									 dim_x: width of the rectangle in pixels
									 dim_y: height of the rectangle in pixels
									 Color: color of the cell to draw
* Output         : None
* Return         : None
*******************************************************************************/
void draw_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t dim_x, uint16_t dim_y, uint16_t Color){
	uint16_t i;
	
	for(i = 0; i < dim_y; i++)
		LCD_DrawLine(Xpos, Ypos + i, Xpos + dim_x, Ypos + i, Color);
}

/*******************************************************************************
* Function Name  : draw_image
* Description    : Draw an image on screen, based of pixel_data content.
* Input          : start_x: upper right corner x of image coordinate
									 start_y: upper right corner y of image coordinate
									 dim_x: width of image to draw in pixels
									 dim_y: height of image to draw in pixels
									 pixel_dim: scaling factor, dimension of individual pixel (2 = 2x2 pixel)
									 pixel_data: template used for color drawing
* Output         : None
* Return         : None
*******************************************************************************/
void draw_image(uint16_t start_x, uint16_t start_y, uint16_t dim_x, uint16_t dim_y, uint16_t pixel_dim, unsigned char const* pixel_data){
	uint16_t i;
	uint16_t j;
	uint16_t color;
	uint16_t counter = 0;
	
	for(i = start_y; i < start_y + dim_y; i = i + pixel_dim){
		for(j = start_x; j < start_x + dim_x; j = j + pixel_dim){
			color = pixel_data[counter];
			color += pixel_data[++counter]<<8;
			draw_pixel(j, i, color, pixel_dim);
			
			counter++;
		}
	}
}

/*******************************************************************************
* Function Name  : cuddle_animation
* Description    : Alternate 2 drawings in a time interval given by Timer 3 (250 ms).
									 When 8 cycles have been completed (2 s) reset parameters and stop.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void cuddle_animation(void){
	static int cycle_counter = 0;
	
	if(cycle_counter == 8){
		draw_rectangle(55, 101, 13, 12, White);
		draw_rectangle(55 + 81, 101 + 12, 13, 12, White);
		draw_rectangle(55 + 96, 101 + 75, 13, 12, White);
		reset_timer(3);
		disable_timer(3);
		cycle_counter = 0;
		return;
	}
	else if(cycle_counter%2 == 0){
		draw_rectangle(55, 101, 13, 12, White);
		draw_rectangle(55 + 81, 101 + 12, 13, 12, White);
		draw_rectangle(55 + 96, 101 + 75, 13, 12, White);
	}
	else{
		draw_image(55, 101, 13, 12, 1, heart.pixel_data);
		draw_image(55 + 81, 101 + 12, 13, 12, 1, heart.pixel_data);
		draw_image(55 + 96, 101 + 75, 13, 12, 1, heart.pixel_data);
	}
	
	cycle_counter++;
}

/*******************************************************************************
* Function Name  : death_animation
* Description    : Simple death animation.
* Input          : speed: speed of the animation
* Output         : None
* Return         : None
*******************************************************************************/
void death_animation(uint16_t speed){
	int i;
	int Xpos = 55;

	//exit screen animation
	for(i = 1; i < 14; i++){
		draw_rectangle(Xpos, 101, 120, 99, White);
		Xpos = 55 + i*speed;
		draw_image(Xpos, 101, 120, 99, 3, new_link_zelda.pixel_data);
	}
}

/*******************************************************************************
* Function Name  : meal_snack_animation
* Description    : Food creation and eating animation.
* Input          : speed: speed of the animation
									 flag: food type selector
* Output         : None
* Return         : None
*******************************************************************************/
void meal_snack_animation(uint16_t speed, uint16_t flag){
	int i;
	int Xpos = 55;
	
	//if flag == 0 --> snack else flag == 1 --> meal
	if(!flag)		
		draw_image(185, 127, 34, 34, 2, triforce_zelda.pixel_data);
	else
		draw_image(200, 127, 20, 36, 2, rupee_zelda.pixel_data);
	
	//go to food animation
	for(i = 1; i < 24; i++){
		Xpos = 55 + i*speed;
		draw_image(Xpos, 101, 120, 99, 3, new_link_zelda.pixel_data);
	}
	
	sound_selector = 1;											//select eatings sound effect
	enable_sound = 1;		
	enable_timer(2);
	playSound();

	//go to rest place animation
	for(i = 1; i < 24; i++){
		Xpos -= speed;
		draw_image(Xpos, 101, 120, 99, 3, new_link_zelda.pixel_data);
	}
}

/*******************************************************************************
* Function Name  : draw_age
* Description    : Draw elapsed time.
* Input          : age: elapsed time in seconds
* Output         : None
* Return         : None
*******************************************************************************/
void draw_age(uint16_t age){
	char time_in_char[5] = "";

	sprintf(time_in_char, "    Age %02d:%02d:%02d    ", age/3600, (age%3600)/60, age%60);

	GUI_Text(35, 7, (uint8_t *) time_in_char, Black, White);
}

/*******************************************************************************
* Function Name  : draw_charge_bar
* Description    : Draw individual battery cell.
* Input          : Xpos: upper right corner of cell x coordinate
									 Ypos: upper right corner of cell y coordinate
									 dim_x: width of the cell in pixels
									 dim_y: height of the cell in pixels
									 Color: color of the cell to draw 
* Output         : None
* Return         : None
*******************************************************************************/
void draw_charge_bar(uint16_t Xpos, uint16_t Ypos, uint16_t dim_x, uint16_t dim_y, uint16_t Color){
	uint16_t i;
	uint16_t j;
	
	for(i = Ypos; i < Ypos + dim_y; i++)
		for(j = Xpos; j < Xpos + dim_x; j++)
			LCD_SetPoint(j, i, Color);
}

/*******************************************************************************
* Function Name  : increase_happiness
* Description    : Draw battery cell based on happiness value, then increase happiness
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/

void increase_happiness(void){
	//battery cell update
	if(happiness == 1)
		draw_charge_bar(35 + 1 + 2 + 12 * happiness, 45 + 2 + 1, 10, 20, Yellow);				//add one Yellow charge bar for happiness
	else if(happiness == 2)
		draw_charge_bar(35 + 1 + 2 + 12 * happiness, 45 + 2 + 1, 10, 20, Green);				//add one Green charge bar for happiness
	else if(happiness == 3 || happiness == 4)
		draw_charge_bar(35 + 1 + 2 + 12 * happiness, 45 + 2 + 1, 10, 20, Green);				//add one Green charge bar for happiness
	
	happiness++;
}

/*******************************************************************************
* Function Name  : increase_satiety
* Description    : Draw battery cell based on satiety value, then increase satiety
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void increase_satiety(void){
	//battery cell update
	if(satiety == 1)
		draw_charge_bar(135 + 1 + 2 + 12 * satiety, 45 + 2 + 1, 10, 20, Yellow);				//add one Yellow charge bar for satiety
	else if(satiety == 2)
		draw_charge_bar(135 + 1 + 2 + 12 * satiety, 45 + 2 + 1, 10, 20, Green);					//add one Green charge bar for satiety
	else if(satiety == 3 || satiety == 4)
		draw_charge_bar(135 + 1 + 2 + 12 * satiety, 45 + 2 + 1, 10, 20, Green);					//add one Green charge bar for satiety
	
	satiety++;
}

/*******************************************************************************
* Function Name  : draw_battery
* Description    : Draw default battery.
* Input          : Xpos: upper right corner of battery x coordinate
									 Ypos: upper right corner of battery y coordinate
									 dim_x: width of battery in pixels
									 dim_y: height of battery in pixels
									 border_width: width of border in pixels
									 bar_width: individual cell bar width in pixels
									 bar_height: individual cell bar height in pixels
* Output         : None
* Return         : None
*******************************************************************************/
void draw_battery(uint16_t Xpos, uint16_t Ypos, uint16_t dim_x, uint16_t dim_y, uint16_t border_width, uint16_t bar_width, uint16_t bar_height){
	uint16_t i = Ypos;
	uint16_t j;
	uint16_t color;
	
	//draw external lines for battery
	for(j = Xpos + border_width; j < Xpos + dim_x; j = j + border_width)
		draw_pixel(j, i, Black, border_width);
	
	i = Ypos + dim_y;
	
	for(j = Xpos + border_width; j < Xpos + dim_x; j = j + border_width)
		draw_pixel(j, i, Black, border_width);
	
	j = Xpos;
	
	for(i = Ypos + border_width; i < Ypos + dim_y; i = i + border_width)
		draw_pixel(j, i, Black, border_width);
	
	j = Xpos + dim_x;
	
	for(i = Ypos + border_width; i < Ypos + dim_y; i = i + border_width)
		draw_pixel(j, i, Black, border_width);
	
	//draw little square for battery
	j = Xpos + dim_x;
	
	for(i = Ypos + 7; i <= Ypos + 16; i = i + 4)
		draw_pixel(j, i, Black, 4);
		
	for(i = 0; i < 5; i++) {
		if(i == 0)
			color = Red;
		else if(i == 1)
			color = Yellow;
		else
			color = Green;
		
		draw_charge_bar(Xpos + 1 + 2 + 12 * i , Ypos + 2 + 1, bar_width, bar_height, color);	
	}
}

/*******************************************************************************
* Function Name  : draw_speaker
* Description    : Draw speaker image according to volume.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void draw_speaker(void){
	static int last_volume = 3;
	
	if(volume != last_volume){
		draw_image(2, 10, 19, 16, 1, speaker_3.pixel_data);
	
		//remove lines according to volume level
		if(volume < 3)
			draw_rectangle(2 + 10 + volume*3, 10, 3*(3 - volume) - 1, 16, White);
	}
	
	last_volume = volume;
}

/*******************************************************************************
* Function Name  : draw_ui
* Description    : Draw defualt start UI.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void draw_ui(void){
	//header
	GUI_Text(35, 7, (uint8_t *) "    Age 00:00:00    ", Black, White);
	GUI_Text(20, 25, (uint8_t *) " Happiness ", Black, White);
	GUI_Text(130, 25, (uint8_t *) " Satiety ", Black, White);
	draw_battery(35, 45, 62, 24, 2, 10, 20);		//draw default happiness battery
	draw_battery(135, 45, 62, 24, 2, 10, 20);		//draw default satiety battery
	draw_image(2, 10, 19, 16, 1, speaker_3.pixel_data); //draw default sound image
	
	//footer
	draw_box(0, 260, 119, 58, Black, 3);			//draw left black box
	draw_box(119, 260, 119, 58, Black, 3);		//draw right black box	
	GUI_Text(35, 285, (uint8_t *) " Snack ", Black, White);
	GUI_Text(150, 285, (uint8_t *) "  Meal ", Black, White);
}

/*******************************************************************************
* Function Name  : write_screen
* Description    : Draw default UI and character on screen. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void write_screen(void){
	draw_ui();
	
	draw_image(55, 101, 120, 99, 3, new_link_zelda.pixel_data);
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
