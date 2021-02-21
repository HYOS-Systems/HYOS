/*
 * ccdh.c
 *
 *  Created on: Jan 12, 2021
 *      Author: Bayram
 */

#include "hyendOS/App/hyend_os.h"

// Check for valid Initialization
#ifdef INC_CCDH_H_

Microcontroller mc_NULL;
Microcontroller mc_IFC;
Microcontroller mc_FSMS;
Microcontroller mc_RSMS;
Microcontroller mc_VAS;
Microcontroller mc_COMBINER;

Microcontroller* controller[] = {
	&mc_NULL,
	&mc_IFC,
	&mc_FSMS,
	&mc_RSMS,
	&mc_VAS,
	&mc_COMBINER
};

uint8_t number_mc = sizeof(controller) / sizeof(controller[0]);


void voidMethod() {
	// Empty Method
}

void initIFC(){
	// Insert Handle-Initialization here
}

void initFSMS(){
	// Insert Handle-Initialization here
}

void initRSMS(){
	mc_RSMS.initHandle = &rsms_init;
}

void initVAS(){
	// Insert Handle-Initialization here
}

void initCOMBINER(){
	// Insert Handle-Initialization here
}

void initDefault() {
	xprintf("Initializing MCs\n");
	xprintf("Number of MCs: %d\n", number_mc);

	microcontroller = &mc_NULL;

	mc_NULL.number = RSMS;
	mc_IFC.number = IFC;
	mc_FSMS.number = FSMS;
	mc_RSMS.number = RSMS;
	mc_VAS.number = VAS;
	mc_COMBINER.number = COMBINER;

	for (int i = 0; i < number_mc; i++) {
		Microcontroller* mc = controller[i];
		mc->initHandle = &voidMethod;
		mc->cyclic5HzHandle = &voidMethod;
		mc->cyclic10HzHandle = &voidMethod;
		mc->cyclic100HzHandle = &voidMethod;
		mc->cyclic1kHzHandle = &voidMethod;
		mc->cyclic10kHzHandle = &voidMethod;
		mc->canNVICHandle = &voidMethod;
	}

	initIFC();
	initFSMS();
	initRSMS();
	initVAS();
	initCOMBINER();
}

void initHyendOS(enum MICROCONTROLLER mc_ID) {
	initDefault();

	for (int i = 0; i < number_mc; i++) {
		Microcontroller* mc = controller[i];
		if (mc->number == mc_ID) {
			microcontroller = mc;
			break;
		}
	}
}

#endif /* CCDH_APP_CCDH_H_ */

