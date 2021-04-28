/*
 * ccdh.c
 *
 *  Created on: Jan 12, 2021
 *      Author: Bayram
 */

#include "hyendOS/App/hyend_os.h"

// Check for valid Initialization
#ifdef INC_CCDH_H_

Microcontroller mc;

void voidMethod() {
	// Empty Method
	// Is empty, shall remain empty...
}

void initNULL_MCU() {
	microcontroller->number = NULL_MCU;
	microcontroller->master = NULL_MCU;
}

void initIFC() {
	microcontroller->number = IFC;
	microcontroller->master = GSE;
	// Insert Handle-Initialization here
}

void initFSMS() {
	microcontroller->number = FSMS;
	microcontroller->master = IFC;
	// Insert Handle-Initialization here
}

void initRSMS() {
	microcontroller->number = RSMS;
	microcontroller->master = IFC;
	microcontroller->initHandle = &rsms_test_init;
	microcontroller->canNVICHandle = &rsms_test_canHandle;
}

void initVAS() {
	microcontroller->number = VAS;
	microcontroller->master = IFC;
	// Insert Handle-Initialization here
}

void initGSE() {
	microcontroller->number = GSE;
	microcontroller->master = GSE;
	// Insert Handle-Initialization here
}

void initDefault() {
	xprintf("Initializing MCs\n");

	microcontroller = &mc;

	voidTasks.alfaHandle = &voidMethod;
	voidTasks.bravoHandle = &voidMethod;
	voidTasks.charlieHandle = &voidMethod;
	voidTasks.deltaHandle = &voidMethod;
	voidTasks.echoHandle = &voidMethod;
	voidTasks.foxtrotHandle = &voidMethod;

	voidMCState.stateID = NULL_STATE;
	voidMCState.tasks = &voidTasks;

	/*
	 * What is done after this for-loop?
	 * - every Method Handle has voidMethod
	 * - all MC_States are validly initialized with voidTasks -> getMCState works now
	 * - mc is in NULL_STATE
	 *
	 * What is to be done by user?
	 * - fill method Handles
	 * - define Tasks for MC_States -> can be done as late binding in initHandle()
	 */
	microcontroller->initHandle = &voidMethod;
	microcontroller->canNVICHandle = &voidMethod;
	for (int j = 0; j < STATE_ID_END; j++) {
		microcontroller->mcStates[j] = voidMCState;
		microcontroller->mcStates[j].stateID = j;
	}
	microcontroller->state = getMCState(NULL_STATE);
}

/*
 * Initializes every struct.
 * Calles respective init-Function for the Microcontroller.
 * Sets State to NULL_STATE
 * Calls State Transition to IDLE.
 */
void initHyendOS(MICROCONTROLLER mc_ID) {
	initDefault();

	switch (mc_ID) {
	case NULL_MCU:
		initNULL_MCU();
		break;
	case IFC:
		initIFC();
		break;
	case FSMS:
		initFSMS();
		break;
	case RSMS:
		initRSMS();
		break;
	case VAS:
		initVAS();
		break;
	case GSE:
		initGSE();
		break;
	default:
		break;
	}

	microcontroller->initHandle();
	stateTransition(IDLE);
}

#endif /* CCDH_APP_CCDH_H_ */

