/*******************************************************************************************************                         
**--------------File Info-------------------------------------------------------------------------------
** File name:				draw.h
** Descriptions:		
**------------------------------------------------------------------------------------------------------
** Created by:			Gabriele Lorenzo
** Created date:		29/12/2022
** Version:					1.0
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

#ifndef __draw_H 
#define __draw_H

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"

/* Private function prototypes -----------------------------------------------*/
void write_screen(void);
void draw_ui(void);
void draw_speaker(void);
void draw_battery(uint16_t Xpos, uint16_t Ypos, uint16_t dim_x, uint16_t dim_y, uint16_t border_width, uint16_t bar_width, uint16_t bar_length);
extern void increase_happiness(void);
extern void increase_satiety(void);
void draw_charge_bar(uint16_t Xpos, uint16_t Ypos, uint16_t dim_x, uint16_t dim_y, uint16_t Color);
void draw_age(uint16_t age);
void meal_snack_animation(uint16_t speed, uint16_t flag);
void death_animation(uint16_t speed);
void cuddle_animation(void);
void draw_image(uint16_t start_x, uint16_t start_y, uint16_t dim_x, uint16_t dim_y, uint16_t pixel_dim, unsigned char const* pixel_data);
void draw_rectangle(uint16_t Xpos, uint16_t Ypos, uint16_t dim_x, uint16_t dim_y, uint16_t Color);
void draw_box(uint16_t Xpos, uint16_t Ypos, uint16_t dim_x, uint16_t dim_y, uint16_t Color, uint16_t border_width);
void draw_pixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color, uint16_t pixel_dim);

/* Custom Variables */
extern int age;
extern int happiness;
extern int satiety;

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
