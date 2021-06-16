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
}

void rsms_IDLE_while() {
}

void rsms_IDLE_exit() {
}

// FUELING ====================================================================
void rsms_FUELING_entry() {
}

void rsms_FUELING_while() {
}

void rsms_FUELING_exit() {
}

// RDY_SET ====================================================================
void rsms_RDY_SET_entry() {
}

void rsms_RDY_SET_while() {
}

void rsms_RDY_SET_exit() {
}

// FLIGHT ====================================================================
void rsms_FLIGHT_entry() {
}

void rsms_FLIGHT_while() {
}

void rsms_FLIGHT_exit() {
}

// LANDED ====================================================================
void rsms_LANDED_entry() {
}

void rsms_LANDED_while() {
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





