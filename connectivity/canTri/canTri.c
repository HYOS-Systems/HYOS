/*
 * ccdh.c
 *
 *  Created on: Sep 12, 2021
 *      Author: Bayram
 */

/* Includes ------------------------------------------------------------------*/
#include "connectivity/canTri/canTri.h"

#ifdef INC_CANTRI_H_
/* Init ------------------------------------------------------------------*/
void CAN_Start(CANBus *bus) {
	uint8_t state;
	state = HAL_CAN_ActivateNotification(bus->hcan,
	CAN_IT_RX_FIFO0_MSG_PENDING);
	xprintf("CAN%d: NotificationState: %d\n", bus->number, state);
	state = HAL_CAN_Start(bus->hcan);
	xprintf("CAN%d: StartState: %d\n", bus->number, state);

	xprintf("CAN%d init completed.\n", bus->number);
}

void setStandardFilter(CANBus *bus) {
	bus->sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	bus->sFilterConfig.FilterIdHigh = 0x0000;
	bus->sFilterConfig.FilterIdLow = 0;
	bus->sFilterConfig.FilterMaskIdHigh = 0;
	bus->sFilterConfig.FilterMaskIdLow = 0;
	bus->sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	bus->sFilterConfig.FilterActivation = ENABLE;
//	bus->sFilterConfig.FilterBank = 1;
	bus->sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
//	bus->sFilterConfig.SlaveStartFilterBank = 27;

	HAL_StatusTypeDef state = HAL_CAN_ConfigFilter(bus->hcan,
			&(bus->sFilterConfig));
	xprintf("CAN%d: FilterInitState: %d\n", bus->number, state);
}

void CAN_setHeader(CANBus *bus, uint32_t ExtID) {
	bus->pTxHeader.DLC = 8;
	bus->pTxHeader.IDE = CAN_ID_EXT;
	bus->pTxHeader.RTR = CAN_RTR_DATA;
	bus->pTxHeader.ExtId = ExtID;

	bus->TxMailbox = CAN_TX_MAILBOX0;
}

CANBus* CAN_init(CAN_HandleTypeDef *canHandle, uint32_t ExtID) {
	if (canHandle->Instance == 0) {
		xprintf("ERROR: Invalid CAN-Handle.\n");
		return 0;
	}
#ifdef CAN1
	else if (canHandle->Instance == CAN1) {
		bus1.hcan = canHandle;
		bus1.number = 1;

		CAN_setHeader(&bus1, ExtID);
		setStandardFilter(&bus1);
		CAN_Start(&bus1);

//		HAL_CAN_MspInit(bus1.hcan);
		return &bus1;
	}
#endif
#ifdef CAN2
	else if (canHandle->Instance == CAN2) {
		bus2.hcan = canHandle;
		bus2.number = 2;

		CAN_setHeader(&bus2, ExtID);
		setStandardFilter(&bus2);
		CAN_Start(&bus2);

//		HAL_CAN_MspInit(bus2.hcan);
		return &bus2;
	}
#endif
#ifdef CAN3
	else if (canHandle->Instance == CAN3) {
		bus3.hcan = canHandle;
		bus3.number = 3;

		CAN_setHeader(&bus3, ExtID);
		setStandardFilter(&bus3);
		CAN_Start(&bus3);

//		HAL_CAN_MspInit(bus3.hcan);
		return &bus3;
	}
#endif
	else {
		return 0;
	}
}

/* Settings ------------------------------------------------------------------*/
void CAN_setID(CANBus *bus, uint32_t ID) {
	bus->pTxHeader.StdId = ID;
}

void CAN1_setID(uint32_t ID) {
	CAN_setID(&bus1, ID);
}

void CAN2_setID(uint32_t ID) {
	CAN_setID(&bus2, ID);
}

void CAN3_setID(uint32_t ID) {
	CAN_setID(&bus3, ID);
}

/* Send ------------------------------------------------------------------*/
void CAN_SendMessage(uint8_t *ch, CANBus *bus) {
	if (bus->number != 0) {
		xprintf("CAN%d: Nachricht wird gesendet: %s\n", bus->number, ch);
		HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(bus->hcan,
				&(bus->pTxHeader), ch, &(bus->TxMailbox));
		xprintf("Nachricht wurde gesendet mit Status: %d\n\n", status);
	} else {
		xprintf("Bus nicht initialisiert.\n");
	}
}

void CAN1_SendMessage(char *ch) {
	CAN_SendMessage((uint8_t*) ch, &bus1);
}

void CAN2_SendMessage(char *ch) {
	CAN_SendMessage((uint8_t*) ch, &bus2);
}

void CAN3_SendMessage(char *ch) {
	CAN_SendMessage((uint8_t*) ch, &bus3);
}

/* Receive ------------------------------------------------------------------*/
void CAN_ReceiveMessage(CANBus *bus) {
	if (bus->number > 0) {
		HAL_CAN_GetRxMessage(bus->hcan, CAN_RX_FIFO0, &(bus->pRxHeader),
				bus->receiveBuffer);
	} else {
		xprintf("CAN is not initialized.\n");
		bus->receiveBuffer[0] = 'C';
		bus->receiveBuffer[1] = 'A';
		bus->receiveBuffer[2] = 'N';
		bus->receiveBuffer[3] = 48 + bus->number;
		bus->receiveBuffer[4] = 'N';
		bus->receiveBuffer[5] = 'U';
		bus->receiveBuffer[6] = 'L';
		bus->receiveBuffer[7] = 'L';
	}
}

void CAN_ReceiveForwardMessage(unsigned char *message, CANBus *bus) {
	int i;
	CAN_ReceiveMessage(bus);
	for (i = 0; i < 8; i++)
		message[i] = bus->receiveBuffer[i];
	message[i] = '\0'; // Adding "EndOfString"
}

void CAN1_ReceiveMessage(unsigned char *message) {
	CAN_ReceiveForwardMessage(message, &bus1);
}

void CAN2_ReceiveMessage(unsigned char *message) {
	CAN_ReceiveForwardMessage(message, &bus2);
}

void CAN3_ReceiveMessage(unsigned char *message) {
	CAN_ReceiveForwardMessage(message, &bus3);
}

#endif
/****END OF FILE****/
