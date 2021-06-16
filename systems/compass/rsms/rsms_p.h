/*
 * rsms_t.h
 *
 *  Created on: Jun 16, 2021
 *      Author: Bayram
 */

#pragma once
#include "systems/compass/rsms/rsms_types.h"

#ifdef __STM32_SPI_PERIPHERAL
#define _H_RSMS_P
#endif

void RSMS_P_init(RSMS_PeripheralStruct*);
void RSMS_P_measureTemperature();
