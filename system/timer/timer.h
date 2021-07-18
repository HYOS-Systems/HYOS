/*
 * timer.h
 *
 *  Created on: Jun 19, 2021
 *      Author: Bayram
 */

#include "drivers/hyend_os_drivers.h"

typedef struct {
	uint8_t flag;
	uint16_t cnt;
	uint16_t max;
} Timer;

#ifdef __STM32_TIM_PERIPHERAL
#define _TIMER_H

void TIMER_tick(Timer*);
uint8_t TIMER_itsTime(Timer*);

#endif
