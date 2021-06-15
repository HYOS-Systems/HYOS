/*
 * rsms.c
 *
 *  Created on: 17.01.2021
 *      Author: Bayram
 */

#include "systems/test/rsms/rsms_test.h"

#ifdef HYENDOS_SYSTEMS_RSMS_TEST_H_

RSMS_Test_PinStruct *rsms_struct;

// IDLE ====================================================================
void rsms_test_IDLE_entry() {
	RSMS_TEST_blinkLED(rsms_struct->LD_Yellow);
}

void rsms_test_IDLE_while() {
	HAL_GPIO_WritePin(rsms_struct->LD_Yellow.port, rsms_struct->LD_Yellow.pin,
			GPIO_PIN_SET);
}

void rsms_test_IDLE_exit() {
	RSMS_TEST_blinkLED(rsms_struct->LD_Yellow);
}

// FUELING ====================================================================
void rsms_test_FUELING_entry() {
	RSMS_TEST_blinkLED(rsms_struct->LD_Green);
}

void rsms_test_FUELING_while() {
	HAL_GPIO_WritePin(rsms_struct->LD_Green.port, rsms_struct->LD_Green.pin,
			GPIO_PIN_SET);
}

void rsms_test_FUELING_exit() {
	RSMS_TEST_blinkLED(rsms_struct->LD_Green);
}

// RDY_SET ====================================================================
void rsms_test_RDY_SET_entry() {
	RSMS_TEST_blinkLED(rsms_struct->LD_Red);
}

void rsms_test_RDY_SET_while() {
	HAL_GPIO_WritePin(rsms_struct->LD_Red.port, rsms_struct->LD_Red.pin,
			GPIO_PIN_SET);
}

void rsms_test_RDY_SET_exit() {
	RSMS_TEST_blinkLED(rsms_struct->LD_Red);
}

// FLIGHT ====================================================================
void rsms_test_FLIGHT_entry() {
	RSMS_TEST_blinkLED(rsms_struct->LD_Blue);
}

void rsms_test_FLIGHT_while() {
	HAL_GPIO_WritePin(rsms_struct->LD_Blue.port, rsms_struct->LD_Blue.pin,
			GPIO_PIN_SET);
}

void rsms_test_FLIGHT_exit() {
	RSMS_TEST_blinkLED(rsms_struct->LD_Blue);
}

// LANDED ====================================================================
void rsms_test_LANDED_entry() {
	RSMS_TEST_blinkLED(rsms_struct->LD_White);
}

void rsms_test_LANDED_while() {
	HAL_GPIO_WritePin(rsms_struct->LD_White.port, rsms_struct->LD_White.pin,
			GPIO_PIN_SET);
}

void rsms_test_LANDED_exit() {
	RSMS_TEST_blinkLED(rsms_struct->LD_White);
}

// init ====================================================================
void RSMS_TEST_init(RSMS_Test_PinStruct *rsms_test_struct) {
	rsms_struct = rsms_test_struct;
	RSMS_TEST_tasks_init(rsms_test_struct);

	// Init Microcontroller
	initMicrocontroller();
	microcontroller.number = RSMS;
	microcontroller.master = IFC;

	Tasks *currentTasks;

	currentTasks = &getMCState(IDLE)->tasks;
	currentTasks->entry = &rsms_test_IDLE_entry;
	currentTasks->whileHandle = &rsms_test_IDLE_while;
	currentTasks->canHandle = &RSMS_TEST_canHandle;
	currentTasks->exit = &rsms_test_IDLE_exit;

	currentTasks = &getMCState(FUELING)->tasks;
	currentTasks->entry = &rsms_test_FUELING_entry;
	currentTasks->whileHandle = &rsms_test_FUELING_while;
	currentTasks->canHandle = &RSMS_TEST_canHandle;
	currentTasks->exit = &rsms_test_FUELING_exit;

	currentTasks = &getMCState(RDY_SET)->tasks;
	currentTasks->entry = &rsms_test_RDY_SET_entry;
	currentTasks->whileHandle = &rsms_test_RDY_SET_while;
	currentTasks->canHandle = &RSMS_TEST_canHandle;
	currentTasks->exit = &rsms_test_RDY_SET_exit;

	currentTasks = &getMCState(FLIGHT)->tasks;
	currentTasks->entry = &rsms_test_FLIGHT_entry;
	currentTasks->whileHandle = &rsms_test_FLIGHT_while;
	currentTasks->canHandle = &RSMS_TEST_canHandle;
	currentTasks->exit = &rsms_test_FLIGHT_exit;

	currentTasks = &getMCState(LANDED)->tasks;
	currentTasks->entry = &rsms_test_LANDED_entry;
	currentTasks->whileHandle = &rsms_test_LANDED_while;
	currentTasks->canHandle = &RSMS_TEST_canHandle;
	currentTasks->exit = &rsms_test_LANDED_exit;
}



#endif
