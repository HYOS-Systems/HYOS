/*
 * ccdh.c
 *
 *  Created on: Jan 12, 2021
 *      Author: Bayram
 */

#include "systems/hyend_os_systems.h"

// Check for valid Initialization
#ifdef INC_CCDH_H_

Microcontroller mc;

/*
 * Calls State Transition to IDLE.
 */
void HYOS_start() {
//	microcontroller.state = getMCState(NULL_STATE);
	stateTransition(IDLE);
}

#endif /* CCDH_APP_CCDH_H_ */

