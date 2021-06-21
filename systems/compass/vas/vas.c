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
}

void vas_IDLE_while() {
	if (TIMER_itsTime(&vas_struct->statusTim)) {
		VAS_BOARD_sendStatus();
	}
}

void vas_IDLE_exit() {
}

// FUELING ====================================================================
void vas_FUELING_entry() {
}

void vas_FUELING_while() {
	if (TIMER_itsTime(&vas_struct->statusTim)) {
		VAS_BOARD_sendStatus();
	}
}

void vas_FUELING_exit() {
}

// RDY_SET ====================================================================
void vas_RDY_SET_entry() {
}

void vas_RDY_SET_while() {
	if (TIMER_itsTime(&vas_struct->statusTim)) {
		VAS_BOARD_sendStatus();
	}
}

void vas_RDY_SET_exit() {
}

// FLIGHT ====================================================================
void vas_FLIGHT_entry() {
}

void vas_FLIGHT_while() {
	if (TIMER_itsTime(&vas_struct->statusTim)) {
		VAS_BOARD_sendStatus();
	}
}

void vas_FLIGHT_exit() {
}

// LANDED ====================================================================
void vas_LANDED_entry() {
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
	microcontroller.master = GSE;

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

