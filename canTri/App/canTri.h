/**
 ******************************************************************************
 * File Name          : CAN.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
/* Check if CAN is implemented -----------------------------------------------*/
#ifdef STM32F4xx_HAL_CAN_H

/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "xprintf/App/xprintf.h"

void init_CAN(CAN_HandleTypeDef*, uint32_t);

void CAN1_SendMessage(char *ch);
void CAN2_SendMessage(char *ch);
void CAN3_SendMessage(char *ch);
void CAN1_ReceiveMessage(unsigned char *message);
void CAN2_ReceiveMessage(unsigned char *message);
void CAN3_ReceiveMessage(unsigned char *message);

#endif // STM32F4xx_HAL_CAN_H

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
