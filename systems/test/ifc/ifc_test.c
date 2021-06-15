/*
 * rsms.c
 *
 *  Created on: 17.01.2021
 *      Author: Bayram
 */

#include "systems/test/ifc/ifc_test.h"

#ifdef HYENDOS_SYSTEMS_IFC_TEST_H_

IFC_Test_PinStruct *ifc_struct;

// IDLE ====================================================================
void ifc_test_IDLE_entry() {
	IFC_TEST_blinkLED(ifc_struct->LD_Yellow);
	IFC_TEST_orderTransition(IDLE);
}

void ifc_test_IDLE_while() {
	HAL_GPIO_WritePin(ifc_struct->LD_Yellow.port, ifc_struct->LD_Yellow.pin,
			GPIO_PIN_SET);
}

void ifc_test_IDLE_exit() {
	IFC_TEST_blinkLED(ifc_struct->LD_Yellow);
}

// FUELING ====================================================================
void ifc_test_FUELING_entry() {
	IFC_TEST_blinkLED(ifc_struct->LD_Green);
	IFC_TEST_orderTransition(FUELING);
}

void ifc_test_FUELING_while() {
	HAL_GPIO_WritePin(ifc_struct->LD_Green.port, ifc_struct->LD_Green.pin,
			GPIO_PIN_SET);
}

void ifc_test_FUELING_exit() {
	IFC_TEST_blinkLED(ifc_struct->LD_Green);
}

// RDY_SET ====================================================================
void ifc_test_RDY_SET_entry() {
	IFC_TEST_blinkLED(ifc_struct->LD_Red);
	IFC_TEST_orderTransition(RDY_SET);
}

void ifc_test_RDY_SET_while() {
	HAL_GPIO_WritePin(ifc_struct->LD_Red.port, ifc_struct->LD_Red.pin,
			GPIO_PIN_SET);
}

void ifc_test_RDY_SET_exit() {
	IFC_TEST_blinkLED(ifc_struct->LD_Red);
}

// FLIGHT ====================================================================
void ifc_test_FLIGHT_entry() {
	IFC_TEST_blinkLED(ifc_struct->LD_Blue);
	IFC_TEST_orderTransition(FLIGHT);
}

void ifc_test_FLIGHT_while() {
	HAL_GPIO_WritePin(ifc_struct->LD_Blue.port, ifc_struct->LD_Blue.pin,
			GPIO_PIN_SET);
}

void ifc_test_FLIGHT_exit() {
	IFC_TEST_blinkLED(ifc_struct->LD_Blue);
}

// LANDED ====================================================================
void ifc_test_LANDED_entry() {
	IFC_TEST_blinkLED(ifc_struct->LD_White);
	IFC_TEST_orderTransition(LANDED);
}

void ifc_test_LANDED_while() {
	HAL_GPIO_WritePin(ifc_struct->LD_White.port, ifc_struct->LD_White.pin,
			GPIO_PIN_SET);
}

void ifc_test_LANDED_exit() {
	IFC_TEST_blinkLED(ifc_struct->LD_White);
}

// init ====================================================================
void IFC_TEST_init(IFC_Test_PinStruct *ifc_test_struct) {
	ifc_struct = ifc_test_struct;
	IFC_TEST_tasks_init(ifc_test_struct);

	// Init Microcontroller
	initMicrocontroller();
	microcontroller.number = IFC;
	microcontroller.master = GSE;

	Tasks *currentTasks;

	currentTasks = &(getMCState(IDLE)->tasks);
	currentTasks->entry = &ifc_test_IDLE_entry;
	currentTasks->whileHandle = &ifc_test_IDLE_while;
	currentTasks->canHandle = &IFC_TEST_canHandle;
	currentTasks->exit = &ifc_test_IDLE_exit;

	currentTasks = &(getMCState(FUELING)->tasks);
	currentTasks->entry = &ifc_test_FUELING_entry;
	currentTasks->whileHandle = &ifc_test_FUELING_while;
	currentTasks->canHandle = &IFC_TEST_canHandle;
	currentTasks->exit = &ifc_test_FUELING_exit;

	currentTasks = &(getMCState(RDY_SET)->tasks);
	currentTasks->entry = &ifc_test_RDY_SET_entry;
	currentTasks->whileHandle = &ifc_test_RDY_SET_while;
	currentTasks->canHandle = &IFC_TEST_canHandle;
	currentTasks->exit = &ifc_test_RDY_SET_exit;

	currentTasks = &getMCState(FLIGHT)->tasks;
	currentTasks->entry = &ifc_test_FLIGHT_entry;
	currentTasks->whileHandle = &ifc_test_FLIGHT_while;
	currentTasks->canHandle = &IFC_TEST_canHandle;
	currentTasks->exit = &ifc_test_FLIGHT_exit;

	currentTasks = &getMCState(LANDED)->tasks;
	currentTasks->entry = &ifc_test_LANDED_entry;
	currentTasks->whileHandle = &ifc_test_LANDED_while;
	currentTasks->canHandle = &IFC_TEST_canHandle;
	currentTasks->exit = &ifc_test_LANDED_exit;
}



#endif
