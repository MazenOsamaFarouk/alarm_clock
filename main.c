/*
 * main.c
 *
 *  Created on: Mar 20, 2021
 *      Author: mazen Osama
 */

#include "LIB/STD_TYPES.h"
#include "DIO/DIO_Interface.h"
#include "TIMERS/Timers_Interface.h"
#include "GIE/GIE_Interface.h"
#include "LCD/LCD_Interface.h"
#include "KEYPAD/KEYPAD.h"


#include <util/delay.h>
#include <stdio.h>
/**
 * KeyMap
 */
#define KEY_UP     3
#define KEY_DOWN   4
#define KEY_RESET  1
#define KEY_OK	   2


#define BUZZER_PIN   PORTD,PIN2
#define LCD_BL_PIN	 PORTD,PIN1
#define ONE_SECOND  31250UL

typedef struct
{
	u8 hours;
	u8 mins;
	u8 secs;
	u32 total;
}TimeObject;

typedef enum
{
	TIMER_MODE,
	SETTING_MODE
}AppMode;



char time_string[10]; /**> string in the format HH:MM:SS */
TimeObject setting = {.hours=0,.mins=0, .secs=0};
TimeObject alarm = {.hours=0,.mins=0, .secs=0};
volatile Bool display = false;
volatile AppMode mode = TIMER_MODE ; /*  application starts in timer mode */
volatile Bool alarm_finished = true; // alarm is finished by default

const u8 arrow_position[3]={5,8,11};
u8 current_arrow_pos = 0;
u8 display_on_time = 0;

void BUZZER_Init(void)
{
	DIO_VidSetPinDirection(BUZZER_PIN,OUTPUT);
}
void BUZZER_AlarmFinishedTone(void)
{
	for(u8 i=0 ;i<3;i++)
	{
		DIO_VidSetPinValue(BUZZER_PIN,1);
		_delay_ms(100);
		DIO_VidSetPinValue(BUZZER_PIN,0);
		_delay_ms(50);
	}
	DIO_VidSetPinValue(BUZZER_PIN,1);
	_delay_ms(100);
	DIO_VidSetPinValue(BUZZER_PIN,0);
}

void BUZZER_SettingDoneTone(void)
{
	for(u8 i=0 ;i<2;i++)
	{
		DIO_VidSetPinValue(BUZZER_PIN,1);
		_delay_ms(100);
		DIO_VidSetPinValue(BUZZER_PIN,0);
		_delay_ms(50);
	}
}

void CalcTotalTime(TimeObject* t)
{
	t->total = (t->hours*60*60)+(t->mins*60) + t->secs ;
}

void ResetTimeObject(TimeObject*t)
{
	t->hours = 0;
	t->mins = 0;
	t->secs = 0;
	t->total = 0;
}

void UpdateDisplay(TimeObject* screen_time)
{
	sprintf(time_string,"%02d:%02d:%02d",screen_time->hours,
			screen_time->mins,
			screen_time->secs);
	LCD_VidSendCommand(CLEAR_DISPLAY);
	LCD_VidSetCursorPosition(4,0);
	LCD_VidPrintString(time_string,0);
}
void UpdateSelectionArrow(void)
{
	LCD_VidSetCursorPosition(arrow_position[current_arrow_pos],1);
	LCD_VidSendChar('^');
}

void Count1sec(void)
{
	CalcTotalTime(&alarm);
	if(alarm.total < setting.total)
	{
		if(alarm.secs != 59)
		{
			alarm.secs++;
		}
		else
		{
			alarm.secs = 0;
			if(alarm.mins != 59)
			{
				alarm.mins++;
			}
			else
			{
				alarm.mins =0;
				alarm.hours++;
			}
		}
	}
	else
	{
		TCNT_VidStopTimer(1);
		alarm_finished = true;
	}

	display = true;
}

void ChangeMode(void)
{
	if(mode == TIMER_MODE)
	{
		mode = SETTING_MODE;
	}
	else if( mode == SETTING_MODE)
	{
		mode = TIMER_MODE;
	}
}

void ClearMessageLine(void)
{
	LCD_VidSetCursorPosition(0,1);
	LCD_VidPrintString("               ",0);
}

