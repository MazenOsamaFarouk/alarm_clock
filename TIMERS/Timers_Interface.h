/*
 * Timers.h
 *
 *  Created on: Jun 27, 2020
 *      Author: mazen
 */

#ifndef TIMERS_INTERFACE_H_
#define TIMERS_INTERFACE_H_

/* Timer IDs*/
#define TIMER0 0
#define TIMER1 1
#define TIMER2 2
#define TIMER1A 'A'
#define TIMER1B 'B'

/*  Interrupt types */
// Timer-0
#define TCNT0_OVF_INT	 0
#define TCNT0_CTC_INT    1

// Timer-1
#define TCNT1_OVF_INT	 2
#define TCNT2_CTCA_INT   3
#define TCNT0_CTCB_INT   4
#define TCNT0_IC_INT	 5

// Timer-2
#define TCNT2_OVF_INT	 6
#define TCNT2_CTC_INT    7



void TCNT_VidInit(u8 timer_no);
void TCNT_VidStartTimer(u8 timer_no);
void TCNT_VidStopTimer(u8 timer_no);
void TCNT_VidResetAllTimers(void);
void TCNT_VidLoadOCR(u16 value, u8 timer_no);
void TCNT1_VidLoadICR1(u16 value);

void TCNT_VidCount_1s(u8 timer_no, u8 no_sec);
void TCNT_VidCount_1ms(u8 timer_no, u8 no_ms);
void TCNT_VidCount_1us(u8 timer_no, u8 no_us);



void TCNT_VidSetCallBack( u8 timer_no ,void (*LOCfptr)(void));


#endif /* TIMERS_INTERFACE_H_ */


