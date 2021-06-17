/*
 * rsms_board.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/rsms/rsms_board.h"

RSMS_PeripheralStruct *rsms_struct;

typedef struct {
#ifdef __STM32_CAN_PERIPHERAL
CANBus *bus;
#endif
} RSMS_BOARD_variables;

RSMS_BOARD_variables rsmsBoard;

#ifdef __STM32_SPI_PERIPHERAL
#endif

void RSMS_BOARD_initUsart() {
#ifdef __STM32_UART_PERIPHERAL
	initXprint(rsms_struct->serialDebug);
#endif
}

void RSMS_BOARD_initCAN() {
#ifdef __STM32_CAN_PERIPHERAL
	rsmsBoard.bus = CAN_init(rsms_struct->busIFC, RSMS);
#endif
}

void RSMS_BOARD_initSDLogging() {
#ifdef __STM32_SDIO_PERIPHERAL
	SDFH_init();
	Logger_init(1000);
#endif
}

void RSMS_BOARD_initSensors() {
#ifdef __STM32_SPI_PERIPHERAL
	RSMS_P_init(rsms_struct);
	RSMS_T_init(rsms_struct);
#endif
}

void RSMS_BOARD_init(RSMS_PeripheralStruct *rsms_PeripheralStruct) {
	rsms_struct = rsms_PeripheralStruct;

	RSMS_BOARD_initUsart();
	RSMS_BOARD_initCAN();
	RSMS_BOARD_initSDLogging();
	RSMS_BOARD_initSensors();

	xprintf("RSMS Initialisiert.\n");
}

void RSMS_BOARD_canHandle(void *pt) {

}
