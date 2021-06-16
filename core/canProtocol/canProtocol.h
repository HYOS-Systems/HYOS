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

typedef struct {
	uint32_t extID;
	uint8_t payload[8];
	uint8_t dlc;
} CANP_Package;

typedef struct {
	DATA_ID ID;
	DATA_STATUS status;
} CANP_DataHeader;

typedef struct {
	CANP_DataHeader header;
	uint16_t payload;
} CANP_Data;

typedef struct {
	CANP_DataHeader header;
	MCU_ID mcu;
	MCU_STATUS mcuStatus;
	STATE_ID state;
} CANP_Status;

typedef struct {
	MCU_ID sourceMCU;
	MCU_ID targetMCU;
	MESSAGE_TYPE messageType;
	uint32_t timeStamp;
} CANP_MessageHeader;

typedef struct {
	CANP_MessageHeader header;
	CANP_Data data1;
	CANP_Data data2;
} CANP_DataMessage;

typedef struct {
	CANP_MessageHeader header;
	DATA_ID dataID1;
	DATA_ID dataID2;
	STATE_ID state;
} CANP_RequestDataMessage;

typedef struct {
	CANP_MessageHeader header;
	STATE_ID state;
	uint8_t messageValid;
} CANP_TransitionMessage;

void CANP_unpackHeader(CANP_Package *package, CANP_MessageHeader *mHeader);

void CANP_packData(CANP_Package *package, CANP_DataMessage *message);
void CANP_packRequestData(CANP_Package *package, CANP_RequestDataMessage *message);
void CANP_packTransition(CANP_Package *package, CANP_TransitionMessage *message);

void CANP_unpackDataMessage(CANP_Package *package, CANP_DataMessage *message);
void CANP_unpackRequestDataMessage(CANP_Package *package,CANP_RequestDataMessage *message);
void CANP_unpackTransitionMessage(CANP_Package *package, CANP_TransitionMessage *message);

uint8_t CANP_isStatusData(CANP_Data*);
void CANP_unpackStatus(CANP_Data*, CANP_Status*);
void CANP_packStatus(CANP_Data*, CANP_Status*);
