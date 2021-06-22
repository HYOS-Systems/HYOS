/*
 * gopro.h
 *
 *  Created on: Jun 18, 2021
 *      Author: Bayram
 */

#pragma once

#include "hyendOS/hyend_os.h"

#ifdef __STM32_I2C_PERIPHERAL
#define PERIPHERALS_INA233_H_
#endif /* PERIPHERALS_GOPRO_GOPRO_H_ */

typedef struct {
	uint16_t address;
#ifdef __STM32_I2C_PERIPHERAL
	I2C_HandleTypeDef *hi2c;
#endif
} INA233;

void INA_init(INA233*);
short INA_measure(INA233*);
