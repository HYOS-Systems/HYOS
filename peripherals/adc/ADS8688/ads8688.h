/*
 * ads8688.h
 *
 *  Created on: 09.12.2020
 *      Author: Bayram
 */
#pragma once

#include "drivers/hyend_os_drivers.h"

// Required Definitions
#ifdef __STM32_SPI_PERIPHERAL

// Define valid Initialization
#define INC_ADS8688_H_

#include <stdio.h>
//#include "connectivity/sd/App/SDfileHandling.h"
#include "spi.h"

typedef enum {
	PM2V5	= 0b0000,
	PM1V25	= 0b0001,
	PM0V635	= 0b0010,
	ZT2V5	= 0b0101,
	ZT1V25	= 0b0110
} ADS_INPUT_RANGE;

typedef struct {
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* cs_port;
	uint16_t cs_pin;
	uint8_t active_pins;
	ADS_INPUT_RANGE input_range;
} ADS_8688;

void ADS_init(ADS_8688*);
uint16_t ADS_measure(ADS_8688*);

#endif /* STM32F#xx_HAL_SPI_H */
