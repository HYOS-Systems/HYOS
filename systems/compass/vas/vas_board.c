/*
 * vas_board.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/vas/vas_board.h"

#define VAS_BOARD_nMeasurements 8
VAS_PeripheralStruct *vas_struct;

typedef struct {
	uint8_t i;
	MCU_STATUS mcuStatus;
#ifdef __STM32_CAN_PERIPHERAL
	CANBus *bus;
	CANP_MessageHeader header;
	CANP_Status status;
	CANP_DataMessage dataMessage;
	CANP_TransitionMessage transitionMessage;
#endif
	int current;
}VAS_BOARD_variables;

VAS_BOARD_variables vasBoard;

#ifdef __STM32_SPI_PERIPHERAL
#endif

void VAS_BOARD_initUsart() {
#ifdef __STM32_UART_PERIPHERAL
	initXprint(vas_struct->serialDebug);
#endif
}

void VAS_BOARD_initGP() {
	INA_init(&vas_struct->GP1.ina);
}

void VAS_BOARD_initCAN() {
#ifdef __STM32_CAN_PERIPHERAL
	vasBoard.bus = CAN_init(vas_struct->bus, VAS);
#endif
}

void VAS_BOARD_initTim() {
#ifdef __STM32_TIM_PERIPHERAL
	vas_struct->statusTim.cnt = 0;
	vas_struct->statusTim.max = 1000;
	vas_struct->statusTim.flag = 0;

	vas_struct->gpCheckTim.cnt = 0;
	vas_struct->gpCheckTim.max = 10000;
	vas_struct->gpCheckTim.flag = 0;

	HAL_TIM_Base_Start_IT(vas_struct->htim);
#endif
}

void VAS_BOARD_initDataHandling() {
#ifdef __STM32_CAN_PERIPHERAL
	vasBoard.status.header.ID = VAS_STATUS;
	vasBoard.status.header.status = DATA_OK;

	vasBoard.dataMessage.header.targetMCU = IFC;
	vasBoard.dataMessage.header.messageType = DATA;
	vasBoard.dataMessage.data2.header.ID = SYSTEM_NULL_DATA;
#endif
}

void VAS_BOARD_init(VAS_PeripheralStruct *vas_PeripheralStruct) {
	vas_struct = vas_PeripheralStruct;
	vasBoard.mcuStatus = VAS_OK;

	VAS_BOARD_initUsart();
	VAS_BOARD_initCAN();
	VAS_BOARD_initGP();
	VAS_BOARD_initTim();
	VAS_BOARD_initDataHandling();

	xprintf("VAS Initialisiert.\n");
}

// Taks ===============================================================

short VAS_BOARD_getGPCurrent() {
	vasBoard.current = 0;
	for (vasBoard.i = 0; vasBoard.i < VAS_BOARD_nMeasurements; vasBoard.i++) {
		vasBoard.current += INA_measure(&vas_struct->GP1.ina);
		HAL_Delay(1);
	}
	return vasBoard.current / VAS_BOARD_nMeasurements;
}

uint8_t VAS_isGPOn() {
	return VAS_BOARD_getGPCurrent() > 2000;
}

uint8_t VAS_isGPRecording() {
	return VAS_BOARD_getGPCurrent() > 6000;
}

void VAS_switchPin(GPIOPair *pair, uint32_t delay) {
	HAL_GPIO_WritePin(pair->port, pair->pin, GPIO_PIN_SET);
	HAL_Delay(delay);
	HAL_GPIO_WritePin(pair->port, pair->pin, GPIO_PIN_RESET);
}

void VAS_shutdownGP() {
	VAS_switchPin(&vas_struct->GP1.on, 3000);
}

void VAS_startGP() {
	VAS_switchPin(&vas_struct->GP1.on, 1000);
	HAL_Delay(1000);
	if (VAS_isGPOn()) {
		vasBoard.mcuStatus = VAS_OK;
	} else {
		vasBoard.mcuStatus = VAS_GP_NO_POWER;
	}
	HAL_Delay(6000);
}

void VAS_startGPRecording() {
	VAS_switchPin(&vas_struct->GP1.rec, 200);
}

void VAS_BOARD_sendStatus() {
#ifdef __STM32_CAN_PERIPHERAL
	vasBoard.status.state = microcontroller.state->stateID;
	vasBoard.status.mcuStatus = vasBoard.mcuStatus;
	CANI_packStatus(&vasBoard.dataMessage.data1, &vasBoard.status);

	vasBoard.dataMessage.header.timeStamp = HYOS_GetTick();
	CANI_sendData(vasBoard.bus, &vasBoard.dataMessage);
#endif
}

void VAS_BOARD_tick() {
#ifdef __STM32_TIM_PERIPHERAL
	TIMER_tick(&vas_struct->statusTim);
	TIMER_tick(&vas_struct->gpCheckTim);
#endif
}

void VAS_BOARD_canHandle(void *pt) {
#ifdef __STM32_CAN_PERIPHERAL
	vasBoard.bus = (CANBus*) pt;
	CANI_receiveMessage(vasBoard.bus, &vasBoard.header);

	if (!CANI_isThisTarget(&vasBoard.header)) {
		return;
	}

	switch (vasBoard.header.messageType) {
	case DATA:
		break;
	case REQUEST_DATA:
		break;
	case TRANSITION:
		CANI_interpretTransitionMessage(vasBoard.bus, &vasBoard.transitionMessage);
		break;
	default:
		break;
	}
#endif
}
