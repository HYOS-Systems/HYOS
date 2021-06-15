/*
 * ifc_test_tasks.c
 *
 *  Created on: Jun 10, 2021
 *      Author: Bayram
 */

#include "systems/test/ifc/ifc_test_tasks.h"

uint16_t ifc_blinkPeriod = 500;
uint8_t ifc_blinkCount = 3;

CANP_TransitionMessage ifc_transitionOrder;
IFC_Test_PinStruct *ifc_struct;

uint32_t IFC_TEST_logTime() {
	return HAL_GetTick();
}

void IFC_TEST_canHandle(void *busPointer) {
	xprintf("Call Of CAN-Handle\n");
	CANBus* bus = (CANBus*) busPointer;
	CANP_MessageHeader header;
	CANI_receiveMessage(bus, &header);

	if (!CANI_isThisTarget(&header)) {
		return;
	}

	CANP_DataMessage dataMessage;
	CANP_RequestDataMessage reqDataMessage;
	CANP_TransitionMessage transitionMessage;

	switch (header.messageType) {
	case DATA:
		CANI_interpretDataMessage(bus, &dataMessage);
		/* Do stuff with dataMessage */
		break;
	case REQUEST_DATA:
		CANI_interpretRequestDataMessage(bus, &reqDataMessage);
		//responseWithData(bus, &reqDataMessage);
		break;
	case TRANSITION:
		CANI_interpretTransitionMessage(bus, &transitionMessage);
		break;
	default:
		break;
	}
}

void IFC_TEST_orderTransition(STATE_ID state) {
	ifc_transitionOrder.header.timeStamp = IFC_TEST_logTime();
	ifc_transitionOrder.state = state;
	CANI_sendTransition(ifc_struct->systemsBus, &ifc_transitionOrder);
}

void IFC_TEST_blinkLED(IFC_LD_TypeDef ld) {
	HAL_GPIO_WritePin(ld.port, ld.pin, GPIO_PIN_SET);
	for (uint8_t i = 0; i < ifc_blinkCount * 2 - 1; i++) {
		HAL_Delay(ifc_blinkPeriod);
		HAL_GPIO_TogglePin(ld.port, ld.pin);
	}
}

void IFC_TEST_tasks_init(IFC_Test_PinStruct *ifc_test_struct) {
	ifc_struct = ifc_test_struct;
	ifc_transitionOrder.messageValid = 1;
	ifc_transitionOrder.state = NULL_STATE;
	ifc_transitionOrder.header.targetMCU = MAX_MCU;
}

