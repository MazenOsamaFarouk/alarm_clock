/*
 * Timer_1_Config.h
 *
 *  Created on: Jan 10, 2021
 *      Author: mazen
 */

#ifndef TIMERS_TIMER_1_CONFIG_H_
#define TIMERS_TIMER_1_CONFIG_H_


/****************** Timer/counter 1 config settings ********************/
#define TCNT1_OP_MODE          TCNT1_CTC_OC
#define TCNT1_CLK_SELECT        IO_PS_1024

#define OC1_CHANNEL_SELECT        A

#define OCM1A_Interrupt         ENABLE
#define OCM1B_Interrupt         DISABLE
#define TCNT1_IC_Interrupt		DISABLE
#define TCNT1_OVF_Interrupt		DISABLE

#define TCNT1_PRELOAD			  0



#if TCNT1_OP_MODE==TCNT1_NORMAL || (TCNT1_OP_MODE==TCNT1_CTC_OC || TCNT1_OP_MODE==TCNT1_CTC_IC)

#if TCNT1_OP_MODE==TCNT1_NORMAL && TCNT1_OVF_Interrupt==ENABLE
#define TCNT1_CALLBACK		CB_TCNT1_OVF

#elif TCNT1_OP_MODE == TCNT1_CTC_OC

	#if OC1_CHANNEL_SELECT == A

#define COM1A_NON_PWM       NORMAL_OC1AB_DISCONN
#define TCNT1_CALLBACK		CB_TCNT1_CTC_A
	#elif OC1_CHANNEL_SELECT == B

#define COM1B_NON_PWM       NORMAL_OC1AB_DISCONN
#define TCNT1_CALLBACK		CB_TCNT1_CTC_B

	#endif
#elif TCNT1_OP_MODE == TCNT1_CTC_IC && TCNT1_IC_Interrupt == ENABLE
#define TCNT1_CALLBACK		CB_TCNT1_IC

#endif
#else

	#if OC1_CHANNEL_SELECT == A

#define COM1A_PWM		    CLR_OC1AB_NON_INV

	#elif OC1_CHANNEL_SELECT == B

#define COM1B_PWM		    CLR_OC1AB_NON_INV

	#endif

#endif

#endif /* TIMERS_TIMER_1_CONFIG_H_ */
