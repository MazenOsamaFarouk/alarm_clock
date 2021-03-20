/*
 * Timers_Config.h
 *
 *  Created on: Jun 29, 2020
 *      Author: mazen
 */

#ifndef TIMERS_CONFIG_H_
#define TIMERS_CONFIG_H_


#ifndef F_CPU
	#define F_CPU 8000000   //Default for IMT-hardware Kit is 8 MHz crystal
#endif


#include "Timer_0_Config.h"
#include "Timer_1_Config.h"
#include "Timer_2_Config.h"

#endif /* TIMERS_CONFIG_H_ */
