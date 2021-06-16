/*
 * rsms_types.h
 *
 *  Created on: Jun 16, 2021
 *      Author: Bayram
 */

#pragma once
#include "hyendOS/hyend_os.h"
#include "systems/systemUtils.h"

typedef struct {
#ifdef __STM32_SPI_PERIPHERAL
	SPI_HandleTypeDef *hspi_p;
	GPIOPair chipSelect_p;
	SPI_HandleTypeDef *hspi_t;
	GPIOPair chipSelect_t;
#endif
#ifdef __STM32_UART_PERIPHERAL
	UART_HandleTypeDef *serialDebug;
#endif
#ifdef __STM32_CAN_PERIPHERAL
	CAN_HandleTypeDef *busIFC;
#endif
	GPIOPair LD1;
	GPIOPair LD2;
	GPIOPair LD3;
	GPIOPair LD4;
	GPIOPair UTIL1;
	GPIOPair UTIL2;
	GPIOPair UTIL3;
	GPIOPair UTIL4;
} RSMS_PeripheralStruct;


