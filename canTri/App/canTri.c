/**
 ******************************************************************************
 * File Name          : CAN.c
 * Description        : This file provides code for the configuration
 *                      of the CAN instances.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "canTri/App/canTri.h"

/* Check if CAN is implemented -----------------------------------------------*/
#ifdef STM32F4xx_HAL_CAN_H
typedef struct {
	uint8_t number;
	CAN_TxHeaderTypeDef pTxHeader;
	CAN_RxHeaderTypeDef pRxHeader;
	uint32_t TxMailbox;
	uint8_t a, r[8];
	CAN_FilterTypeDef sFilterConfig;
	CAN_HandleTypeDef* hcan;
} CANBus;

CANBus bus1;
CANBus bus2;
CANBus bus3;

/* Init ------------------------------------------------------------------*/
void CAN_Start(CANBus* bus) {
	uint8_t state;
	state = HAL_CAN_ActivateNotification(bus->hcan,
	CAN_IT_RX_FIFO0_MSG_PENDING);
	xprintf("CAN%d: NotificationState: %d\n", bus->number, state);
	state = HAL_CAN_Start(bus->hcan);
	xprintf("CAN%d: StartState: %d\n", bus->number, state);

	xprintf("CAN%d init completed.\n", bus->number);
}

void setStandardFilter(CANBus* bus) {
	bus->sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	bus->sFilterConfig.FilterIdHigh = 0x350 << 5;
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

void setStandardHeader(CANBus* bus, uint32_t StdID) {
	bus->pTxHeader.DLC = 8;
	bus->pTxHeader.IDE = CAN_ID_STD;
	bus->pTxHeader.RTR = CAN_RTR_DATA;
	bus->pTxHeader.StdId = StdID;

	bus->TxMailbox = CAN_TX_MAILBOX0;
}

void init_CAN(CAN_HandleTypeDef* canHandle, uint32_t StdID) {
	if (canHandle->Instance == 0) {
		xprintf("ERROR: Invalid CAN-Handle.\n");
	} else if (canHandle->Instance == CAN1) {
		bus1.hcan = canHandle;
		bus1.number = 1;

		setStandardHeader(&bus1, StdID);
		setStandardFilter(&bus1);
		CAN_Start(&bus1);

		HAL_CAN_MspInit(bus1.hcan);
	} else if (canHandle->Instance == CAN2) {
		bus2.hcan = canHandle;
		bus2.number = 2;

		setStandardHeader(&bus2, StdID);
		setStandardFilter(&bus2);
		CAN_Start(&bus2);

		HAL_CAN_MspInit(bus2.hcan);
	} else if (canHandle->Instance == CAN3) {
		bus3.hcan = canHandle;
		bus3.number = 3;

		setStandardHeader(&bus3, StdID);
		setStandardFilter(&bus3);
		CAN_Start(&bus3);

		HAL_CAN_MspInit(bus3.hcan);
	}
}

/* Settings ------------------------------------------------------------------*/
void CAN_setID(CANBus* bus, uint32_t ID) {
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
void CAN_SendMessage(uint8_t* ch, CANBus* bus) {
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
void CAN_ReceiveMessage(unsigned char *message, CANBus* bus) {
	int i;
	if (bus->number > 0) {
		HAL_CAN_GetRxMessage(bus->hcan, CAN_RX_FIFO0, &(bus->pRxHeader),
				bus->r);
	} else {
		xprintf("CAN is not initialized.\n");
		bus->r[0] = 'C';
		bus->r[1] = 'A';
		bus->r[2] = 'N';
		bus->r[3] = 48 + bus->number;
		bus->r[4] = 'N';
		bus->r[5] = 'U';
		bus->r[6] = 'L';
		bus->r[7] = 'L';
	}
	for (i = 0; i < 8; i++)
		message[i] = bus->r[i];
	message[i] = '\0'; // Adding "EndOfString"
}

void CAN1_ReceiveMessage(unsigned char *message) {
	CAN_ReceiveMessage(message, &bus1);
}

void CAN2_ReceiveMessage(unsigned char *message) {
	CAN_ReceiveMessage(message, &bus2);
}

void CAN3_ReceiveMessage(unsigned char *message) {
	CAN_ReceiveMessage(message, &bus3);
}

#endif // STM32F4xx_HAL_CAN_H
/****END OF FILE****/
