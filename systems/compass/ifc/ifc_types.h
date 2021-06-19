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
	// TODO; Remove LDs
	GPIOPair LD1;
	GPIOPair LD2;
	GPIOPair LD3;
	GPIOPair LD4;

	uint8_t flag1;
	uint32_t count1;
	uint32_t max1;
	uint8_t flag2;
	uint32_t count2;
	uint32_t max2;
} IFC_PeripheralStruct;


