/*
 * rsms.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/ifc/ifc.h"

IFC_PeripheralStruct *ifc_struct;

// Stub =========================================================================
void ifc_initSingleDATA_FIELD(DATA_ID key, STATE_ID number) {
	DATA_FIELD *data;
	CANP_Data canData;
	CANP_Status status;

	data = MAP_getData(key);

	canData.header.ID = key;
	if (CANP_isStatusData(&canData)) {
		status.mcuStatus = key >> 4;
		status.state = number;
		CANP_packStatus(&canData, &status);
		data->value = canData.payload;
	} else {
		data->value = number | key;
	}
}

void ifc_init_DATA_FIELD(STATE_ID number) {
	DATA_ID key;

	key = SYSTEM_NULL_DATA;
	ifc_initSingleDATA_FIELD(key, number);

	key = SYSTEM_STATUS;
	ifc_initSingleDATA_FIELD(key, number);

	key = VAS_STATUS;
	ifc_initSingleDATA_FIELD(key, number);

	key = RSMS_STATUS;
	ifc_initSingleDATA_FIELD(key, number);

	key = RSMS_PRESSURE_1;
	ifc_initSingleDATA_FIELD(key, number);

	key = RSMS_PRESSURE_2;
	ifc_initSingleDATA_FIELD(key, number);

	key = RSMS_PT_1;
	ifc_initSingleDATA_FIELD(key, number);

	key = RSMS_PT_2;
	ifc_initSingleDATA_FIELD(key, number);

	key = RSMS_PT_7;
	ifc_initSingleDATA_FIELD(key, number);

	key = RSMS_PT_8;
	ifc_initSingleDATA_FIELD(key, number);

	key = FSMS_STATUS;
	ifc_initSingleDATA_FIELD(key, number);

	key = FSMS_GPS_NSAT;
	ifc_initSingleDATA_FIELD(key, number);

	key = PCDU_STATUS;
	ifc_initSingleDATA_FIELD(key, number);

	key = IFC_STATUS;
	ifc_initSingleDATA_FIELD(key, number);
}

// IDLE ====================================================================
void ifc_IDLE_entry() {
	IFC_DH_initSysBus(ifc_struct);
	IFC_DH_sendTransition(IDLE);
}

void ifc_IDLE_while() {
#ifdef __STM32_TIM_PERIPHERAL
	if (TIMER_itsTime(&ifc_struct->statusTim)) {
		IFC_BOARD_logStatus();
		HAL_GPIO_TogglePin(ifc_struct->LD1.port, ifc_struct->LD1.pin);
	}
#endif
//	uint32_t time = 3000;
//	IFC_DH_sendTransition(IDLE);
//	HAL_Delay(time);
//	IFC_DH_sendTransition(FUELING);
//	HAL_Delay(time);
//	IFC_DH_sendTransition(RDY_SET);
//	HAL_Delay(time);
//	IFC_DH_sendTransition(FLIGHT);
//	HAL_Delay(time);
}

void ifc_IDLE_exit() {
}

// FUELING ====================================================================
void ifc_FUELING_entry() {
	IFC_DH_sendTransition(FUELING);
}

void ifc_FUELING_while() {
#ifdef __STM32_TIM_PERIPHERAL
	if (TIMER_itsTime(&ifc_struct->statusTim)) {
		IFC_BOARD_logStatus();
	}
#endif
}

void ifc_FUELING_exit() {
}

// RDY_SET ====================================================================
void ifc_RDY_SET_entry() {
	IFC_DH_sendTransition(RDY_SET);
}

void ifc_RDY_SET_while() {
#ifdef __STM32_TIM_PERIPHERAL
	if (TIMER_itsTime(&ifc_struct->statusTim)) {
		IFC_BOARD_logStatus();
	}
#endif
}

void ifc_RDY_SET_exit() {
}

// FLIGHT ====================================================================
void ifc_FLIGHT_entry() {
	IFC_DH_sendTransition(FLIGHT);
}

void ifc_FLIGHT_while() {
#ifdef __STM32_TIM_PERIPHERAL
	if (TIMER_itsTime(&ifc_struct->statusTim)) {
		IFC_BOARD_logStatus();
	}
#endif
}

void ifc_FLIGHT_exit() {
}

// LANDED ====================================================================
void ifc_LANDED_entry() {
	IFC_DH_sendTransition(LANDED);
}

void ifc_LANDED_while() {
#ifdef __STM32_TIM_PERIPHERAL
	if (TIMER_itsTime(&ifc_struct->statusTim)) {
		IFC_BOARD_logStatus();
	}
#endif
}

void ifc_LANDED_exit() {
}

// init ====================================================================
void IFC_init(IFC_PeripheralStruct *ifc_PeripheralStruct) {
	ifc_struct = ifc_PeripheralStruct;
	IFC_BOARD_init(ifc_PeripheralStruct);

	// Init Microcontroller
	initMicrocontroller();
	microcontroller.number = IFC;
	microcontroller.master = GSE;

	Tasks *currentTasks;

	currentTasks = &getMCState(IDLE)->tasks;
	currentTasks->entry = &ifc_IDLE_entry;
	currentTasks->whileHandle = &ifc_IDLE_while;
	currentTasks->canHandle = &IFC_BOARD_canHandle;
	currentTasks->exit = &ifc_IDLE_exit;

	currentTasks = &getMCState(FUELING)->tasks;
	currentTasks->entry = &ifc_FUELING_entry;
	currentTasks->whileHandle = &ifc_FUELING_while;
	currentTasks->canHandle = &IFC_BOARD_canHandle;
	currentTasks->exit = &ifc_FUELING_exit;

	currentTasks = &getMCState(RDY_SET)->tasks;
	currentTasks->entry = &ifc_RDY_SET_entry;
	currentTasks->whileHandle = &ifc_RDY_SET_while;
	currentTasks->canHandle = &IFC_BOARD_canHandle;
	currentTasks->exit = &ifc_RDY_SET_exit;

	currentTasks = &getMCState(FLIGHT)->tasks;
	currentTasks->entry = &ifc_FLIGHT_entry;
	currentTasks->whileHandle = &ifc_FLIGHT_while;
	currentTasks->canHandle = &IFC_BOARD_canHandle;
	currentTasks->exit = &ifc_FLIGHT_exit;

	currentTasks = &getMCState(LANDED)->tasks;
	currentTasks->entry = &ifc_LANDED_entry;
	currentTasks->whileHandle = &ifc_LANDED_while;
	currentTasks->canHandle = &IFC_BOARD_canHandle;
	currentTasks->exit = &ifc_LANDED_exit;
}

