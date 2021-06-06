/*
 * rsms.c
 *
 *  Created on: 17.01.2021
 *      Author: Bayram
 */

#include <hyendOS/systems/test/ifc/ifc_test.h>

#ifdef HYENDOS_SYSTEMS_IFC_TEST_H_

uint8_t buttonCounter = 0;

void ifc_test_init(){
}

void ifc_canHandle(CANBus* bus){
	MessageHeader mHeader;
	MESSAGE_TYPE mType = CANI_receiveMessage(bus, &mHeader);

	if(!CANI_isThisTarget(&mHeader)){
		return;
	}

	DataMessage dataMessage;
	StateMessage stateMessage;
	RequestDataMessage reqDataMessage;
	StateMessage reqStateMessage;

	switch(mType){
		case DATA:
			CANI_interpretDataMessage(bus, &dataMessage);
			/* Do stuff with dMessage */
			break;
		case STATUS:
			CANI_interpretStateMessage(bus, &stateMessage);
			/* Do stuff with sMessage */
			break;
		case REQUEST_DATA:
			CANI_interpretRequestDataMessage(bus, &reqDataMessage);
			//responseWithData(bus, &reqDataMessage);
			break;
		case REQUEST_STATUS:
			CANI_interpretRequestStateMessage(bus, &reqStateMessage);
			//responseWithState(bus, &reqStateMessage);
			break;
		case TRANSITION:
			CANI_interpretTransitionMessage(bus, &mHeader);
			break;
		default:
			break;
	}
}

#endif
