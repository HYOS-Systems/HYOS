/*
 * canTri.h
 *
 * Ensures that CANBus struct is available even with no definedn CAN Preipheral.
 */
#pragma once

#include <hyendOS/OSVersioning/App/HALVersioning.h>

#define CAN_PERIPHERAL_MISSING 1
#define CAN_PERIPHERAL_EXISTS 3

#ifdef __STM32_CAN_PERIPHERAL
#define INC_CANTRI_H_ CAN_PERIPHERAL_EXISTS
#else
#define INC_CANTRI_H_ CAN_PERIPHERAL_MISSING
#endif

#if (INC_CANTRI_H_ == CAN_PERIPHERAL_EXISTS)
#include "can.h"
#include "connectivity/xprintf/App/xprintf.h"
#endif

typedef struct {
	uint8_t number;
#if (INC_CANTRI_H_ == CAN_PERIPHERAL_EXISTS)
	CAN_TxHeaderTypeDef pTxHeader;
	CAN_RxHeaderTypeDef pRxHeader;
	uint32_t TxMailbox;
	uint8_t a;
	uint8_t receiveBuffer[8];
	uint8_t transceiveBuffer[8];
	CAN_FilterTypeDef sFilterConfig;
	CAN_HandleTypeDef* hcan;
#endif
} CANBus;

#if (INC_CANTRI_H_ == CAN_PERIPHERAL_EXISTS)
CANBus bus1;
CANBus bus2;
CANBus bus3;

void init_CAN(CAN_HandleTypeDef*, uint32_t);

void setHeader(CANBus* bus, uint32_t ExtID);

void CAN_SendMessage(uint8_t *ch, CANBus *bus);
void CAN1_SendMessage(char *ch);
void CAN2_SendMessage(char *ch);
void CAN3_SendMessage(char *ch);
void CAN_ReceiveMessage(CANBus *bus);
void CAN1_ReceiveMessage(unsigned char *message);
void CAN2_ReceiveMessage(unsigned char *message);
void CAN3_ReceiveMessage(unsigned char *message);


#endif

/*****END OF FILE****/
