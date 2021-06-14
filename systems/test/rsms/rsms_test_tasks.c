/*
 * ifc_test_tasks.c
 *
 *  Created on: Jun 10, 2021
 *      Author: Bayram
 */

#include "systems/test/rsms/rsms_test_tasks.h"

uint16_t rsms_blinkPeriod = 500;
uint8_t rsms_blinkCount = 3;

RSMS_Test_PinStruct *rsms_struct;

uint32_t RSMS_TEST_logTime() {
	return HAL_GetTick();
}

void RSMS_TEST_canHandle(void *busPointer) {
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

void RSMS_TEST_blinkLED(RSMS_LD_TypeDef ld) {
	HAL_GPIO_WritePin(ld.port, ld.pin, GPIO_PIN_SET);
	for (uint8_t i = 0; i < rsms_blinkCount * 2 - 1; i++) {
		HAL_Delay(rsms_blinkPeriod);
		HAL_GPIO_TogglePin(ld.port, ld.pin);
	}
}

void RSMS_TEST_tasks_init(RSMS_Test_PinStruct *rsms_test_struct) {
	rsms_struct = rsms_test_struct;
}

