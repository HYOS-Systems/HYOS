/*
 * canProtocol.c
 *
 *  Created on: 05.04.2021
 *      Author: Bayram
 */

#include "hyendOS/CANProtocol/Impl/canProtocolImpl.h"

#ifdef CANPROTOCOL_H_

CANPackage package;
uint8_t i;

// Receiving -----------------------------------------------------------------------------
MESSAGE_TYPE CANI_receiveMessage(CANBus* bus, MessageHeader* mHeader){
	CAN_ReceiveMessage(bus);
	package.extID = bus->pRxHeader.ExtId;
	CANP_unpackHeader(&package, mHeader);

	return mHeader->messageType;
}

uint8_t CANI_isThisTarget(MessageHeader* mHeader){
	return mHeader->targetMCU == MAX_MCU || mHeader->targetMCU == microcontroller->number;
}

// Sending -----------------------------------------------------------------------------
void CANI_fillHeader(MessageHeader *mHeader) {
	mHeader->sourceMCU = microcontroller->number;
}

void CANI_sendMessage(CANBus* bus, MessageHeader* mHeader, uint8_t* payload){
	setHeader(bus, package.extID);
	CAN_SendMessage(payload, bus);
}

void CANI_sendData(CANBus* bus, DataMessage* message){
	CANI_fillHeader(&message->header);

	CANP_packData(&package, message);
	CANI_sendMessage(bus, &(message->header), package.payload);
}

void CANI_sendState(CANBus* bus, StateMessage* message){
	CANI_fillHeader(&message->header);

	CANP_packState(&package, message);
	CANI_sendMessage(bus, &(message->header), package.payload);
}

void CANI_sendRequestData(CANBus* bus, RequestDataMessage* message){
	CANI_fillHeader(&message->header);

	message->state = microcontroller->state->stateID;

	CANP_packRequestData(&package, message);
	CANI_sendMessage(bus, &(message->header), package.payload);
}

void CANI_sendRequestState(CANBus* bus, StateMessage* message){
	CANI_fillHeader(&message->header);

	CANP_packRequestState(&package, message);
	CANI_sendMessage(bus, &(message->header), package.payload);
}

// Interpreting -----------------------------------------------------------------------------
void CANI_interptetSingleDataFromMessage(Data* data, uint8_t* buffer, uint8_t startIndx){
	data->status = buffer[startIndx] >> 4;
	data->dataType = (0x0F & buffer[startIndx]) << 8 | buffer[startIndx + 1];
	data->payload = buffer[startIndx + 2] << 8 | buffer[startIndx + 3];
}

uint8_t CANI_isStateBus(CANBus* bus){
	return microcontroller->stateBus->number == bus->number;
}

uint8_t CANI_isTransitionHeader(MessageHeader* mHeader){
	return mHeader->messageType == TRANSITION && mHeader->sourceMCU == microcontroller->master;
}

void CANI_copyCANpackage(CANPackage* package, CANBus* bus){
	package->dlc = bus->pRxHeader.DLC;
	package->extID = bus->pRxHeader.ExtId;
	for(i=0; i<package->dlc; i++){
		package->payload[i] = bus->receiveBuffer[i];
	}
}

void CANI_interpretDataMessage(CANBus* bus, DataMessage* message){
	CANI_copyCANpackage(&package, bus);
	CANP_unpackDataMessage(&package, message);
}

void CANI_interpretStateMessage(CANBus* bus, StateMessage* message){
	CANI_copyCANpackage(&package, bus);
	CANP_unpackStateMessage(&package, message);
}

void CANI_interpretRequestDataMessage(CANBus* bus, RequestDataMessage* message){
	CANI_copyCANpackage(&package, bus);
	CANP_unpackRequestDataMessage(&package, message);
}

void CANI_interpretStateRequestMessage(CANBus* bus, StateMessage* message){
	CANI_copyCANpackage(&package, bus);
	CANP_unpackStateMessage(&package, message);
}

void CANI_interpretTransitionMessage(CANBus* bus, TransitionMessage* message){
	CANI_copyCANpackage(&package, bus);
	CANP_unpackTransitionMessage(&package, message);
	if (CANI_isStateBus(bus) && CANI_isTransitionHeader(&message->header) && message->messageValid){
		stateTransition(message->state);
	}
}

#endif
