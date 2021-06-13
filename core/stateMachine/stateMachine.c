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

void initDefaultTask(Tasks* tasks){
	tasks->entry = &voidMethod;
	tasks->whileHandle = &voidMethod;
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

//	microcontroller = &mc;

	/*
	 * What is done after this for-loop?
	 * - every Method Handle has voidMethod
	 * - all MC_States are validly initialized with voidTasks -> getMCState works now
	 * - mc is in NULL_STATE
	 *
	 * What is to be done by user?
	 * - fill method Handles
	 * - define Tasks for MC_States -> can be done as late binding in initHandle()
	 */
	for (int j = 0; j < STATE_ID_END; j++) {
		initDefaultTask(&microcontroller.mcStates[j].tasks);
		microcontroller.mcStates[j].stateID = j;
	}
	microcontroller.state = getMCState(NULL_STATE);
}

Tasks* getTasks() {
	return &microcontroller.state->tasks;
}

MC_State* getMCState(STATE_ID id){
	MC_State* state = &(microcontroller.mcStates[0]);
	for (int i = 0; i < STATE_ID_END; i++){
		MC_State currentState = microcontroller.mcStates[i];
		if(currentState.stateID == id){
			state = &currentState;
			break;
		}
	}
	return state;
}

uint8_t isStateHigher(STATE_ID stateID){
	uint8_t isStateValid = stateID > microcontroller.state->stateID;
	return isStateValid;
}

void stateTransition(STATE_ID stateID){
	if (isStateHigher(stateID)){
		microcontroller.state->tasks.exit();
		microcontroller.state = getMCState(stateID);
		microcontroller.state->tasks.entry();
	}
}

