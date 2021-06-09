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

void voidMethod() {
	// Empty Method
	// Is empty, shall remain empty...
}

void HYOS_initNULL_MCU() {
	microcontroller->number = NULL_MCU;
	microcontroller->master = NULL_MCU;
}

void HYOS_initIFC(IFC_Test_PinStruct) {
	microcontroller->number = IFC;
	microcontroller->master = GSE;
	// Insert Handle-Initialization here
}

void HYOS_initFSMS() {
	microcontroller->number = FSMS;
	microcontroller->master = IFC;
	// Insert Handle-Initialization here
}

void HYOS_initRSMS() {
	microcontroller->number = RSMS;
	microcontroller->master = IFC;
//	microcontroller->initHandle = &rsms_test_init;
//	microcontroller->canNVICHandle = &rsms_test_canHandle;
}

void HYOS_initVAS() {
	microcontroller->number = VAS;
	microcontroller->master = IFC;
	// Insert Handle-Initialization here
}

void HYOS_initGSE() {
	microcontroller->number = GSE;
	microcontroller->master = GSE;
	GSE_init();
	// Insert Handle-Initialization here
}

void HYOS_initDefaultTask(Tasks* tasks){
	tasks->entry = &voidMethod;
	tasks->whileHandle = &voidMethod;
	tasks->exit = &voidMethod;

	tasks->alfaHandle = &voidMethod;
	tasks->bravoHandle = &voidMethod;
	tasks->charlieHandle = &voidMethod;
	tasks->deltaHandle = &voidMethod;
	tasks->echoHandle = &voidMethod;
	tasks->foxtrotHandle = &voidMethod;
}

void HYOS_initDefault() {
	xprintf("Initializing MCs\n");

	microcontroller = &mc;

	initDefaultTask(&voidMCState.tasks);
	voidMCState.stateID = NULL_STATE;

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
	for (int j = 0; j < STATE_ID_END; j++) {
		initDefaultTask(&microcontroller->mcStates[j].tasks);
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
void HYOS_initHyendOS(MICROCONTROLLER mc_ID) {
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

	stateTransition(IDLE);
}

#endif /* CCDH_APP_CCDH_H_ */

