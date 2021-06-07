/*
 * canProtocol.h
 *
 *  Created on: 05.04.2021
 *      Author: Bayram
 */

#pragma once

#define CANPROTOCOL_H_
#include <core/enums/hyend_os_enums.h>

// ----------------------------------------------------------
// Basic Data Types.
// Use HAL implementation or define own fitting data types
//#include <drivers/HAL/HALVersioning.h>
#include <stdint.h>
// Choose DataTypes to fit the bit size.
//typedef unsigned char uint8_t;
//typedef unsigned short uint16_t;
//typedef unsigned long uint32_t;
//typedef long unsigned long uint64_t;
// ----------------------------------------------------------

typedef enum {
	DATA_OK = 0,
	DATA_OLD,
	DATA_MAX = 15
} DATA_STATUS;

typedef struct {
	uint32_t extID;
	uint8_t payload[8];
	uint8_t dlc;
} CANPackage;

typedef struct {
	DATA_ID dataType;
	DATA_STATUS status;
	uint16_t payload;
} Data;

typedef struct {
	MICROCONTROLLER sourceMCU;
	MICROCONTROLLER targetMCU;
	MESSAGE_TYPE messageType;
	uint16_t timestamp;
} MessageHeader;

typedef struct {
	MessageHeader header;
	Data data1;
	Data data2;
} DataMessage;

typedef struct {
	MessageHeader header;
	STATE_ID state;
} StateMessage;

typedef struct {
	MessageHeader header;
	DATA_ID dataID1;
	DATA_ID dataID2;
	STATE_ID state;
} RequestDataMessage;

typedef struct {
	MessageHeader header;
	STATE_ID state;
	uint8_t messageValid;
} TransitionMessage;

MESSAGE_TYPE CANP_unpackHeader(CANPackage *package, MessageHeader *mHeader);

void CANP_packData(CANPackage *package, DataMessage *message);
void CANP_packState(CANPackage *package, StateMessage *message);
void CANP_packRequestData(CANPackage *package, RequestDataMessage *message);
void CANP_packRequestState(CANPackage *package, StateMessage *message);
void CANP_packStateTransitionMessage(CANPackage *package, TransitionMessage *message);

void CANP_unpackDataMessage(CANPackage *package, DataMessage *message);
void CANP_unpackStateMessage(CANPackage *package, StateMessage *message);
void CANP_unpackRequestDataMessage(CANPackage *package, RequestDataMessage *message);
void CANP_unpackRequestStateMessage(CANPackage *package, StateMessage *message);
void CANP_unpackTransitionMessage(CANPackage *package, TransitionMessage *message);
