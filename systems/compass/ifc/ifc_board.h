/*
 * ifc_board.h
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/ifc/ifc_types.h"

void IFC_blinkLED(GPIOPair ld);
void IFC_flickLED(GPIOPair ld1, GPIOPair ld2, GPIOPair ld3);
void IFC_BOARD_init(IFC_PeripheralStruct *ifc_PeripheralStruct);

void IFC_BOARD_canHandle(void *pt);
