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
	GPIOPair GP1ON;
	GPIOPair GP1REC;
	uint16_t i2c_add;
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
#endif

#ifdef __STM32_I2C_PERIPHERAL
	I2C_HandleTypeDef *hi2c;
#endif

	GPIOPair LD1;
	GPIOPair LD2;
	GPIOPair LD3;
	GPIOPair LD4;
	VAS_GoPro GP1;
	VAS_GoPro GP2;
} VAS_PeripheralStruct;

