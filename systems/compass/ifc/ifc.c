/*
 * rsms.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/ifc/ifc.h"

IFC_PeripheralStruct *ifc_struct;

// IDLE ====================================================================
void ifc_IDLE_entry() {
}

void ifc_IDLE_while() {
}

void ifc_IDLE_exit() {
}

// FUELING ====================================================================
void ifc_FUELING_entry() {
}

void ifc_FUELING_while() {
}

void ifc_FUELING_exit() {
}

// RDY_SET ====================================================================
void ifc_RDY_SET_entry() {
}

void ifc_RDY_SET_while() {
}

void ifc_RDY_SET_exit() {
}

// FLIGHT ====================================================================
void ifc_FLIGHT_entry() {
}

void ifc_FLIGHT_while() {
}

void ifc_FLIGHT_exit() {
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
	microcontroller.number = RSMS;
	microcontroller.master = IFC;

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





