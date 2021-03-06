/*
 * KEYPAD.c
 *
 *  Created on: Jun 12, 2020
 *      Author: mazen
 */
#include "../LIB/STD_TYPES.h"
#include "../DIO/DIO_Interface.h"
#include "KEYPAD.h"
#include <util/delay.h>

void KEYPAD_VidInit(void)
{
	DIO_VidSetPortDirection(KEYPAD_PORT,0b00000011);
	DIO_VidSetPortValue(KEYPAD_PORT,0b00001111); // activate pull up resistors
}

u8 KEYPAD_u8GetPressedKey(void)
{
	u8 pressed_key = 0;

	u8 col = 0 , row = 0 ;

	for(col=0 ; col<COLS ; col++)
	{
		DIO_VidSetPinValue(KEYPAD_PORT,col+0,0);

		for(row=0 ; row<ROWS ; row++)
		{
			if(DIO_u8GetPinValue(KEYPAD_PORT,row+2) == 0 )
			{
				pressed_key =  (row + (col*COLS))+1;
			}
			while(DIO_u8GetPinValue(KEYPAD_PORT,row+2) == 0){}
			_delay_ms(DEBOUNCE_DELAY);
		}

		DIO_VidSetPinValue(KEYPAD_PORT,col+0,1);

	}



	return pressed_key;
}
