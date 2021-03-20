/*
 * main.c
 *
 *  Created on: Mar 20, 2021
 *      Author: mazen
 */

#include "LIB/STD_TYPES.h"
#include "LCD/LCD_Interface.h"
#include "TIMERS/Timers_Interface.h"
#include "GIE/GIE_Interface.h"
#include "EXTI/EXTI_Interface.h"
#include "DIO/DIO_Interface.h"
#include <avr/eeprom.h>
#include <util/delay.h>




typedef struct
{
	u8 hours;
	u8 mins;
	u8 secs;
}TimeObject;



int main(void)
{
	TCNT_VidInit(1); /* use Timer 1 for counting */
	TCNT_VidInit(2); /* use Timer 2 for PWM for buzzer */
	LCD_VidInit();
	GIE_enable();
	char* time_string[10]; /**> string in the format HH:MM:SS */
	TimeObject alarm = {.hours=0,.mins=0, .secs=0};
	TimeObject setting = {.hours=0,.mins=0, .secs=0};


	while(1)
	{

	
	}
return 0;
}
