/*
 * rsms_board.h
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/rsms/rsms_types.h"
#include "systems/compass/rsms/rsms_p.h"
#include "systems/compass/rsms/rsms_t.h"

void RSMS_BOARD_init(RSMS_PeripheralStruct *rsms_PeripheralStruct);
void RSMS_BOARD_tick(void);

void RSMS_BOARD_canHandle(void *pt);

void RSMS_BOARD_logAndSendStatus(void);
void RSMS_BOARD_sendData(void);
