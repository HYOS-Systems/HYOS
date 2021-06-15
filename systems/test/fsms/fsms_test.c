/*
 * rsms.c
 *
 *  Created on: 17.01.2021
 *      Author: Bayram
 */

#include "systems/test/fsms/fsms_test.h"

#ifdef HYENDOS_SYSTEMS_FSMS_TEST_H_

FSMS_Test_PinStruct *fsms_struct;

// IDLE ====================================================================
void fsms_test_IDLE_entry() {
	FSMS_TEST_blinkLED(fsms_struct->LD_Yellow);
}

void fsms_test_IDLE_while() {
	HAL_GPIO_WritePin(fsms_struct->LD_Yellow.port, fsms_struct->LD_Yellow.pin,
			GPIO_PIN_SET);
}

void fsms_test_IDLE_exit() {
	FSMS_TEST_blinkLED(fsms_struct->LD_Yellow);
}

// FUELING ====================================================================
void fsms_test_FUELING_entry() {
	FSMS_TEST_blinkLED(fsms_struct->LD_Green);
}

void fsms_test_FUELING_while() {
	HAL_GPIO_WritePin(fsms_struct->LD_Green.port, fsms_struct->LD_Green.pin,
			GPIO_PIN_SET);
}

void fsms_test_FUELING_exit() {
	FSMS_TEST_blinkLED(fsms_struct->LD_Green);
}

// RDY_SET ====================================================================
void fsms_test_RDY_SET_entry() {
	FSMS_TEST_blinkLED(fsms_struct->LD_Red);
}

void fsms_test_RDY_SET_while() {
	HAL_GPIO_WritePin(fsms_struct->LD_Red.port, fsms_struct->LD_Red.pin,
			GPIO_PIN_SET);
}

void fsms_test_RDY_SET_exit() {
	FSMS_TEST_blinkLED(fsms_struct->LD_Red);
}

// FLIGHT ====================================================================
void fsms_test_FLIGHT_entry() {
	FSMS_TEST_blinkLED(fsms_struct->LD_Blue);
}

void fsms_test_FLIGHT_while() {
	FAL_GPIO_WritePin(fsms_struct->LD_Blue.port, fsms_struct->LD_Blue.pin,
			GPIO_PIN_SET);
}

void fsms_test_FLIGHT_exit() {
	FSMS_TEST_blinkLED(fsms_struct->LD_Blue);
}

// LANDED ====================================================================
void fsms_test_LANDED_entry() {
	FSMS_TEST_blinkLED(fsms_struct->LD_White);
}

void fsms_test_LANDED_while() {
	HAL_GPIO_WritePin(fsms_struct->LD_White.port, fsms_struct->LD_White.pin,
			GPIO_PIN_SET);
}

void fsms_test_LANDED_exit() {
	FSMS_TEST_blinkLED(fsms_struct->LD_White);
}

// init ====================================================================
void FSMS_TEST_init(FSMS_Test_PinStruct *fsms_test_struct) {
	fsms_struct = fsms_test_struct;
	FSMS_TEST_tasks_init(fsms_test_struct);

	// Init Microcontroller
	initMicrocontroller();
	microcontroller.number = RSMS;
	microcontroller.master = IFC;

	Tasks *currentTasks;

	currentTasks = &getMCState(IDLE)->tasks;
	currentTasks->entry = &fsms_test_IDLE_entry;
	currentTasks->whileHandle = &fsms_test_IDLE_while;
	currentTasks->canHandle = &FSMS_TEST_canHandle;
	currentTasks->exit = &fsms_test_IDLE_exit;

	currentTasks = &getMCState(FUELING)->tasks;
	currentTasks->entry = &fsms_test_FUELING_entry;
	currentTasks->whileHandle = &fsms_test_FUELING_while;
	currentTasks->canHandle = &FSMS_TEST_canHandle;
	currentTasks->exit = &fsms_test_FUELING_exit;

	currentTasks = &getMCState(RDY_SET)->tasks;
	currentTasks->entry = &fsms_test_RDY_SET_entry;
	currentTasks->whileHandle = &fsms_test_RDY_SET_while;
	currentTasks->canHandle = &FSMS_TEST_canHandle;
	currentTasks->exit = &fsms_test_RDY_SET_exit;

	currentTasks = &getMCState(FLIGHT)->tasks;
	currentTasks->entry = &fsms_test_FLIGHT_entry;
	currentTasks->whileHandle = &fsms_test_FLIGHT_while;
	currentTasks->canHandle = &FSMS_TEST_canHandle;
	currentTasks->exit = &fsms_test_FLIGHT_exit;

	currentTasks = &getMCState(LANDED)->tasks;
	currentTasks->entry = &fsms_test_LANDED_entry;
	currentTasks->whileHandle = &fsms_test_LANDED_while;
	currentTasks->canHandle = &FSMS_TEST_canHandle;
	currentTasks->exit = &fsms_test_LANDED_exit;
}



#endif
