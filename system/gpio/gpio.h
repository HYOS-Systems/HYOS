/*
 * systemUtils.h
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#pragma once

#include "drivers/hyend_os_drivers.h"

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIOPair;
