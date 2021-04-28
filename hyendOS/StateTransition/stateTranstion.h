/*
 * stateTranstion.h
 *
 *  Created on: 25.03.2021
 *      Author: Bayram
 */
#pragma once

#include <hyendOS/OSVersioning/App/HALVersioning.h>
#include "connectivity/canTri/App/canTri.h"
#include "hyendOS/CANProtocol/App/canProtocol.h"

#define INC_STATE_TRANSITION_

typedef struct {
	void (*entry)(void);
	void (*alfaHandle)(void);
	void (*bravoHandle)(void);
	void (*charlieHandle)(void);
	void (*deltaHandle)(void);
	void (*echoHandle)(void);
	void (*foxtrotHandle)(void);
	void (*exit)(void);
} Tasks;

typedef struct {
	Tasks *tasks;
	STATE_ID stateID;
} MC_State;

typedef struct {
	MC_State *state;
	MICROCONTROLLER number;
	MICROCONTROLLER master;
	MC_State mcStates[STATE_ID_END];
	CANBus *stateBus;
	void (*initHandle)(void);
	void (*canNVICHandle)(CANBus*);
} Microcontroller;

Tasks voidTasks;
MC_State voidMCState;

Microcontroller *microcontroller;

MC_State* getMCState(STATE_ID);
Tasks* getTasks(void);
void stateTransition(STATE_ID);