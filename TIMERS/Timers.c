/*
 * Timers.c
 *
 *  Created on: Jun 27, 2020
 *      Author: mazen
 */
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_CALC.h"
#include "../DIO/DIO_Interface.h"
#include "Timers_Register.h"
#include "Timers_Config.h"



// array of call back function pointers
static void (*TCNT0_CallBackFptr[2])(void) = {NULL,NULL} ;
static void (*TCNT1_CallBackFptr[4])(void) = {NULL,NULL,NULL,NULL} ;
static void (*TCNT2_CallBackFptr[2])(void) = {NULL,NULL} ;



void TCNT_VidInit(u8 timer_no)
{


	switch(timer_no)
	{
	case 0:
		/* 1. choose wave generation mode*/
		#if TCNT0_OP_MODE == TCNT0_NORMAL
			CLR_BIT(TCCR0,WGM00);
			CLR_BIT(TCCR0,WGM01);
		#elif TCNT0_OP_MODE == TCNT0_PWM
			SET_BIT(TCCR0,WGM00);
			CLR_BIT(TCCR0,WGM01);
		#elif TCNT0_OP_MODE == TCNT0_CTC
			CLR_BIT(TCCR0,WGM00);
			SET_BIT(TCCR0,WGM01);
		#elif TCNT0_OP_MODE == TCNT0_FAST_PWM
			SET_BIT(TCCR0,WGM00);
			SET_BIT(TCCR0,WGM01);
		#endif

		/* 2. compare match mode select  */
		#ifdef COM0_NON_PWM
			TCCR0 = (TCCR0 & 0xCF) | COM0_NON_PWM ;
		#endif
		#ifdef COM0_PWM
			TCCR0 = (TCCR0 & 0xCF) | COM0_PWM ;
			/* 2.a make the corresponding pin an output*/
			DIO_VidSetPinDirection(PORTB,PIN3,1);
		#endif
		/* 3. choose prescaler */
			TCCR0 = (TCCR0 & 0xF8) | NO_CLK ; //start with timer off

			/* 5. Preload Timer */
				TCNT0 = TCNT0_PRELOAD ;

		/* 4. Overflow Interrupt*/
		#if TCNT0_OVF_Interrupt == ENABLE
			SET_BIT(TIMSK,TOIE0);
		#elif TCNT0_OVF_Interrupt == DISABLE
			CLR_BIT(TIMSK,TOIE0);
		#endif

		/*  6. output compare Interrupt*/
		#if OCM0_Interrupt == ENABLE
			SET_BIT(TIMSK,OCIE0);
		#elif OCM0_Interrupt == DISABLE
			CLR_BIT(TIMSK,OCIE0);
		#endif
			break ;
	case 1:
		/* 1. choose wave generation mode*/
		TCCR1A = (TCCR1A & 0xFC) | (TCNT1_OP_MODE & 0x03) ; //WGM11 and WGM10
		TCCR1B = (TCCR1B & 0xE7) | ((TCNT1_OP_MODE>>2)<<3); //WGM13 and WGM12

		/* 2. compare match mode select  */
#ifdef COM1A_NON_PWM
		TCCR1A = (TCCR1A & 0x3F) | (COM1A_NON_PWM<<6);

#endif


#ifdef COM1B_NON_PWM
		TCCR1A = (TCCR1A & 0xCF) | (COM1B_NON_PWM<<4);

#endif

#ifdef COM1A_PWM
		TCCR1A = (TCCR1A & 0x3F) | (COM1A_PWM<<6);
		/* 2.a set the corresponding pin as output */
		DIO_VidSetPinDirection(PORTD,PIN5,1);
#endif



#ifdef COM1B_PWM
		TCCR1A = (TCCR1A & 0xCF) | (COM1B_PWM<<4);
		/* 2.a set the corresponding pin as output */
		DIO_VidSetPinDirection(PORTD,PIN4,1);
#endif


		/* 3. choose prescaler */
		TCCR1B = (TCCR1B & 0xF8) | NO_CLK ; //start with the timer off
		/* 5. Preload Timer */
		TCNT1 = TCNT1_PRELOAD ;
		/* 4. Overflow Interrupt*/
#if TCNT1_OVF_Interrupt == ENABLE
		SET_BIT(TIMSK,TOIE1);
#elif TCNT1_OVF_Interrupt == DISABLE
		CLR_BIT(TIMSK,TOIE1);
#endif

#if OCM1A_Interrupt == ENABLE
		SET_BIT(TIMSK,OCIE1A);
#elif OCM1A_Interrupt == DISABLE
		CLR_BIT(TIMSK,OCIE1A);
#endif

#if OCM1B_Interrupt == ENABLE
		SET_BIT(TIMSK,OCIE1B);
#elif OCM1B_Interrupt == DISABLE
		CLR_BIT(TIMSK,OCIE1B);
#endif

#if TCNT1_IC_Interrupt == ENABLE
		DIO_VidSetPinDirection(PORTD,PIN6,0);
		SET_BIT(TIMSK,TICIE1);
#elif TCNT1_IC_Interrupt == DISABLE
		CLR_BIT(TIMSK,TICIE1);
#endif
		break ;
	case 2: break ;
	}



}
void TCNT_VidStartTimer(u8 timer_no)
{
	//start the clock of the timer
	switch(timer_no)
	{
	case 0: TCCR0 = (TCCR0 & 0xF8) | TCNT0_CLK_SELECT ;  break ;
	case 1: TCCR1B = (TCCR1B & 0xF8) | TCNT1_CLK_SELECT ;  break ;
	case 2: TCCR2 = (TCCR2 & 0xF8) | TCNT2_CLK_SELECT ; break ;
	}
}
void TCNT_VidStopTimer(u8 timer_no)
{
	//start the clock of the timer
	switch(timer_no)
	{
	case 0: TCCR0 = (TCCR0 & 0xF8) | NO_CLK ;  break ;
	case 1: TCCR1B = (TCCR1B & 0xF8) | NO_CLK ; break ;
	case 2: TCCR2 = (TCCR2 & 0xF8) | NO_CLK ; break ;
	}
}

