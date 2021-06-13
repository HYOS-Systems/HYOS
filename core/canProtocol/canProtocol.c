/*
 * canProtocol.c
 *
 *  Created on: 05.04.2021
 *      Author: Bayram
 */

#include "core/canProtocol/canProtocol.h"

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
MESSAGE_TYPE CANP_unpackHeader(CANP_Package *package, CANP_MessageHeader *mHeader) {
	uint32_t header = package->extID;
	mHeader->sourceMCU = sourceMask & header >> sourceOffset;
	mHeader->targetMCU = targetMask & header >> targetOffset;
	mHeader->messageType = meTypeMask & header >> meTypeOffset;
	mHeader->timeStamp = timeFrMask & header >> timeFrOffset;
	return mHeader->messageType;
}

// Sending -----------------------------------------------------------------------------
uint32_t CANP_packHeader(CANP_MessageHeader *mHeader) {
	uint32_t header = (sourceMask & mHeader->sourceMCU << sourceOffset)
			| (targetMask & mHeader->targetMCU << targetOffset)
			| (meTypeMask & mHeader->messageType << meTypeOffset)
			| (timeFrMask & mHeader->timeStamp);
	return header;
}

void buildHalfPayload(uint8_t *payload, CANP_Data *data, uint8_t startIndx) {
	payload[startIndx + 0] = data->dataHeader.dataStatus << 4 | (0x0F & data->dataHeader.dataType >> 8);
	payload[startIndx + 1] = 0xFF & data->dataHeader.dataType;
	payload[startIndx + 2] = 0xFF & data->payload >> 8;
	payload[startIndx + 3] = 0xFF & data->payload;
}

void CANP_packMessage(CANP_Package *package, CANP_MessageHeader *mHeader, uint8_t *payload) {
	package->extID = CANP_packHeader(mHeader);

	for (uint8_t i = 1; i < 8; i++) {
		package->payload[i] = payload[i];
	}
}

void CANP_packData(CANP_Package *package, CANP_DataMessage *message) {
	message->header.messageType = DATA;

	uint8_t payload[8];
	buildHalfPayload(payload, &(message->data1), 0);
	buildHalfPayload(payload, &(message->data2), 4);

	CANP_packMessage(package, &(message->header), payload);
}

void CANP_packRequestData(CANP_Package *package, CANP_RequestDataMessage *message) {
	message->header.messageType = REQUEST_DATA;

	uint8_t payload[8];
	payload[0] = DATA_OK << 4 | (message->dataID1 >> 8 & 0x0F);
	payload[1] = message->dataID1;
	payload[2] = message->state >> 8;
	payload[3] = message->state;
	payload[4] = DATA_OK << 4 | (message->dataID2 >> 8 & 0x0F);
	payload[5] = message->dataID2;
	payload[6] = message->state >> 8;
	payload[7] = message->state;

	CANP_packMessage(package, &(message->header), payload);
}

void CANP_packTransition(CANP_Package *package, CANP_TransitionMessage *message) {
	message->header.messageType = TRANSITION;

	uint8_t payload[8];
	payload[0] = (TRANSITION & 0x0F) | (TRANSITION << 4 | 0xF0);
	payload[1] = payload[0];
	payload[2] = message->state >> 8;
	payload[3] = message->state;
	payload[4] = payload[2];
	payload[5] = payload[3];
	payload[6] = payload[2];
	payload[7] = payload[3];
}

// Interpreting -----------------------------------------------------------------------------
void CANP_unpackSingleDataFromMessage(CANP_Data *data, uint8_t *buffer, uint8_t startIndx) {
	data->dataHeader.dataStatus = buffer[startIndx] >> 4;
	data->dataHeader.dataType = (0x0F & buffer[startIndx]) << 8 | buffer[startIndx + 1];
	data->payload = buffer[startIndx + 2] << 8 | buffer[startIndx + 3];
}

STATE_ID getStateIDfromMessage(CANP_Package *package) {
	return (0xF & package->payload[6]) << 8 | package->payload[7];
}

void CANP_unpackDataMessage(CANP_Package *package, CANP_DataMessage *message) {
	CANP_unpackSingleDataFromMessage(&(message->data1), package->payload, 0);
	CANP_unpackSingleDataFromMessage(&(message->data2), package->payload, 4);
}

void CANP_unpackRequestDataMessage(CANP_Package *package, CANP_RequestDataMessage *message) {
	message->state = getStateIDfromMessage(package);
	message->dataID1 = (package->payload[0] & 0x0F) << 8 | package->payload[1];
	message->dataID2 = (package->payload[4] & 0x0F) << 8 | package->payload[5];
}

void CANP_unpackTransitionMessage(CANP_Package *package, CANP_TransitionMessage *message) {
	MESSAGE_TYPE dType1 = package->payload[0] >> 4;
	MESSAGE_TYPE dType2 = package->payload[0] & 0x0F;
	MESSAGE_TYPE dType3 = package->payload[1] >> 4;
	MESSAGE_TYPE dType4 = package->payload[1] & 0x0F;

	uint8_t firstHalfIsTransition = dType1 == TRANSITION && dType2 == TRANSITION
			&& dType3 == TRANSITION && dType4 == TRANSITION;

	STATE_ID state1 = package->payload[2] << 8 | package->payload[3];
	STATE_ID state2 = package->payload[4] << 8 | package->payload[5];
	STATE_ID state3 = package->payload[6] << 8 | package->payload[7];

	uint8_t isStatesConsistent = state1 == state2 && state2 == state3;

	message->messageValid = firstHalfIsTransition && isStatesConsistent;
	message->state = message->messageValid ? dType1 : NULL_STATE;
}

void CANP_unpackStatus(CANP_Data* data, CANP_Status* status) {
	status->dataHeader = data->dataHeader;
	status->mcuStatus = data->payload >> 8;
	status->state = 0x00FF & data->payload;
	status->mcu = status->mcuStatus >> 5;
}

void CANP_packStatus(CANP_Data* data, CANP_Status* status) {
	data->dataHeader = status->dataHeader;
	data->payload = status->mcuStatus << 8 | status->state;
}

#endif
