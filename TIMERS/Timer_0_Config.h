/*
 * Timer_0_Config.h
 *
 *  Created on: Jan 10, 2021
 *      Author: mazen
 */

#ifndef TIMERS_TIMER_0_CONFIG_H_
#define TIMERS_TIMER_0_CONFIG_H_


/********* Timer/counter 0 config settings ************/
#define TCNT0_OP_MODE           TCNT0_FAST_PWM
#define TCNT0_CLK_SELECT        IO_PS_64
#define TCNT0_OCM0_Interrupt    DISABLE
#define TCNT0_OVF_Interrupt     DISABLE
#define TCNT0_PRELOAD			0

#if TCNT0_OP_MODE==TCNT0_NORMAL || TCNT0_OP_MODE==TCNT0_CTC

#define COM0_NON_PWM       NORMAL_OC0_DISCONN

#elif TCNT0_OP_MODE==TCNT0_PWM || TCNT0_OP_MODE==TCNT0_FAST_PWM

#define COM0_PWM		   CLR_OC0_NON_INV

#endif

#if TCNT0_OVF_Interrupt==ENABLE && TCNT0_OCM0_Interrupt==DISABLE
	#define TCNT0_CALLBACK  CB_TCNT0_OVF
#elif TCNT0_OVF_Interrupt==DISABLE && TCNT0_OCM0_Interrupt==ENABLE
	#define TCNT0_CALLBACK  CB_TCNT0_CTC
#elif  TCNT0_OVF_Interrupt==ENABLE && TCNT0_OCM0_Interrupt==ENABLE
	#error "Timer_0 Config Error: Can not enable the interrupts of two modes at once!!"
#elif TCNT0_OVF_Interrupt==DISABLE && TCNT0_OCM0_Interrupt==DISABLE
	#define TCNT0_CALLBACK  CB_TCNT0_CTC
#endif

/**********************************************************/


#endif /* TIMERS_TIMER_0_CONFIG_H_ */
