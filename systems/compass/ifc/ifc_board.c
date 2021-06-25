/*
 * ifc_board.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/ifc/ifc_board.h"

IFC_PeripheralStruct *ifc_struct;

typedef struct {
	MCU_STATUS mcuStatus;
	uint32_t time;
	CANP_Status status;
	CANP_Data data;
} IFC_BOARD_Peripherals;
IFC_BOARD_Peripherals ifcBoard;

// Init =================================================================
void IFC_BOARD_initUsart() {
#ifdef __STM32_UART_PERIPHERAL
	initXprint(ifc_struct->serialDebug);
#endif
}

void IFC_BOARD_initSDLogging() {
#ifdef __STM32_SDIO_PERIPHERAL
	FRESULT res = SDFH_init();
	Logger_init(480);

	if (res != FR_OK) {
		ifcBoard.mcuStatus = IFC_SD_FAULT;
	}
#endif
}

void IFC_BOARD_initTim() {
#ifdef __STM32_TIM_PERIPHERAL

	ifc_struct->statusTim.cnt = 0;
	ifc_struct->statusTim.max = 1000;
	ifc_struct->statusTim.flag = 0;
	ifc_struct->second.cnt = 0;
	ifc_struct->second.max = 10;
	ifc_struct->second.flag = 0;

	HAL_TIM_Base_Start_IT(ifc_struct->htim);
#endif
}

void IFC_BOARD_init(IFC_PeripheralStruct *ifc_PeripheralStruct) {
	ifc_struct = ifc_PeripheralStruct;
	ifcBoard.mcuStatus = IFC_OK;

//	IFC_BOARD_initUsart(ifc_PeripheralStruct);
	MAP_init();
	IFC_DH_init(ifc_struct);
	IFC_BOARD_initSDLogging();
	IFC_BOARD_initTim();

	xprintf("IFC Initialisiert.\n");
}

// DataHandling =================================================================
void IFC_BOARD_tick() {
#ifdef __STM32_TIM_PERIPHERAL
	TIMER_tick(&ifc_struct->statusTim);
	TIMER_tick(&ifc_struct->second);
#endif
}

void IFC_BOARD_logSingleStatus(const char *messageState, uint8_t lenState, const char *messageStatus, uint8_t lenStatus,
		DATA_ID key) {
#ifdef __STM32_SDIO_PERIPHERAL
	ifcBoard.time = MAP_getDataToCANP(key, &ifcBoard.data);
	CANI_unpackStatus(&ifcBoard.data, &ifcBoard.status);

	Logger_logData(messageState, lenState, ifcBoard.time, ifcBoard.status.state);
	Logger_logData(messageStatus, lenStatus, ifcBoard.time, ifcBoard.status.mcuStatus);
#endif
}

void IFC_BOARD_logStatus() {
	// Store own state to Map
	ifcBoard.status.state = microcontroller.state->stateID;
	ifcBoard.status.mcuStatus = ifcBoard.mcuStatus;
	ifcBoard.status.header.ID = IFC_STATUS;
	ifcBoard.status.header.status = DATA_OK;
	CANI_packStatus(&ifcBoard.data, &ifcBoard.status);
	MAP_setDataFromCANP(&ifcBoard.data, HYOS_GetTick());

	// Log all Stati to SD Card
	IFC_BOARD_logSingleStatus("IFC_State", 9, "IFC_Status", 10, IFC_STATUS);
	IFC_BOARD_logSingleStatus("FSMS_State", 10, "FSMS_Status", 11, FSMS_STATUS);
	IFC_BOARD_logSingleStatus("RSMS_State", 10, "RSMS_Status", 11, RSMS_STATUS);
	IFC_BOARD_logSingleStatus("VAS_State", 9, "VAS_Status", 10, VAS_STATUS);
}

// DataHandling =================================================================
void IFC_BOARD_canHandle(void *pt) {
	IFC_DH_canHandle((CANBus*) pt);
}
