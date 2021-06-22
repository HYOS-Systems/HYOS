/*
 * ifc_types.h
 *
 *  Created on: Jun 16, 2021
 *      Author: Bayram
 */

#pragma once
#include "hyendOS/hyend_os.h"
#include "systems/systemUtils.h"

typedef struct {
	GPIOPair on;
	GPIOPair rec;
	GPIOPair alert;
	INA233 ina;
} VAS_GoPro;

typedef struct {
#ifdef __STM32_UART_PERIPHERAL
	UART_HandleTypeDef *serialDebug;
#endif
#ifdef __STM32_CAN_PERIPHERAL
	CAN_HandleTypeDef *bus;
#endif
#ifdef __STM32_TIM_PERIPHERAL
	TIM_HandleTypeDef *htim;
	Timer statusTim;
	Timer gpCheckTim;
#endif

	GPIOPair LD1;
	GPIOPair LD2;
	GPIOPair LD3;
	GPIOPair LD4;
	VAS_GoPro GP1;
	VAS_GoPro GP2;
} VAS_PeripheralStruct;

