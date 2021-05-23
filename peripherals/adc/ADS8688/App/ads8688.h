/*
 * ads8688.h
 *
 *  Created on: 09.12.2020
 *      Author: Bayram
 */
#pragma once

#include <hyendOS/OSVersioning/App/HALVersioning.h>

// Required Definitions
#ifdef __STM32_SPI_PERIPHERAL

// Define valid Initialization
#define INC_ADS8688_H_

#include <stdio.h>
#include "connectivity/sd/App/SDfileHandling.h"
#include "spi.h"

typedef struct {
	GPIO_TypeDef* cs_port;
	uint16_t cs_pin;
	SPI_HandleTypeDef* hspi;
} ADS_8688;

void initADC(ADS_8688*);
void measure(ADS_8688*);

#endif /* STM32F#xx_HAL_SPI_H */
