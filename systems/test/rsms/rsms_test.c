/*
 * rsms.c
 *
 *  Created on: 17.01.2021
 *      Author: Bayram
 */

#include <systems/test/rsms/rsms_test.h>

#ifdef HYENDOS_SYSTEMS_RSMS_TEST_H_

ADS_8688 adc1;
ADS_8688 adc2;
//Tasks idle_tasks;

void rsms_test_init() {
	initXprint(&huart2);
	init_CAN(&hcan3, 0x111);

	adc1.hspi = &hspi1;
	adc1.cs_pin = SPI1_CS_Pin;
	adc1.cs_port = SPI1_CS_GPIO_Port;
	adc1.active_pins = 0b00000011;
	adc1.input_range = ZT1V25;

	adc2.hspi = &hspi2;
	adc2.cs_pin = SPI2_CS_Pin;
	adc2.cs_port = SPI2_CS_GPIO_Port;
	adc2.active_pins = 0b11000011;
	adc2.input_range = ZT1V25;

	initADC(&adc1);
	initADC(&adc2);
	xprintf("RSMS Initialisiert.\n");
}

void rsms_responseWithData(CANBus *bus) {
	DataMessage message;

//	message.header.sourceMCU = microcontroller->number;
	message.header.targetMCU = IFC;
//	message.header.messageType = DATA;
	message.header.timestamp = (uint16_t) HAL_GetTick();

	message.data1.status = DATA_OK;
	message.data1.dataType = IMU_VZ;
	message.data1.payload = 0xFFFF;

	message.data2.status = DATA_OLD;
	message.data2.dataType = IMU_VY;
	message.data2.payload = 0xFFFF;

	sendData(bus, &message);
}

void rsms_test_responseWithState(CANBus *bus) {
	StateMessage message;

	message.header.targetMCU = IFC;
	message.header.timestamp = (uint16_t) HAL_GetTick();

	message.state = microcontroller->state->stateID;

	sendState(bus, &message);
}

void rsms_test_canHandle(CANBus *bus) {
	/*
	 * CAN_Handler Struct necessary?
	 */

	MessageHeader mHeader;
	MESSAGE_TYPE mType = receiveMessage(bus, &mHeader);

	if (!isThisTarget(&mHeader)) {
		return;
	}

	DataMessage dataMessage;
	StateMessage stateMessage;
	RequestDataMessage reqDataMessage;
	StateMessage reqStateMessage;

	switch (mType) {
	case DATA:
		interpretDataMessage(bus, &dataMessage);
		/* Do stuff with dMessage */
		break;
	case STATUS:
		interpretStateMessage(bus, &stateMessage);
		/* Do stuff with sMessage */
		break;
	case REQUEST_DATA:
		interpretRequestDataMessage(bus, &reqDataMessage);
		//responseWithData(bus);
		break;
	case REQUEST_STATUS:
		interpretRequestStateMessage(bus, &reqStateMessage);
		//responseWithState(bus);
		break;
	case TRANSITION:
		interpretStateTransitionMessage(bus, &mHeader);
		break;
	default:
		break;
	}
}

#endif
