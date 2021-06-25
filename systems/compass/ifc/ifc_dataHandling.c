/*
 * ifc_dataHandling.c
 *
 *  Created on: 21.06.2021
 *      Author: Bayram
 */

#include "systems/compass/ifc/ifc_dataHandling.h"

typedef struct {
	MCU_STATUS mcuStatus;
#ifdef __STM32_CAN_PERIPHERAL
	CANBus *gseBus;
	CANBus *sysBus;

	// Interrupt Handling
	CANBus *busIR;
#endif
	CANP_MessageHeader headerIR;
	// Sys
	CANP_Status statusSys;
	CANP_DataMessage dataMessageSys;
	CANP_TransitionMessage transitionMessageSys;
	// GSE
	CANP_Data dataGSE;
	CANP_DataMessage dataMessageGSE;
	CANP_RequestDataMessage reqDataMessageGSE;
	CANP_TransitionMessage transitionMessageGSE;
} IFC_DataHandling;

IFC_DataHandling ifcDataHandling;

void IFC_DH_initSysBus(IFC_PeripheralStruct* ifc_struct) {
#ifdef __STM32_CAN_PERIPHERAL
	ifcDataHandling.sysBus = CAN_init(ifc_struct->busSys, IFC);
#endif
}

void IFC_DH_init(IFC_PeripheralStruct *ifc_struct) {
#ifdef __STM32_CAN_PERIPHERAL
	ifcDataHandling.gseBus = CAN_init(ifc_struct->busGSE, IFC);
#endif

	ifcDataHandling.transitionMessageSys.header.targetMCU = MAX_MCU;
	ifcDataHandling.transitionMessageSys.messageValid = 1;

	ifcDataHandling.dataMessageGSE.header.targetMCU = GSE;
	ifcDataHandling.dataMessageGSE.header.messageType = DATA;
}

// State Machine ===========================================================
void IFC_DH_sendSingleTransition(STATE_ID state) {
	ifcDataHandling.transitionMessageSys.header.timeStamp = HYOS_GetTick();
	ifcDataHandling.transitionMessageSys.state = state;

#ifdef __STM32_CAN_PERIPHERAL
	CANI_sendTransition(ifcDataHandling.sysBus, &ifcDataHandling.transitionMessageSys);
#endif
}

void IFC_DH_sendTransition(STATE_ID state) {
	IFC_DH_sendSingleTransition(state);
	HAL_Delay(500);
	IFC_DH_sendSingleTransition(state);
	HAL_Delay(500);
	IFC_DH_sendSingleTransition(state);
}

// Sys Handling ============================================================
void IFC_DH_reactOnData() {
	MAP_setDataFromCANP(&ifcDataHandling.dataMessageSys.data1, ifcDataHandling.dataMessageSys.header.timeStamp);
	MAP_setDataFromCANP(&ifcDataHandling.dataMessageSys.data2, ifcDataHandling.dataMessageSys.header.timeStamp);
}

// GSE Handling ============================================================
void IFC_DH_reactOnSingleRequest(CANP_MessageHeader *header, DATA_ID dataID) {
	header->timeStamp = MAP_getDataToCANP(dataID, &ifcDataHandling.dataGSE);
}

void IFC_DH_reactOnRequest() {
	IFC_DH_reactOnSingleRequest(&ifcDataHandling.reqDataMessageGSE.header, ifcDataHandling.reqDataMessageGSE.dataID1);
	ifcDataHandling.dataMessageGSE.data1 = ifcDataHandling.dataGSE;

	IFC_DH_reactOnSingleRequest(&ifcDataHandling.reqDataMessageGSE.header, ifcDataHandling.reqDataMessageGSE.dataID2);
	ifcDataHandling.dataMessageGSE.data2 = ifcDataHandling.dataGSE;

	CANI_sendData(ifcDataHandling.gseBus, &ifcDataHandling.dataMessageGSE);
}

// CAN Handling ============================================================
void IFC_DH_canHandle(CANBus *bus) {
#ifdef __STM32_CAN_PERIPHERAL
	ifcDataHandling.busIR = bus;
	CANI_receiveMessage(ifcDataHandling.busIR, &ifcDataHandling.headerIR);

	if (!CANI_isThisTarget(&ifcDataHandling.headerIR)) {
		return;
	}

	switch (ifcDataHandling.headerIR.messageType) {
	case DATA: // From Sys
		CANI_interpretDataMessage(ifcDataHandling.busIR, &ifcDataHandling.dataMessageSys);
		IFC_DH_reactOnData();
		break;
	case REQUEST_DATA: // From GSE
		CANI_interpretRequestDataMessage(ifcDataHandling.busIR, &ifcDataHandling.reqDataMessageGSE);
		if(ifcDataHandling.reqDataMessageGSE.dataID1 == IFC_STATUS){
			uint8_t test = 1;
		}
		IFC_DH_reactOnRequest();
		break;
	case TRANSITION: // From GSE
		CANI_interpretTransitionMessage(ifcDataHandling.busIR, &ifcDataHandling.transitionMessageGSE);
		break;
	default:
		break;
	}
#endif
}
