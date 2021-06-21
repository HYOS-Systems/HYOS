/*
 * timer.c
 *
 *  Created on: Jun 19, 2021
 *      Author: Bayram
 */

#include "peripherals/timer/timer.h"
#ifdef _TIMER_H

uint8_t TIMER_itsTime(Timer *timer) {
	if (timer->flag > 0) {
		timer->flag = 0;
		return 1;
	} else {
		return 0;
	}
}

void TIMER_tick(Timer *timer){
	timer->cnt++;
	if (timer->cnt >= timer->max) {
		timer->cnt = 0;
		timer->flag = 1;
	}
}

#endif
