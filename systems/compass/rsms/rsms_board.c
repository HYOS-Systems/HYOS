/*
 * rsms_board.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/rsms/rsms_board.h"

RSMS_PeripheralStruct *rsms_struct;

typedef struct {
	MCU_STATUS mcuStatus;
#ifdef __STM32_CAN_PERIPHERAL
	CANBus *bus;
	CANP_MessageHeader header;
	CANP_Status status;
	CANP_DataMessage dataMessage;
	CANP_TransitionMessage transitionMessage;
#endif
} RSMS_BOARD_variables;

RSMS_BOARD_variables rsmsBoard;

#ifdef __STM32_SPI_PERIPHERAL
#endif

void RSMS_BOARD_initUsart() {
#ifdef __STM32_UART_PERIPHERAL
	initXprint(rsms_struct->serialDebug);
#endif
}

void RSMS_BOARD_initCAN() {
#ifdef __STM32_CAN_PERIPHERAL
	rsmsBoard.bus = CAN_init(rsms_struct->busIFC, RSMS);
#endif
}

void RSMS_BOARD_initSDLogging() {
#ifdef __STM32_SDIO_PERIPHERAL
	FRESULT res = SDFH_init();
	Logger_init(10000);

	if (res != FR_OK) {
		rsmsBoard.mcuStatus = RSMS_SD_FAULT;
	}
#endif
}

void RSMS_BOARD_initSensors() {
#ifdef __STM32_SPI_PERIPHERAL
	RSMS_P_init(rsms_struct, rsmsBoard.bus);
	RSMS_T_init(rsms_struct, rsmsBoard.bus);
#endif
}

void RSMS_BOARD_initTim() {
#ifdef __STM32_TIM_PERIPHERAL
	rsms_struct->canStatusTim.cnt = 0;
	rsms_struct->canStatusTim.max = 1000;
	rsms_struct->canStatusTim.flag = 0;

	rsms_struct->canDataTim.cnt = 0;
	rsms_struct->canDataTim.max = 100;
	rsms_struct->canDataTim.flag = 0;

	rsms_struct->pTim.cnt = 0;
	rsms_struct->pTim.max = 1;
	rsms_struct->pTim.flag = 0;

	rsms_struct->tTim.cnt = 0;
	rsms_struct->tTim.max = 100;
	rsms_struct->tTim.flag = 0;

	HAL_TIM_Base_Start_IT(rsms_struct->htim);
#endif
}

void RSMS_BOARD_initDataHandling() {
#ifdef __STM32_CAN_PERIPHERAL
	rsmsBoard.status.header.ID = RSMS_STATUS;
	rsmsBoard.status.header.status = DATA_OK;

	rsmsBoard.dataMessage.header.targetMCU = IFC;
	rsmsBoard.dataMessage.header.messageType = DATA;
	rsmsBoard.dataMessage.data2.header.ID = SYSTEM_NULL_DATA;
#endif
}

void RSMS_BOARD_init(RSMS_PeripheralStruct *rsms_PeripheralStruct) {
	rsms_struct = rsms_PeripheralStruct;
	rsmsBoard.mcuStatus = RSMS_OK;

//	RSMS_BOARD_initUsart();
	RSMS_BOARD_initCAN();
	RSMS_BOARD_initSDLogging();
	RSMS_BOARD_initSensors();
	RSMS_BOARD_initTim();
	RSMS_BOARD_initDataHandling();

	xprintf("RSMS Initialisiert.\n");
}

// Taks ===============================================================
void RSMS_BOARD_logAndSendStatus() {
#ifdef __STM32_CAN_PERIPHERAL
	rsmsBoard.status.state = microcontroller.state->stateID;
	rsmsBoard.status.mcuStatus = rsmsBoard.mcuStatus;
	CANI_packStatus(&rsmsBoard.dataMessage.data1, &rsmsBoard.status);

	rsmsBoard.dataMessage.header.timeStamp = HYOS_GetTick();
	CANI_sendData(rsmsBoard.bus, &rsmsBoard.dataMessage);
#endif
#ifdef __STM32_SDIO_PERIPHERAL
	Logger_logData("RSMS_State", 10, HYOS_GetTick(), microcontroller.state->stateID);
	Logger_logData("RSMS_Status", 11, HYOS_GetTick(), rsmsBoard.mcuStatus);
#endif
}

void RSMS_BOARD_tick(){
#ifdef __STM32_TIM_PERIPHERAL
	TIMER_tick(&rsms_struct->canStatusTim);
	TIMER_tick(&rsms_struct->canDataTim);
	TIMER_tick(&rsms_struct->pTim);
	TIMER_tick(&rsms_struct->tTim);
#endif
}

void RSMS_BOARD_canHandle(void *pt) {
#ifdef __STM32_CAN_PERIPHERAL
	rsmsBoard.bus = (CANBus*) pt;
	CANI_receiveMessage(rsmsBoard.bus, &rsmsBoard.header);

	if (!CANI_isThisTarget(&rsmsBoard.header)) {
		return;
	}

	switch (rsmsBoard.header.messageType) {
	case DATA:
		break;
	case REQUEST_DATA:
		break;
	case TRANSITION:
		CANI_interpretTransitionMessage(rsmsBoard.bus, &rsmsBoard.transitionMessage);
		break;
	default:
		break;
	}
#endif
}
