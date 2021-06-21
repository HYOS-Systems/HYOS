/*
 * rsms.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/rsms/rsms.h"

RSMS_PeripheralStruct *rsms_struct;

// IDLE ====================================================================
void rsms_IDLE_entry() {
	HAL_GPIO_WritePin(rsms_struct->LD1.port, rsms_struct->LD1.pin, GPIO_PIN_SET);
}

void rsms_IDLE_while() {
#ifdef __STM32_TIM_PERIPHERAL
	if (TIMER_itsTime(&rsms_struct->canStatusTim)) {
		RSMS_BOARD_logAndSendStatus();
	}
#endif
}

void rsms_IDLE_exit() {
	HAL_GPIO_WritePin(rsms_struct->LD1.port, rsms_struct->LD1.pin, GPIO_PIN_RESET);
}

// FUELING ====================================================================
void rsms_FUELING_entry() {
	HAL_GPIO_WritePin(rsms_struct->LD2.port, rsms_struct->LD2.pin, GPIO_PIN_SET);
}

void rsms_FUELING_while() {
#ifdef __STM32_TIM_PERIPHERAL
	if (TIMER_itsTime(&rsms_struct->pTim)) {
		RSMS_P_measureData();
		RSMS_P_calcPoly();
		RSMS_P_logData();
	}
	if (TIMER_itsTime(&rsms_struct->tTim)) {
		RSMS_T_measureData();
		RSMS_T_calcPoly();
		RSMS_T_logData();
	}
	if (TIMER_itsTime(&rsms_struct->canStatusTim)) {
		RSMS_BOARD_logAndSendStatus();
	}
	if (TIMER_itsTime(&rsms_struct->canDataTim)) {
		RSMS_P_sendData();
		RSMS_T_sendData();
	}
#endif
}

void rsms_FUELING_exit() {
	HAL_GPIO_WritePin(rsms_struct->LD2.port, rsms_struct->LD2.pin, GPIO_PIN_RESET);
}

// RDY_SET ====================================================================
void rsms_RDY_SET_entry() {
	HAL_GPIO_WritePin(rsms_struct->LD3.port, rsms_struct->LD3.pin, GPIO_PIN_SET);
}

void rsms_RDY_SET_while() {
#ifdef __STM32_TIM_PERIPHERAL
	if (TIMER_itsTime(&rsms_struct->pTim)) {
		RSMS_P_measureData();
		RSMS_P_calcPoly();
		RSMS_P_logData();
	}
	if (TIMER_itsTime(&rsms_struct->tTim)) {
		RSMS_T_measureData();
		RSMS_T_calcPoly();
		RSMS_T_logData();
	}
	if (TIMER_itsTime(&rsms_struct->canStatusTim)) {
		RSMS_BOARD_logAndSendStatus();
	}
	if (TIMER_itsTime(&rsms_struct->canDataTim)) {
		RSMS_P_sendData();
		RSMS_T_sendData();
	}
#endif
}

void rsms_RDY_SET_exit() {
	HAL_GPIO_WritePin(rsms_struct->LD3.port, rsms_struct->LD3.pin, GPIO_PIN_RESET);
}

// FLIGHT ====================================================================
void rsms_FLIGHT_entry() {
	HAL_GPIO_WritePin(rsms_struct->LD4.port, rsms_struct->LD4.pin, GPIO_PIN_SET);
}

void rsms_FLIGHT_while() {
#ifdef __STM32_TIM_PERIPHERAL
	if (TIMER_itsTime(&rsms_struct->pTim)) {
		RSMS_P_measureData();
		RSMS_P_calcPoly();
		RSMS_P_logData();
	}
	if (TIMER_itsTime(&rsms_struct->tTim)) {
		RSMS_T_measureData();
		RSMS_T_calcPoly();
		RSMS_T_logData();
	}
	if (TIMER_itsTime(&rsms_struct->canStatusTim)) {
		RSMS_BOARD_logAndSendStatus();
	}
	if (TIMER_itsTime(&rsms_struct->canDataTim)) {
		RSMS_P_sendData();
		RSMS_T_sendData();
	}
#endif
}

void rsms_FLIGHT_exit() {
	HAL_GPIO_WritePin(rsms_struct->LD4.port, rsms_struct->LD4.pin, GPIO_PIN_RESET);
}

// LANDED ====================================================================
void rsms_LANDED_entry() {
}

void rsms_LANDED_while() {
#ifdef __STM32_TIM_PERIPHERAL
	if (TIMER_itsTime(&rsms_struct->canStatusTim)) {
		RSMS_BOARD_logAndSendStatus();
	}
#endif
}

void rsms_LANDED_exit() {
}

// init ====================================================================
void RSMS_init(RSMS_PeripheralStruct *rsms_PeripheralStruct) {
	rsms_struct = rsms_PeripheralStruct;
	RSMS_BOARD_init(rsms_PeripheralStruct);

	// Init Microcontroller
	initMicrocontroller();
	microcontroller.number = RSMS;
	microcontroller.master = IFC;

	Tasks *currentTasks;

	currentTasks = &getMCState(IDLE)->tasks;
	currentTasks->entry = &rsms_IDLE_entry;
	currentTasks->whileHandle = &rsms_IDLE_while;
	currentTasks->canHandle = &RSMS_BOARD_canHandle;
	currentTasks->exit = &rsms_IDLE_exit;

	currentTasks = &getMCState(FUELING)->tasks;
	currentTasks->entry = &rsms_FUELING_entry;
	currentTasks->whileHandle = &rsms_FUELING_while;
	currentTasks->canHandle = &RSMS_BOARD_canHandle;
	currentTasks->exit = &rsms_FUELING_exit;

	currentTasks = &getMCState(RDY_SET)->tasks;
	currentTasks->entry = &rsms_RDY_SET_entry;
	currentTasks->whileHandle = &rsms_RDY_SET_while;
	currentTasks->canHandle = &RSMS_BOARD_canHandle;
	currentTasks->exit = &rsms_RDY_SET_exit;

	currentTasks = &getMCState(FLIGHT)->tasks;
	currentTasks->entry = &rsms_FLIGHT_entry;
	currentTasks->whileHandle = &rsms_FLIGHT_while;
	currentTasks->canHandle = &RSMS_BOARD_canHandle;
	currentTasks->exit = &rsms_FLIGHT_exit;

	currentTasks = &getMCState(LANDED)->tasks;
	currentTasks->entry = &rsms_LANDED_entry;
	currentTasks->whileHandle = &rsms_LANDED_while;
	currentTasks->canHandle = &RSMS_BOARD_canHandle;
	currentTasks->exit = &rsms_LANDED_exit;
}

