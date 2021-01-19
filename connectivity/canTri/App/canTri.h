/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once

#include <hyendOS/OSVersioning/App/OSVersioning.h>
/* Check if CAN is implemented -----------------------------------------------*/
#ifdef __STM32_CAN_PERIPHERAL

// Define valid Initialization
#define INC_CANTRI_H_

/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include <connectivity/xprintf/App/xprintf.h>

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

void init_CAN(CAN_HandleTypeDef*, uint32_t);

void CAN1_SendMessage(char *ch);
void CAN2_SendMessage(char *ch);
void CAN3_SendMessage(char *ch);
void CAN1_ReceiveMessage(unsigned char *message);
void CAN2_ReceiveMessage(unsigned char *message);
void CAN3_ReceiveMessage(unsigned char *message);


#endif // STM32F#xx_HAL_CAN_H

/*****END OF FILE****/
