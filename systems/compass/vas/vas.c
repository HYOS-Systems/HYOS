/*
 * rsms.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/vas/vas.h"

VAS_PeripheralStruct *vas_struct;

// IDLE ====================================================================
void vas_IDLE_entry() {
	HAL_GPIO_WritePin(vas_struct->LD1.port, vas_struct->LD1.pin, GPIO_PIN_SET);
}

void vas_IDLE_while() {
	if (TIMER_itsTime(&vas_struct->statusTim)) {
		VAS_BOARD_sendStatus();
	}

	if (TIMER_itsTime(&vas_struct->gpCheckTim)) {
		if (VAS_isGPOn()) {
			VAS_shutdownGP();
		}
	}
}

void vas_IDLE_exit() {
	HAL_GPIO_WritePin(vas_struct->LD1.port, vas_struct->LD1.pin, GPIO_PIN_RESET);
}

// FUELING ====================================================================
void vas_FUELING_entry() {
	HAL_GPIO_WritePin(vas_struct->LD2.port, vas_struct->LD2.pin, GPIO_PIN_SET);
}

void vas_FUELING_while() {
	if (TIMER_itsTime(&vas_struct->statusTim)) {
		VAS_BOARD_sendStatus();
	}
	if (TIMER_itsTime(&vas_struct->gpCheckTim)) {
		if (VAS_isGPOn()) {
			VAS_shutdownGP();
		}
	}
}

void vas_FUELING_exit() {
	HAL_GPIO_WritePin(vas_struct->LD2.port, vas_struct->LD2.pin, GPIO_PIN_RESET);
}

// RDY_SET ====================================================================
void vas_RDY_SET_entry() {
	HAL_GPIO_WritePin(vas_struct->LD3.port, vas_struct->LD3.pin, GPIO_PIN_SET);
	if (!VAS_isGPOn()) {
		VAS_startGP();
		VAS_startGPRecording();
	}
}

void vas_RDY_SET_while() {
	if (TIMER_itsTime(&vas_struct->statusTim)) {
		VAS_BOARD_sendStatus();
	}
	if (TIMER_itsTime(&vas_struct->gpCheckTim)) {
		if (!VAS_isGPOn()) {
			VAS_startGP();
		}
		if (!VAS_isGPRecording()) {
			VAS_startGPRecording();
		}
	}
}

void vas_RDY_SET_exit() {
	HAL_GPIO_WritePin(vas_struct->LD3.port, vas_struct->LD3.pin, GPIO_PIN_RESET);
}

// FLIGHT ====================================================================
void vas_FLIGHT_entry() {
	HAL_GPIO_WritePin(vas_struct->LD4.port, vas_struct->LD4.pin, GPIO_PIN_SET);
}

void vas_FLIGHT_while() {
	if (TIMER_itsTime(&vas_struct->statusTim)) {
		VAS_BOARD_sendStatus();
	}
	if (TIMER_itsTime(&vas_struct->gpCheckTim)) {
		if (!VAS_isGPOn()) {
			VAS_startGP();
		}
		if (!VAS_isGPRecording()) {
			VAS_startGPRecording();
		}
	}
}

void vas_FLIGHT_exit() {
	HAL_GPIO_WritePin(vas_struct->LD4.port, vas_struct->LD4.pin, GPIO_PIN_RESET);
}

// LANDED ====================================================================
void vas_LANDED_entry() {
//	if(VAS_isGPOn()){
//		VAS_shutdownGP();
//	}
}

void vas_LANDED_while() {
	if (TIMER_itsTime(&vas_struct->statusTim)) {
		VAS_BOARD_sendStatus();
	}
}

void vas_LANDED_exit() {
}

// init ====================================================================
void VAS_init(VAS_PeripheralStruct *vas_PeripheralStruct) {
	vas_struct = vas_PeripheralStruct;
	VAS_BOARD_init(vas_PeripheralStruct);

	// Init Microcontroller
	initMicrocontroller();
	microcontroller.number = VAS;
	microcontroller.master = IFC;

	Tasks *currentTasks;

	currentTasks = &getMCState(IDLE)->tasks;
	currentTasks->entry = &vas_IDLE_entry;
	currentTasks->whileHandle = &vas_IDLE_while;
	currentTasks->canHandle = &VAS_BOARD_canHandle;
	currentTasks->exit = &vas_IDLE_exit;

	currentTasks = &getMCState(FUELING)->tasks;
	currentTasks->entry = &vas_FUELING_entry;
	currentTasks->whileHandle = &vas_FUELING_while;
	currentTasks->canHandle = &VAS_BOARD_canHandle;
	currentTasks->exit = &vas_FUELING_exit;

	currentTasks = &getMCState(RDY_SET)->tasks;
	currentTasks->entry = &vas_RDY_SET_entry;
	currentTasks->whileHandle = &vas_RDY_SET_while;
	currentTasks->canHandle = &VAS_BOARD_canHandle;
	currentTasks->exit = &vas_RDY_SET_exit;

	currentTasks = &getMCState(FLIGHT)->tasks;
	currentTasks->entry = &vas_FLIGHT_entry;
	currentTasks->whileHandle = &vas_FLIGHT_while;
	currentTasks->canHandle = &VAS_BOARD_canHandle;
	currentTasks->exit = &vas_FLIGHT_exit;

	currentTasks = &getMCState(LANDED)->tasks;
	currentTasks->entry = &vas_LANDED_entry;
	currentTasks->whileHandle = &vas_LANDED_while;
	currentTasks->canHandle = &VAS_BOARD_canHandle;
	currentTasks->exit = &vas_LANDED_exit;
}

