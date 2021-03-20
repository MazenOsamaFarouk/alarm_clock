/*
 * Timer_2_config.h
 *
 *  Created on: Jan 10, 2021
 *      Author: mazen
 */

#ifndef TIMERS_TIMER_2_CONFIG_H_
#define TIMERS_TIMER_2_CONFIG_H_


/********* Timer/counter 0 config settings ************/
#define TCNT2_OP_MODE           TCNT2_FAST_PWM
#define TCNT2_CLK_SELECT        IO_PS_64
#define TCNT2_OCM2_Interrupt    DISABLE
#define TCNT2_OVF_Interrupt     DISABLE
#define TCNT2_PRELOAD			0

#if TCNT2_OP_MODE==TCNT2_NORMAL || TCNT2_OP_MODE==TCNT2_CTC

#define COM2_NON_PWM       NORMAL_OC2_DISCONN

#elif TCNT2_OP_MODE==TCNT2_PWM || TCNT2_OP_MODE==TCNT2_FAST_PWM

#define COM2_PWM		   CLR_OC2_NON_INV

#endif

#if TCNT2_OVF_Interrupt==ENABLE && TCNT2_OCM2_Interrupt==DISABLE
	#define TCNT2_CALL_BACK  CB_TCNT2_OVF
#elif TCNT2_OVF_Interrupt==DISABLE && TCNT2_OCM2_Interrupt==ENABLE
	#define TCNT2_CALL_BACK  CB_TCNT2_CTC
#elif  TCNT2_OVF_Interrupt==ENABLE && TCNT2_OCM2_Interrupt==ENABLE
	#error "Timer_2 Config Error: Can not enable the interrupts of two modes at once!!"
#endif

/**********************************************************/


#endif /* TIMERS_TIMER_2_CONFIG_H_ */
