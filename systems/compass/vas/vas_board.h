/*
 * vas_board.h
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/vas/vas_types.h"

void VAS_BOARD_init(VAS_PeripheralStruct *vas_PeripheralStruct);
void VAS_BOARD_tick(void);

void VAS_BOARD_canHandle(void *pt);

void VAS_BOARD_sendStatus(void);
void VAS_BOARD_sendData(void);

uint8_t VAS_isGPOn();
uint8_t VAS_isGPRecording();
void VAS_startGP();
void VAS_startGPRecording();
void VAS_shutdownGP();
