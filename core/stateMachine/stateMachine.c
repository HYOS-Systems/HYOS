/*
 * stateTransition.c
 *
 *  Created on: 25.03.2021
 *      Author: Bayram
 */

#include "core/stateMachine/stateMachine.h"

void voidMethod() {
	// Empty Method
	// Is empty, shall remain empty...
}

void voidVoidMethod(void *p) {
	// Empty Method
	// Is empty, shall remain empty...
}

void initDefaultTask(volatile Tasks *tasks) {
	tasks->entry = &voidMethod;
	tasks->whileHandle = &voidMethod;
	tasks->canHandle = &voidVoidMethod;
	tasks->exit = &voidMethod;

	tasks->alfaHandle = &voidMethod;
	tasks->bravoHandle = &voidMethod;
	tasks->charlieHandle = &voidMethod;
	tasks->deltaHandle = &voidMethod;
	tasks->echoHandle = &voidMethod;
	tasks->foxtrotHandle = &voidMethod;
}

void initMicrocontroller() {
	xprintf("Initializing MCs\n");

	for (int j = 0; j < STATE_ID_END; j++) {
		initDefaultTask(&microcontroller.mcStates[j].tasks);
		microcontroller.mcStates[j].stateID = j;
	}
	microcontroller.state = getMCState(NULL_STATE);
}

Tasks* getTasks() {
	return &microcontroller.state->tasks;
}

MC_State* getMCState(STATE_ID id) {
	volatile MC_State *state = &(microcontroller.mcStates[0]);
	for (int i = 0; i < STATE_ID_END; i++) {
		MC_State currentState = microcontroller.mcStates[i];
		if (currentState.stateID == id) {
			state = &microcontroller.mcStates[i];
			break;
		}
	}
	return state;
}

uint8_t isStateHigher(STATE_ID stateID) {
	uint8_t isStateValid = stateID > microcontroller.state->stateID;
	return isStateValid;
}

void stateTransition(STATE_ID stateID) {
//	if (isStateHigher(stateID)) { // TODO Evaluate if check of current state is nice...
	microcontroller.state->tasks.exit();
	microcontroller.state = getMCState(stateID);
	microcontroller.state->tasks.entry();
//	}
}

