/*
 * ifc_test_tasks.c
 *
 *  Created on: Jun 10, 2021
 *      Author: Bayram
 */

#include "systems/test/ifc/ifc_test_tasks.h"

uint16_t blinkPeriod = 500;
uint8_t blinkCount = 3;

CANP_TransitionMessage transitionOrder;
IFC_Test_PinStruct *ifc_struct;

uint32_t IFC_TEST_logTime() {
	return HAL_GetTick();
}

void IFC_TEST_canHandle(CANBus *bus) {
	CANP_MessageHeader mHeader;
	MESSAGE_TYPE mType = CANI_receiveMessage(bus, &mHeader);

	if (!CANI_isThisTarget(&mHeader)) {
		return;
	}

	CANP_DataMessage dataMessage;
//	CANP_StateMessage stateMessage;
	CANP_RequestDataMessage reqDataMessage;
//	CANP_StateMessage reqStateMessage;
	CANP_TransitionMessage transitionMessage;

	switch (mType) {
	case DATA:
		CANI_interpretDataMessage(bus, &dataMessage);
		/* Do stuff with dMessage */
		break;
//	case STATUS:
//		CANI_interpretStateMessage(bus, &stateMessage);
//		/* Do stuff with sMessage */
//		break;
	case REQUEST_DATA:
		CANI_interpretRequestDataMessage(bus, &reqDataMessage);
		//responseWithData(bus, &reqDataMessage);
		break;
//	case REQUEST_STATUS:
//		CANI_interpretRequestStateMessage(bus, &reqStateMessage);
//		responseWithState(bus, &reqStateMessage);
//		break;
	case TRANSITION:
		CANI_interpretTransitionMessage(bus, &transitionMessage);
		break;
	default:
		break;
	}
}

void IFC_TEST_orderTransition(STATE_ID state) {
	transitionOrder.header.timeStamp = ifc_test_logTime();
	transitionOrder.state = state;
	CANI_sendTransition(ifc_struct->systemsBus, &transitionOrder);
}

void IFC_TEST_blinkLED(LD_TypeDef ld) {
	HAL_GPIO_WritePin(ld.port, ld.pin, GPIO_PIN_SET);
	for (uint8_t i = 0; i < blinkCount * 2 - 1; i++) {
		HAL_Delay(blinkPeriod);
		HAL_GPIO_TogglePin(ld.port, ld.pin);
	}
}

void IFC_TEST_tasks_init(IFC_Test_PinStruct *ifc_test_struct) {
	ifc_struct = ifc_test_struct;
	transitionOrder.messageValid = 1;
	transitionOrder.state = NULL_STATE;
	transitionOrder.header.targetMCU = MAX_MCU;
}

