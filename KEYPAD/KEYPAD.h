/*
 * KEYPAD.h
 *
 *  Created on: Jun 12, 2020
 *      Author: mazen
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#define KEYPAD_PORT PORTC
#define DEBOUNCE_DELAY 5
#define COLS 2
#define ROWS 2



void KEYPAD_VidInit(void);

u8 KEYPAD_u8GetPressedKey(void);


#endif /* KEYPAD_H_ */
