/*
 * hyend_os.c
 *
 *  Created on: Jun 19, 2021
 *      Author: Bayram
 */


#include "hyendOS/hyend_os.h"

Microcontroller mc;

uint32_t HYOS_GetTick() {
	return HAL_GetTick();
}

/*
 * Calls State Transition to IDLE.
 */
void HYOS_start() {
//	microcontroller.state = getMCState(NULL_STATE);
	stateTransition(IDLE);
}
