/*
 * ifc_board.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/ifc/ifc_board.h"

union {
#ifdef __STM32_CAN_PERIPHERAL
	CANBus *gseBus;
	CANBus *sysBus;

	CANBus *bus;
	CANP_Status status;
	CANP_MessageHeader header;
	CANP_DataMessage dataMessage;
	CANP_RequestDataMessage reqDataMessage;
	CANP_TransitionMessage transitionMessage;
#endif
} ifcBoard;

//IFC_BOARD_variables ifcBoard;

#ifdef __STM32_SPI_PERIPHERAL
#endif

void IFC_BOARD_initUsart(IFC_PeripheralStruct *ifc_struct) {
#ifdef __STM32_UART_PERIPHERAL
	initXprint(ifc_struct->serialDebug);
#endif
}

void IFC_BOARD_initCAN(IFC_PeripheralStruct *ifc_struct) {
#ifdef __STM32_CAN_PERIPHERAL
	ifcBoard.gseBus = CAN_init(ifc_struct->busGSE, IFC);
	ifcBoard.sysBus = CAN_init(ifc_struct->busSys, IFC);
#endif
}

void IFC_BOARD_initSDLogging() {
#ifdef __STM32_SDIO_PERIPHERAL
	SDFH_init();
	Logger_init(1000);
#endif
}

void IFC_BOARD_init(IFC_PeripheralStruct *ifc_PeripheralStruct) {
	IFC_BOARD_initUsart(ifc_PeripheralStruct);
	IFC_BOARD_initCAN(ifc_PeripheralStruct);
	IFC_BOARD_initSDLogging();

	xprintf("IFC Initialisiert.\n");
}


void IFC_BOARD_reactOnSingleData(CANP_MessageHeader* header, CANP_Data* data) {
	if (CANI_isStatusData(data)){
		CANI_unpackStatus(data, &ifcBoard.status);

	} else {

	}
}

void IFC_BOARD_reactOnData() {
	IFC_BOARD_reactOnSingleData(&ifcBoard.dataMessage.header, &ifcBoard.dataMessage.data1);
	IFC_BOARD_reactOnSingleData(&ifcBoard.dataMessage.header, &ifcBoard.dataMessage.data2);
}

void IFC_BOARD_reactOnRequest() {

}

void IFC_BOARD_canHandle(void *pt) {
	ifcBoard.bus = (CANBus*) pt;
	CANI_receiveMessage(ifcBoard.bus, &ifcBoard.header);

	if (!CANI_isThisTarget(&ifcBoard.header)) {
		return;
	}

	switch (ifcBoard.header.messageType) {
	case DATA:
		CANI_interpretDataMessage(ifcBoard.bus, &ifcBoard.dataMessage);
		IFC_BOARD_reactOnData();
		break;
	case REQUEST_DATA:
		CANI_interpretRequestDataMessage(ifcBoard.bus, &ifcBoard.reqDataMessage);
		IFC_BOARD_reactOnRequest();
		break;
	case TRANSITION:
		CANI_interpretTransitionMessage(ifcBoard.bus, &ifcBoard.transitionMessage);
		break;
	default:
		break;
	}
}
