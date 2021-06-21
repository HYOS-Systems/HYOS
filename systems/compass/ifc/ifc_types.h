/*
 * ifc_types.h
 *
 *  Created on: Jun 16, 2021
 *      Author: Bayram
 */

#pragma once
#include "hyendOS/hyend_os.h"
#include "systems/systemUtils.h"
#include "systems/compass/ifc/dataMap.h"

typedef struct {
#ifdef __STM32_UART_PERIPHERAL
	UART_HandleTypeDef *serialDebug;
#endif
#ifdef __STM32_CAN_PERIPHERAL
	CAN_HandleTypeDef *busGSE;
	CAN_HandleTypeDef *busSys;
#endif
#ifdef __STM32_TIM_PERIPHERAL
	TIM_HandleTypeDef *htim;
	Timer statusTim;
	Timer second;
#endif

	GPIOPair LD1;
} IFC_PeripheralStruct;


