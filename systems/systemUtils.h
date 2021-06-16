/*
 * systemUtils.h
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#pragma once

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIOPair;
