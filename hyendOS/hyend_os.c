/*
 * hyend_os.c
 *
 *  Created on: Jun 19, 2021
 *      Author: Bayram
 */


#include "hyendOS/hyend_os.h"

uint32_t HYOS_GetTick() {
	return HAL_GetTick();
}
