/*
 * ccdh.h
 *
 *  Created on: Jan 12, 2021
 *      Author: Bayram
 */
#pragma once

#include "drivers/hyend_os_drivers.h"

#include "core/hyend_os_core.h"
#include "connectivity/hyend_os_connectivity.h"
#include "peripherals/hyend_os_peripherals.h"
#include "system/hyend_os_system.h"

#include "hyendOS/canProtocolImplementation/canProtocolImpl.h"


uint32_t HYOS_GetTick(void);
void HYOS_start(void);