void TCNT_VidResetAllTimers(void)
{
	SET_BIT(SFIOR,PSR10);
}


void TCNT_VidLoadOCR(u16 value, u8 timer_no)
{
	switch(timer_no)
	{
	case 0: OCR0 = (value & 0x00FF) ;   break;
	case 'A': OCR1A = value; break;
	case 'B': OCR1B = value; break;
	case 2: OCR2 = (value & 0x00FF);    break ;
	}
}

void TCNT1_VidLoadICR1(u16 value)
{
	ICR1 =  value ;
}


void TCNT_VidSetCallBack( u8 timer_no , void (*LOCfptr)(void))
{
	if(LOCfptr != NULL)
	{
		switch(timer_no)
		{
		case 0:
#if TCNT0_OVF_Interrupt==ENABLE || TCNT0_OCM0_Interrupt==ENABLE
			TCNT0_CallBackFptr[TCNT0_CALLBACK]=LOCfptr;

#endif
			break;
		case 1:
#if TCNT1_OVF_Interrupt==ENABLE || TCNT1_IC_Interrupt==ENABLE || OCM1B_Interrupt==ENABLE || OCM1A_Interrupt==ENABLE
			TCNT1_CallBackFptr[TCNT1_CALLBACK]=LOCfptr;
#endif
			break;
		case 2:
#if TCNT2_OVF_Interrupt==ENABLE || TCNT2_OCM2_Interrupt==ENABLE
			TCNT2_CallBackFptr[TCNT2_CALLBACK]=LOCfptr;

#endif
			break;
		}
	}

}

/* Timer/counter 0 - Overflow ISR */

void __vector_11(void)
{
	if(TCNT0_CallBackFptr[CB_TCNT0_OVF] != NULL)
	{
		TCNT0_CallBackFptr[CB_TCNT0_OVF]();
	}
}

/* Timer/counter 0 - compare match ISR */
void __vector_10(void)
{
	if(TCNT0_CallBackFptr[CB_TCNT0_CTC] != NULL)
	{
		TCNT0_CallBackFptr[CB_TCNT0_CTC]();
	}
}

/* Timer-1 - Overflow ISR */
void __vector_9(void)
{
	if(TCNT1_CallBackFptr[CB_TCNT1_OVF] != NULL)
	{
		TCNT1_CallBackFptr[CB_TCNT1_OVF]();
	}
}

/* Timer-1 - CTC_B ISR */
void __vector_8(void)
{
	if(TCNT1_CallBackFptr[CB_TCNT1_CTC_B] != NULL)
	{
		TCNT1_CallBackFptr[CB_TCNT1_CTC_B]();
	}
}

/* Timer-1 - CTC_A ISR  */
void __vector_7(void)
{
	if(TCNT1_CallBackFptr[CB_TCNT1_CTC_A] != NULL)
	{
		TCNT1_CallBackFptr[CB_TCNT1_CTC_A]();
	}
}

/* Timer-1 - Input Capture ISR */
void __vector_6(void)
{
	/* WARNING: THIS MODIFICATION WAS BECASUE THE
	 * VECTOR TABLE OF ATMEGA16 IS DIFFERENT FROM
	 * ATMEGA32 ...*/
	if(TCNT1_CallBackFptr[CB_TCNT1_CTC_A] != NULL)
	{
		TCNT1_CallBackFptr[CB_TCNT1_CTC_A]();
	}
/*

	if(TCNT1_CallBackFptr[CB_TCNT1_IC] != NULL)
	{
		TCNT1_CallBackFptr[CB_TCNT1_IC]();
	}

*/
}

/* Timer/counter 2 - Overflow ISR */

void __vector_5(void)
{
	if(TCNT2_CallBackFptr[CB_TCNT2_OVF] != NULL)
	{
		TCNT2_CallBackFptr[CB_TCNT2_OVF]();
	}
}

/* Timer/counter 2 - compare match ISR */
void __vector_4(void)
{
	if(TCNT2_CallBackFptr[CB_TCNT2_CTC] != NULL)
	{
		TCNT2_CallBackFptr[CB_TCNT2_CTC]();
	}
}











