/*
 * canProtocol.h
 *
 *  Created on: 05.04.2021
 *      Author: Bayram
 */

#pragma once

#include <hyendOS/OSVersioning/App/HALVersioning.h>
#include <hyendOS/StateTransition/stateTranstion.h>
#include "connectivity/canTri/App/canTri.h"

#ifdef INC_CANTRI_H_
#define CANPROTOCOL_H_
#endif

typedef enum {
	DATA_OK = 0,
	DATA_OLD,
	DATA_MAX = 15
} DATA_STATUS;

typedef struct {
	DATA_ID dataType;
	DATA_STATUS status;
	uint16_t payload;
} Data;

typedef struct{
	MICROCONTROLLER sourceMCU;
	MICROCONTROLLER targetMCU;
	MESSAGE_TYPE messageType;
	uint16_t timestamp;
} MessageHeader;

typedef struct{
	MessageHeader header;
	Data data1;
	Data data2;
} DataMessage;

typedef struct{
	MessageHeader header;
	STATE_ID state;
} StateMessage;

typedef struct{
	MessageHeader header;
	DATA_ID dataID1;
	DATA_ID dataID2;
	STATE_ID state;
} RequestDataMessage;

MESSAGE_TYPE receiveMessage(CANBus*, MessageHeader*);
uint8_t isThisTarget(MessageHeader*);

void sendData(CANBus*, DataMessage*);
void sendState(CANBus*, StateMessage*);
void sendRequestData(CANBus*, RequestDataMessage*);
void sendRequestState(CANBus*, StateMessage*);

void interpretDataMessage(CANBus*, DataMessage*);
void interpretStateMessage(CANBus*, StateMessage*);
void interpretRequestDataMessage(CANBus*, RequestDataMessage*);
void interpretRequestStateMessage(CANBus*, StateMessage*);
void interpretStateTransitionMessage(CANBus*, MessageHeader*);
