/*
 * rsms_states_test.c
 *
 *  Created on: 16.04.2021
 *      Author: Bayram
 */

#include "hyendOS/systems/test/rsms/rsms_states_test.h"

void rsms_test_initStates() {
	MC_State *currState;

	currState = getMCState(IDLE);
//	currState->tasks->entry = &voidMethod;
//	currState->tasks->alfaHandle = &voidMethod;
//	currState->tasks->bravoHandle = &voidMethod;
//	currState->tasks->charlieHandle = &voidMethod;
//	currState->tasks->deltaHandle = &voidMethod;
//	currState->tasks->echoHandle = &voidMethod;
//	currState->tasks->foxtrotHandle = &voidMethod;
//	currState->tasks->exit = &voidMethod;

	currState = getMCState(QUICK_START);

	currState = getMCState(LANDED);
	currState->tasks = &voidTasks;
}
