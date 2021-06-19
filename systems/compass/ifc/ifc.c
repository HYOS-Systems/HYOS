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
	IFC_blinkLED(ifc_struct->LD1);
	HAL_GPIO_TogglePin(ifc_struct->LD1.port, ifc_struct->LD1.pin);
	ifc_init_DATA_FIELD(IDLE);
}

void ifc_IDLE_while() {
	if (ifc_struct->flag1 > 0) {
		ifc_struct->flag1 = 0;
		IFC_flickLED(ifc_struct->LD2, ifc_struct->LD3, ifc_struct->LD4);
	}
}

void ifc_IDLE_exit() {
	IFC_blinkLED(ifc_struct->LD1);
}

// FUELING ====================================================================
void ifc_FUELING_entry() {
	IFC_blinkLED(ifc_struct->LD2);
	HAL_GPIO_TogglePin(ifc_struct->LD2.port, ifc_struct->LD2.pin);
	ifc_init_DATA_FIELD(FUELING);
}

void ifc_FUELING_while() {
	if (ifc_struct->flag1 > 0) {
		ifc_struct->flag1 = 0;
		IFC_flickLED(ifc_struct->LD1, ifc_struct->LD3, ifc_struct->LD4);
	}

}

void ifc_FUELING_exit() {
	IFC_blinkLED(ifc_struct->LD2);
}

// RDY_SET ====================================================================
void ifc_RDY_SET_entry() {
	IFC_blinkLED(ifc_struct->LD3);
	HAL_GPIO_TogglePin(ifc_struct->LD3.port, ifc_struct->LD3.pin);
	ifc_init_DATA_FIELD(RDY_SET);
}

void ifc_RDY_SET_while() {
	if (ifc_struct->flag1 > 0) {
		ifc_struct->flag1 = 0;
		IFC_flickLED(ifc_struct->LD1, ifc_struct->LD2, ifc_struct->LD4);
	}
}

void ifc_RDY_SET_exit() {
	IFC_blinkLED(ifc_struct->LD3);
}

// FLIGHT ====================================================================
void ifc_FLIGHT_entry() {
	ifc_init_DATA_FIELD(FLIGHT);
	HAL_GPIO_TogglePin(ifc_struct->LD4.port, ifc_struct->LD4.pin);
	IFC_blinkLED(ifc_struct->LD4);
}

void ifc_FLIGHT_while() {
	if (ifc_struct->flag1 > 0) {
		ifc_struct->flag1 = 0;
		IFC_flickLED(ifc_struct->LD3, ifc_struct->LD2, ifc_struct->LD3);
	}
	if (ifc_struct->flag2 > 0) {
		ifc_struct->flag2 = 0;
		HAL_GPIO_TogglePin(ifc_struct->LD1.port, ifc_struct->LD1.pin);
		Logger_logData("Test", 4, HAL_GetTick() * 100, HAL_GetTick());
	}
}

void ifc_FLIGHT_exit() {
	IFC_blinkLED(ifc_struct->LD4);
}

// LANDED ====================================================================
void ifc_LANDED_entry() {
}

void ifc_LANDED_while() {
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

