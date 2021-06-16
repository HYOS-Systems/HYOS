/*
 * canProtocol.c
 *
 *  Created on: 05.04.2021
 *      Author: Bayram
 */

#include "hyendOS/canProtocolImplementation/canProtocolImpl.h"

#ifdef CANPROTOCOLIMPL_H_

CANP_Package package;
uint8_t i;

// Receiving -----------------------------------------------------------------------------
void CANI_receiveMessage(CANBus *bus, CANP_MessageHeader *mHeader) {
	CAN_ReceiveMessage(bus);
	package.extID = bus->pRxHeader.ExtId;
	CANP_unpackHeader(&package, mHeader);
}

uint8_t CANI_isThisTarget(CANP_MessageHeader *mHeader) {
	return mHeader->targetMCU == MAX_MCU || mHeader->targetMCU == microcontroller.number;
}

// Sending -----------------------------------------------------------------------------
void CANI_fillHeader(CANP_MessageHeader *mHeader) {
	mHeader->sourceMCU = microcontroller.number;
}

void CANI_sendMessage(CANBus *bus, CANP_MessageHeader *mHeader, uint8_t *payload) {
	CAN_setHeader(bus, package.extID);
	CAN_SendMessage(payload, bus);
}

void CANI_sendData(CANBus *bus, CANP_DataMessage *message) {
	CANI_fillHeader(&message->header);

	CANP_packData(&package, message);
	CANI_sendMessage(bus, &(message->header), package.payload);
}

void CANI_sendRequestData(CANBus *bus, CANP_RequestDataMessage *message) {
	CANI_fillHeader(&message->header);

	message->state = microcontroller.state->stateID;

	CANP_packRequestData(&package, message);
	CANI_sendMessage(bus, &(message->header), package.payload);
}

void CANI_sendTransition(CANBus *bus, CANP_TransitionMessage *message) {
	CANI_fillHeader(&message->header);

	CANP_packTransition(&package, message);
	CANI_sendMessage(bus, &(message->header), package.payload);
}

// Interpreting -----------------------------------------------------------------------------
uint8_t CANI_isMaster(CANP_MessageHeader *mHeader) {
	return mHeader->sourceMCU == microcontroller.master;
}

uint8_t CANI_isTransitionHeader(CANP_MessageHeader *mHeader) {
	return mHeader->messageType == TRANSITION;
}

void CANI_copyCANpackage(CANP_Package *package, CANBus *bus) {
	package->dlc = bus->pRxHeader.DLC;
	package->extID = bus->pRxHeader.ExtId;
	for (i = 0; i < package->dlc; i++) {
		package->payload[i] = bus->receiveBuffer[i];
	}
}

void CANI_interpretDataMessage(CANBus *bus, CANP_DataMessage *message) {
	CANI_copyCANpackage(&package, bus);
	CANP_unpackDataMessage(&package, message);
}

void CANI_interpretRequestDataMessage(CANBus *bus, CANP_RequestDataMessage *message) {
	CANI_copyCANpackage(&package, bus);
	CANP_unpackRequestDataMessage(&package, message);
}

void CANI_interpretTransitionMessage(CANBus *bus, CANP_TransitionMessage *message) {
	CANI_copyCANpackage(&package, bus);
	CANP_unpackTransitionMessage(&package, message);
	if (CANI_isMaster(&message->header) && CANI_isTransitionHeader(&message->header) && message->messageValid) {
		stateTransition(message->state);
	}
}

uint8_t CANI_isTransition(CANP_Data *data) {
	return CANP_isStatusData(data);
}

void CANI_unpackStatus(CANP_Data *data, CANP_Status *status) {
	CANP_unpackStatus(data, status);
}

void CANI_packStatus(CANP_Data *data, CANP_Status *status) {
	CANP_packStatus(data, status);
}

#endif
