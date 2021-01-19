/*
 * ads8688.h
 *
 *  Created on: 09.12.2020
 *      Author: Bayram
 */
#pragma once

#include <hyendOS/OSVersioning/App/OSVersioning.h>

// Required Definitions
#if defined(STM32F1xx_HAL_SPI_H) || defined(STM32F4xx_HAL_SPI_H)

// Define valid Initialization
#ifndef INC_ADS8688_H_
#define INC_ADS8688_H_
#endif /* INC_ADS8688_H_ */

#include "spi.h"
#include "sd/App/SDfileHandling.h"
#include <stdio.h>

void initADC(SPI_HandleTypeDef*);
void measure(SPI_HandleTypeDef*);

#endif /* STM32F#xx_HAL_SPI_H */
