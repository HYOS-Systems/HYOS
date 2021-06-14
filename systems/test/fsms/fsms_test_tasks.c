/*
 * ifc_test_tasks.c
 *
 *  Created on: Jun 10, 2021
 *      Author: Bayram
 */

#include "systems/test/fsms/fsms_test_tasks.h"

uint16_t fsms_blinkPeriod = 500;
uint8_t fsms_blinkCount = 3;

FSMS_Test_PinStruct *fsms_struct;

uint32_t FSMS_TEST_logTime() {
	return HAL_GetTick();
}

void FSMS_TEST_canHandle(void *busPointer) {
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

void FSMS_TEST_blinkLED(FSMS_LD_TypeDef ld) {
	HAL_GPIO_WritePin(ld.port, ld.pin, GPIO_PIN_SET);
	for (uint8_t i = 0; i < fsms_blinkCount * 2 - 1; i++) {
		HAL_Delay(fsms_blinkPeriod);
		HAL_GPIO_TogglePin(ld.port, ld.pin);
	}
}

void FSMS_TEST_tasks_init(FSMS_Test_PinStruct *fsms_test_struct) {
	fsms_struct = fsms_test_struct;
}