void UserMessage(char* msg)
{
	ClearMessageLine();
	LCD_VidSetCursorPosition(0,1);
	LCD_VidPrintString(msg,0);
}

void LCD_TurnOff(void)
{
	LCD_VidSendCommand(DISPLAY_OFF);
	DIO_VidSetPinValue(LCD_BL_PIN,0);

}

void LCD_TurnOn(void)
{
	LCD_VidSendCommand(DISPLAY_ON);
	DIO_VidSetPinValue(LCD_BL_PIN,1);

}



int main(void)
{
	u8 pressed_key = 0;
	Bool pause = false;
	CalcTotalTime(&alarm);
	CalcTotalTime(&setting);
	LCD_VidInit();
	DIO_VidSetPinDirection(LCD_BL_PIN,1);
	KEYPAD_VidInit();

	TCNT_VidInit(TIMER1); /* use Timer 1 for counting */
	TCNT_VidLoadOCR(ONE_SECOND,TIMER1A);
	TCNT_VidSetCallBack(TIMER1,Count1sec);
	GIE_enable();

	LCD_TurnOn();
	LCD_VidPrintString("Alarm ON...",0);
	_delay_ms(1000);
	BUZZER_Init();
	BUZZER_AlarmFinishedTone();
	UpdateDisplay(&alarm);
	while(1)
	{
		pressed_key = KEYPAD_u8GetPressedKey();

		if(pressed_key!= 0)
		{

			switch(mode)
			{
			case TIMER_MODE:
				LCD_TurnOn();
				switch(pressed_key)
				{
				case KEY_UP:
					if(alarm.total!=setting.total)
					{
						if(pause==false)
						{
							pause=true;
							ResetTimeObject((&alarm));
						}
						TCNT_VidStartTimer(1);
						alarm_finished = false;
						UserMessage("Alarm Started..");
						display_on_time = alarm.total;
						_delay_ms(1000);
						ClearMessageLine();
					}

					break;
				case KEY_DOWN:
					pause=true;
					TCNT_VidStopTimer(1);
					UserMessage("Alarm Paused..");
					_delay_ms(2000);
					ClearMessageLine();
					break;
				case KEY_RESET:
					ResetTimeObject((&alarm));
					UpdateDisplay(&alarm);
					break;
				case KEY_OK:
					ChangeMode();
					break;
				}
				break;
				case SETTING_MODE:
					LCD_TurnOn();
					switch(pressed_key)
					{
					case KEY_UP:
						switch(current_arrow_pos)
						{
						case 0:
							setting.hours++;
							break;
						case 1:
							setting.mins = (setting.mins+1)%60;
							break;
						case 2:
							setting.secs = (setting.secs+1)%60;
							break;
						}
						break;
						case KEY_DOWN:
							switch(current_arrow_pos)
							{
							case 0:
								setting.hours=setting.hours==0?setting.hours:setting.hours-1;
								break;
							case 1:
								setting.mins=setting.mins==0?59:setting.mins-1;
								break;
							case 2:
								setting.secs=setting.secs==0?59:setting.secs-1;
								break;
							}
							break;
							case KEY_RESET:
								current_arrow_pos = (current_arrow_pos+1)%3 ;
								break;
							case KEY_OK:
								CalcTotalTime(&setting);
								UpdateDisplay(&alarm);
								BUZZER_SettingDoneTone();
								ChangeMode();
								break;
					}
					break;
			}
		}
		else
		{
			if((alarm.total-display_on_time)==5)
			{
				LCD_TurnOff();
				display=false;
			}

			switch(mode)
			{
			case TIMER_MODE:
				if(display==true)
				{
					display=false;
					UpdateDisplay(&alarm);

					if(alarm_finished==true)
					{
						alarm_finished = false;
						display_on_time = 0;
						LCD_TurnOn();
						BUZZER_AlarmFinishedTone();
						UserMessage("Alarm Finished");
						ResetTimeObject(&alarm);
					}
				}
				break;
			case SETTING_MODE:
				LCD_TurnOn();
				UpdateDisplay(&setting);
				UpdateSelectionArrow();
				break;
			}

		}

	}


	return 0;
}
