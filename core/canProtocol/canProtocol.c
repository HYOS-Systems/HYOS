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

static const int mask_20bit = (int) 0x000FFFFF;
static const int mask_09bit = (int) 0x000001FF;
static const int mask_08bit = (int) 0x000000FF;
static const int mask_04bit = (int) 0x0000000F;
static const int mask_03bit = (int) 0x00000007;

// Sending -----------------------------------------------------------------------------
uint32_t CANP_packHeader(CANP_MessageHeader *mHeader) {
	uint32_t header = 0;
	header = header | (mask_03bit & mHeader->sourceMCU) << sourceOffset;
	header = header | (mask_03bit & mHeader->targetMCU) << targetOffset;
	header = header | (mask_03bit & mHeader->messageType) << meTypeOffset;
	header = header | (mask_20bit & mHeader->timeStamp);
	return header;
}

void buildHalfPayload(uint8_t *payload, CANP_Data *data, uint8_t startIndx) {
	payload[startIndx + 0] = data->dataHeader.dataStatus << 4 | (mask_04bit & data->dataHeader.dataType >> 8);
	payload[startIndx + 1] = data->dataHeader.dataType;
	payload[startIndx + 2] = data->payload >> 8;
	payload[startIndx + 3] = data->payload;
}

void CANP_packData(CANP_Package *package, CANP_DataMessage *message) {
	message->header.messageType = DATA;
	package->extID = CANP_packHeader(&message->header);

	buildHalfPayload(package->payload, &message->data1, 0);
	buildHalfPayload(package->payload, &message->data2, 4);
}

void CANP_packRequestData(CANP_Package *package, CANP_RequestDataMessage *message) {
	message->header.messageType = REQUEST_DATA;
	package->extID = CANP_packHeader(&message->header);

	package->payload[0] = DATA_OK << 4 | (message->dataID1 >> 8 & mask_04bit);
	package->payload[1] = message->dataID1;
	package->payload[2] = DATA_OK << 4 | (message->dataID2 >> 8 & mask_04bit);
	package->payload[3] = message->dataID2;
	package->payload[4] = message->state;
}

void CANP_packTransition(CANP_Package *package, CANP_TransitionMessage *message) {
	message->header.messageType = TRANSITION;
	package->extID = CANP_packHeader(&message->header);

	package->payload[0] = (TRANSITION & mask_04bit) | ((TRANSITION & mask_04bit) << 4);
	package->payload[1] = package->payload[0];
	package->payload[2] = message->state;
	package->payload[3] = message->state;
	package->payload[4] = message->state;
}

// Interpreting -----------------------------------------------------------------------------
void CANP_unpackHeader(CANP_Package *package, CANP_MessageHeader *mHeader) {
	uint32_t header = package->extID;
	mHeader->sourceMCU = mask_03bit & header >> sourceOffset;
	mHeader->targetMCU = mask_03bit & header >> targetOffset;
	mHeader->messageType = mask_03bit & header >> meTypeOffset;
	mHeader->timeStamp = mask_20bit & header;
}

void CANP_unpackSingleDataFromMessage(CANP_Data *data, uint8_t *buffer, uint8_t startIndx) {
	data->dataHeader.dataStatus = buffer[startIndx] >> 4 & mask_04bit;
	data->dataHeader.dataType = (mask_04bit & buffer[startIndx]) << 8 | buffer[startIndx + 1];
	data->payload = buffer[startIndx + 2] << 8 | buffer[startIndx + 3];
}

void CANP_unpackDataMessage(CANP_Package *package, CANP_DataMessage *message) {
	CANP_unpackHeader(package, (CANP_MessageHeader*) message);
	CANP_unpackSingleDataFromMessage(&(message->data1), package->payload, 0);
	CANP_unpackSingleDataFromMessage(&(message->data2), package->payload, 4);
}

void CANP_unpackRequestDataMessage(CANP_Package *package, CANP_RequestDataMessage *message) {
	CANP_unpackHeader(package, (CANP_MessageHeader*) message);
	message->state = package->payload[4];
	message->dataID1 = (package->payload[0] & mask_04bit) << 8 | package->payload[1];
	message->dataID2 = (package->payload[2] & mask_04bit) << 8 | package->payload[3];
}

void CANP_unpackTransitionMessage(CANP_Package *package, CANP_TransitionMessage *message) {
	CANP_unpackHeader(package, (CANP_MessageHeader*) message);
	MESSAGE_TYPE dType1 = package->payload[0] >> 4;
	MESSAGE_TYPE dType2 = package->payload[0] & mask_04bit;
	MESSAGE_TYPE dType3 = package->payload[1] >> 4;
	MESSAGE_TYPE dType4 = package->payload[1] & mask_04bit;

	uint8_t firstHalfIsTransition = 1;
	firstHalfIsTransition = firstHalfIsTransition && dType1 == TRANSITION;
	firstHalfIsTransition = firstHalfIsTransition && dType2 == TRANSITION;
	firstHalfIsTransition = firstHalfIsTransition && dType3 == TRANSITION;
	firstHalfIsTransition = firstHalfIsTransition && dType4 == TRANSITION;

	STATE_ID state1 = package->payload[2];
	STATE_ID state2 = package->payload[3];
	STATE_ID state3 = package->payload[4];

	uint8_t isStatesConsistent = state1 == state2 && state2 == state3;

	message->messageValid = firstHalfIsTransition && isStatesConsistent;
	message->state = message->messageValid ? dType1 : NULL_STATE;
}

void CANP_unpackStatus(CANP_Data *data, CANP_Status *status) {
	status->dataHeader = data->dataHeader;
	status->mcuStatus = data->payload >> 8;
	status->state = mask_08bit & data->payload;
	status->mcu = status->mcuStatus >> 5;
}

void CANP_packStatus(CANP_Data *data, CANP_Status *status) {
	data->dataHeader = status->dataHeader;
	data->payload = status->mcuStatus << 8 | status->state;
}

uint8_t CANP_isStatusData(CANP_Data *data) {
	return (data->dataHeader.dataType & mask_09bit) & 0x0001;
}

#endif
