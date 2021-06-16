/*
 * rsms_t.h
 *
 *  Created on: Jun 16, 2021
 *      Author: Bayram
 */

#pragma once
#include "systems/compass/rsms/rsms_types.h"

#ifdef __STM32_SPI_PERIPHERAL
#define _H_RSMS_T
#endif

void RSMS_T_init(RSMS_PeripheralStruct*);
void RSMS_T_measureTemperature(void);
void RSMS_T_logData(void);


