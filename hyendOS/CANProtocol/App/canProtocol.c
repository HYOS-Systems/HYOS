/*
 * canProtocol.c
 *
 *  Created on: 05.04.2021
 *      Author: Bayram
 */

#include "hyendOS/CANProtocol/App/canProtocol.h"

#ifdef CANPROTOCOL_H_

static const uint8_t sourceOffset = 26;
static const uint8_t targetOffset = 23;
static const uint8_t meTypeOffset = 20;
static const uint8_t timeFrOffset = 20;

static const uint32_t sourceMask = 0x00000007;
static const uint32_t targetMask = 0x00000007;
static const uint32_t meTypeMask = 0x00000007;
static const uint32_t timeFrMask = 0x000FFFFF;

// Receiving -----------------------------------------------------------------------------
MESSAGE_TYPE receiveMessage(CANBus* bus, MessageHeader* mHeader){
	CAN_ReceiveMessage(bus);
	uint32_t header = bus->pRxHeader.ExtId;
	mHeader->sourceMCU 	 = sourceMask & header >> sourceOffset;
	mHeader->targetMCU 	 = targetMask & header >> targetOffset;
	mHeader->messageType = meTypeMask & header >> meTypeOffset;
	mHeader->timestamp 	 = timeFrMask & header >> timeFrOffset;
	return mHeader->messageType;
}

uint8_t isThisTarget(MessageHeader* mHeader){
	return mHeader->targetMCU == MAX_MCU || mHeader->targetMCU == microcontroller->number;
}

// Sending -----------------------------------------------------------------------------
uint32_t buildHeader(MessageHeader* mHeader){
	uint32_t header =
			(sourceMask & microcontroller->number << sourceOffset) |
			(targetMask & mHeader->targetMCU << targetOffset) |
			(meTypeMask & mHeader->messageType << meTypeOffset) |
			(timeFrMask & mHeader->timestamp);
	return header;
}

void buildHalfPayload(uint8_t* payload, Data* data, uint8_t startIndx){
	payload[startIndx + 0] = data->status << 4 | (0x0F & data->dataType >> 8);
	payload[startIndx + 1] = 0xFF & data->dataType;
	payload[startIndx + 2] = 0xFF & data->payload >> 8;
	payload[startIndx + 3] = 0xFF & data->payload;
}

void sendMessage(CANBus* bus, MessageHeader* mHeader, uint8_t* payload){
	uint32_t header = buildHeader(mHeader);

	setHeader(bus, header);
	CAN_SendMessage(payload, bus);
}

void sendData(CANBus* bus, DataMessage* message){
	message->header.messageType = DATA;

	uint8_t payload[8];
	buildHalfPayload(payload, &(message->data1), 0);
	buildHalfPayload(payload, &(message->data2), 4);

	sendMessage(bus, &(message->header), payload);
}

void sendState(CANBus* bus, StateMessage* message){
	message->header.messageType = STATUS;

	uint8_t payload[8];
	payload[6] = message->state >> 8;
	payload[7] = message->state;

	sendMessage(bus, &(message->header), payload);
}

void sendRequestData(CANBus* bus, RequestDataMessage* message){
	message->header.messageType = REQUEST_DATA;

	uint8_t payload[8];
	payload[0] = DATA_OK << 4 | (message->dataID1 >> 8 & 0x0F);
	payload[1] = message->dataID1;
	payload[2] = microcontroller->state->stateID >> 8;
	payload[3] = microcontroller->state->stateID;
	payload[4] = DATA_OK << 4 | (message->dataID2 >> 8 & 0x0F);
	payload[5] = message->dataID2;
	payload[6] = microcontroller->state->stateID >> 8;
	payload[7] = microcontroller->state->stateID;

	sendMessage(bus, &(message->header), payload);
}

void sendRequestState(CANBus* bus, StateMessage* message){
	message->header.messageType = REQUEST_STATUS;

	uint8_t payload[8];
	payload[6] = message->state >> 8;
	payload[7] = message->state;

	sendMessage(bus, &(message->header), payload);
}

// Interpreting -----------------------------------------------------------------------------
void interptetSingleDataFromMessage(Data* data, uint8_t* buffer, uint8_t startIndx){
	data->status = buffer[startIndx] >> 4;
	data->dataType = (0x0F & buffer[startIndx]) << 8 | buffer[startIndx + 1];
	data->payload = buffer[startIndx + 2] << 8 | buffer[startIndx + 3];
}

STATE_ID getStateIDfromMessage(CANBus* bus){
	return (0xF & bus->receiveBuffer[6]) << 8 | bus->receiveBuffer[7];
}

uint8_t isStateBus(CANBus* bus){
	return microcontroller->stateBus->number == bus->number;
}

uint8_t isTransitionHeader(MessageHeader* mHeader){
	return mHeader->messageType == TRANSITION && mHeader->sourceMCU == microcontroller->master;
}

uint8_t isTransitionMessageValid(){
	uint8_t* message = microcontroller->stateBus->receiveBuffer;
	MESSAGE_TYPE dType1 = message[0] >> 4;
	MESSAGE_TYPE dType2 = message[0] & 0x0F;
	MESSAGE_TYPE dType3 = message[1] >> 4;
	MESSAGE_TYPE dType4 = message[1] & 0x0F;
	uint8_t firstHalfIsTransition =
			dType1 == TRANSITION &&
			dType2 == TRANSITION &&
			dType3 == TRANSITION &&
			dType4 == TRANSITION;

	STATE_ID state1 = message[2] << 8 | message[3];
	STATE_ID state2 = message[4] << 8 | message[5];
	STATE_ID state3 = message[6] << 8 | message[7];
	uint8_t isStatesConsistent = state1 == state2 && state2 == state3;
	return firstHalfIsTransition && isStatesConsistent;
}

void interpretDataMessage(CANBus* bus, DataMessage* currentMessage){
	interptetSingleDataFromMessage(&(currentMessage->data1), bus->receiveBuffer, 0);
	interptetSingleDataFromMessage(&(currentMessage->data2), bus->receiveBuffer, 4);
}

void interpretStateMessage(CANBus* bus, StateMessage* message){
	message->state = getStateIDfromMessage(bus);
}

void interpretRequestDataMessage(CANBus* bus, RequestDataMessage* message){
	message->state = getStateIDfromMessage(bus);
	message->dataID1 = (bus->receiveBuffer[0] & 0x0F) << 8 | bus->receiveBuffer[1];
	message->dataID2 = (bus->receiveBuffer[4] & 0x0F) << 8 | bus->receiveBuffer[5];
}

void interpretStateRequestMessage(CANBus* bus, StateMessage* message){
	interpretStateMessage(bus, message);
}

void interpretTransitionMessage(CANBus* bus, MessageHeader* mHeader){
	if (isStateBus(bus) && isTransitionHeader(mHeader) && isTransitionMessageValid()){
		stateTransition(getStateIDfromMessage(microcontroller->stateBus));
	}
}

#endif
