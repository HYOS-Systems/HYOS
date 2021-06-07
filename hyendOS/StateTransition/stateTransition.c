/*
 * stateTransition.c
 *
 *  Created on: 25.03.2021
 *      Author: Bayram
 */

#include "hyendOS/StateTransition/stateTranstion.h"

Tasks* getTasks() {
	return &microcontroller->state->tasks;
}

MC_State* getMCState(STATE_ID id){
	MC_State* state = &(microcontroller->mcStates[0]);
	for (int i = 0; i < STATE_ID_END; i++){
		MC_State currentState = microcontroller->mcStates[i];
		if(currentState.stateID == id){
			state = &currentState;
			break;
		}
	}
	return state;
}

uint8_t isStateHigher(STATE_ID stateID){
	uint8_t isStateValid = stateID > microcontroller->state->stateID;
	return isStateValid;
}

void stateTransition(STATE_ID stateID){
	if (isStateHigher(stateID)){
		microcontroller->state->tasks.exit();
		microcontroller->state = getMCState(stateID);
		microcontroller->state->tasks.entry();
	}
}

