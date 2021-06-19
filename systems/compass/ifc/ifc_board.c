/*
 * ifc_board.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Bayram
 */

#include "systems/compass/ifc/ifc_board.h"

typedef struct {
#ifdef __STM32_CAN_PERIPHERAL
//#if 1
	CANBus *gseBus;
	CANBus *sysBus;

	CANBus *bus;
	CANP_Status status;
	CANP_MessageHeader header;
	CANP_Data data;
	CANP_DataMessage dataMessage;
	CANP_RequestDataMessage reqDataMessage;
	CANP_TransitionMessage transitionMessage;
#endif
} IFC_BOARD_Peripherals;
IFC_BOARD_Peripherals ifcBoard;

#ifdef __STM32_SPI_PERIPHERAL
#endif

// Init =================================================================
void IFC_BOARD_initUsart(IFC_PeripheralStruct *ifc_struct) {
#ifdef __STM32_UART_PERIPHERAL
	initXprint(ifc_struct->serialDebug);
#endif
}

void IFC_BOARD_initCAN(IFC_PeripheralStruct *ifc_struct) {
#ifdef __STM32_CAN_PERIPHERAL
	ifcBoard.gseBus = CAN_init(ifc_struct->busGSE, IFC);
//	ifcBoard.sysBus = CAN_init(ifc_struct->busSys, IFC);
#endif
}

void IFC_BOARD_initSDLogging() {
#ifdef __STM32_SDIO_PERIPHERAL
	SDFH_init();
	Logger_init(10);
#endif
}

void IFC_BOARD_init(IFC_PeripheralStruct *ifc_PeripheralStruct) {
	MAP_init();
//	IFC_BOARD_initUsart(ifc_PeripheralStruct);
	IFC_BOARD_initCAN(ifc_PeripheralStruct);
	IFC_BOARD_initSDLogging();

	xprintf("IFC Initialisiert.\n");
}

// Test =================================================================
void IFC_blinkLED(GPIOPair ld) {
	HAL_GPIO_WritePin(ld.port, ld.pin, GPIO_PIN_SET);
	for (uint8_t i = 0; i < 3 * 2 - 1; i++) {
		HAL_Delay(500);
		HAL_GPIO_TogglePin(ld.port, ld.pin);
	}
}

void IFC_flickLED(GPIOPair ld1, GPIOPair ld2, GPIOPair ld3) {
	HAL_GPIO_WritePin(ld1.port, ld1.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ld2.port, ld2.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ld3.port, ld3.pin, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(ld1.port, ld1.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ld2.port, ld2.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ld3.port, ld3.pin, GPIO_PIN_RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(ld1.port, ld1.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ld2.port, ld2.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ld3.port, ld3.pin, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(ld1.port, ld1.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ld2.port, ld2.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ld3.port, ld3.pin, GPIO_PIN_RESET);
}

// DataHandling =================================================================
void IFC_BOARD_reactOnSingleData(CANP_MessageHeader *header, CANP_Data *data) {
	if (CANI_isStatusData(data)) {
		CANI_unpackStatus(data, &ifcBoard.status);

	} else {

	}
}

void IFC_BOARD_reactOnData() {
	IFC_BOARD_reactOnSingleData(&ifcBoard.dataMessage.header, &ifcBoard.dataMessage.data1);
	IFC_BOARD_reactOnSingleData(&ifcBoard.dataMessage.header, &ifcBoard.dataMessage.data2);
}

void IFC_BOARD_reactOnSingleRequest(CANP_MessageHeader *header, DATA_ID dataID) {
	header->timeStamp = MAP_getDataToCANP(dataID, &ifcBoard.data);
}

void IFC_BOARD_reactOnRequest() {
	IFC_BOARD_reactOnSingleRequest(&ifcBoard.reqDataMessage.header, ifcBoard.reqDataMessage.dataID1);
	ifcBoard.dataMessage.data1 = ifcBoard.data;
	IFC_BOARD_reactOnSingleRequest(&ifcBoard.reqDataMessage.header, ifcBoard.reqDataMessage.dataID2);
	ifcBoard.dataMessage.data2 = ifcBoard.data;

	ifcBoard.dataMessage.header.targetMCU = GSE;
	ifcBoard.dataMessage.header.messageType = DATA;
	ifcBoard.dataMessage.header.timeStamp = HAL_GetTick() * 100;

	CANI_sendData(ifcBoard.gseBus, &ifcBoard.dataMessage);
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
