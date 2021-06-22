/*
 * rsms_t.c
 *
 *  Created on: Jun 16, 2021
 *      Author: Bayram
 */

#include "systems/compass/rsms/rsms_p.h"

#ifdef _H_RSMS_P

#define RSMS_P_max_smoothing 1
#define RSMS_P_n_sensors 4

RSMS_PeripheralStruct *rsms_struct;

typedef struct {
	uint8_t i;
	uint16_t j;
	uint16_t counter;
	uint16_t buffer[RSMS_P_max_smoothing][RSMS_P_n_sensors];
	char message[RSMS_P_n_sensors][4];
	uint16_t currentData[RSMS_P_n_sensors];
	uint32_t currentTime[RSMS_P_n_sensors];
	long double sum;
	double polyCoef[RSMS_P_n_sensors][2];

	uint16_t currentPres[RSMS_P_n_sensors];
	CANBus *bus;
	CANP_DataMessage dataMessage;
	ADS_8688 adc;
} RSMS_P_variables;

RSMS_P_variables rsmsP;

void RSMS_P_initMessages() {
	for (rsmsP.i = 0; rsmsP.i < RSMS_P_n_sensors; rsmsP.i++) {
		snprintf(rsmsP.message[rsmsP.i], 4, "P_%u", rsmsP.i);
	}
}

void RSMS_P_initADC() {
	rsmsP.adc.hspi = rsms_struct->hspi_p;
	rsmsP.adc.cs_pin = rsms_struct->chipSelect_p.pin;
	rsmsP.adc.cs_port = rsms_struct->chipSelect_p.port;
	rsmsP.adc.active_pins = 0b11000011;
	rsmsP.adc.input_range = ZT2V5;

	ADS_init(&rsmsP.adc);
}

void RSMS_P_initStruct() {
	rsmsP.i = 0;
	rsmsP.j = 0;
	rsmsP.counter = 0;

	// Arch_Dept
	rsmsP.polyCoef[0][0] = 3.41829E-2;
	rsmsP.polyCoef[0][1] = 7.70418E-4;

	rsmsP.polyCoef[1][0] = 3.09771E-2 + 1;
	rsmsP.polyCoef[1][1] = 1.55146E-3;

	rsmsP.polyCoef[2][0] = 0;
	rsmsP.polyCoef[2][1] = 1;

	rsmsP.polyCoef[3][0] = 0;
	rsmsP.polyCoef[3][1] = 1;
}

void RSMS_P_initCAN(CANBus *bus) {
	rsmsP.bus = bus;
	rsmsP.dataMessage.header.targetMCU = IFC;
	rsmsP.dataMessage.header.messageType = DATA;

	rsmsP.dataMessage.data1.header.status = DATA_OK;
	rsmsP.dataMessage.data2.header.status = DATA_OK;
}

void RSMS_P_init(RSMS_PeripheralStruct *rsms_peripheralStruct, CANBus *bus) {
	rsms_struct = rsms_peripheralStruct;

	RSMS_P_initStruct();
	RSMS_P_initADC();
	RSMS_P_initMessages();
	RSMS_P_initCAN(bus);
}

void RSMS_P_measureData() {
	// Measure
	for (rsmsP.i = 0; rsmsP.i < RSMS_P_n_sensors; rsmsP.i++) {
		ADS_measure(&rsmsP.adc);
		rsmsP.buffer[rsmsP.counter][rsmsP.i] = rsmsP.adc.dataBuffer;
		rsmsP.currentTime[rsmsP.i] = HYOS_GetTick();
	}

	// Count Data for Smoothing
	rsmsP.counter++;
	if (rsmsP.counter >= RSMS_P_max_smoothing) {
		rsmsP.counter = 0;
	}

	// Smooth
	for (rsmsP.i = 0; rsmsP.i < RSMS_P_n_sensors; rsmsP.i++) {
		rsmsP.sum = 0;
		for (rsmsP.j = 0; rsmsP.j < RSMS_P_max_smoothing; rsmsP.j++) {
			rsmsP.sum += rsmsP.buffer[rsmsP.j][rsmsP.i] / (double) RSMS_P_max_smoothing;
		}
		rsmsP.currentData[rsmsP.i] = (uint16_t) rsmsP.sum;
	}
}

void RSMS_P_logData() {
	for (rsmsP.i = 0; rsmsP.i < RSMS_P_n_sensors; rsmsP.i++) {
		Logger_logData(rsmsP.message[rsmsP.i], 3, rsmsP.currentTime[rsmsP.i], rsmsP.currentPres[rsmsP.i]);
	}
}

void RSMS_P_printData() {
	xprintf("Pressure:\t");
	for (rsmsP.i = 0; rsmsP.i < RSMS_P_n_sensors; rsmsP.i++) {
//		xprintf("Time: %06u ", rsmsP.currentTime[rsmsP.i]);
		xprintf("Value: %06u\t\t", rsmsP.currentData[rsmsP.i]);
	}
	xprintf("\n");
}

void RSMS_P_calcPoly() {
	for (rsmsP.i = 0; rsmsP.i < RSMS_P_n_sensors; rsmsP.i++) {
		rsmsP.currentPres[rsmsP.i] = (uint16_t) (100
				* (rsmsP.polyCoef[rsmsP.i][0] + rsmsP.polyCoef[rsmsP.i][1] * rsmsP.currentData[rsmsP.i]));
	}
}

void RSMS_P_printPressure() {
	xprintf("Pressure:\t");
	for (rsmsP.i = 0; rsmsP.i < RSMS_P_n_sensors; rsmsP.i++) {
//		xprintf("Time: %06u ", rsmsP.currentTime[rsmsP.i]);
		xprintf("Value: %06u 10mbar\t\t", rsmsP.currentPres[rsmsP.i]);
	}
	xprintf("\n");
}

void RSMS_P_sendData() {
	for (rsmsP.i = 0; rsmsP.i < RSMS_P_n_sensors; rsmsP.i += 2) {
		rsmsP.dataMessage.header.timeStamp = rsmsP.currentTime[rsmsP.i];

		rsmsP.dataMessage.data1.header.ID = RSMS_PRESSURE_1 + rsmsP.i;
		rsmsP.dataMessage.data1.payload = rsmsP.currentPres[rsmsP.i];
		rsmsP.dataMessage.data2.header.ID = RSMS_PRESSURE_1 + rsmsP.i + 1;
		rsmsP.dataMessage.data2.payload = rsmsP.currentPres[rsmsP.i + 1];

		CANI_sendData(rsmsP.bus, &rsmsP.dataMessage);
	}
}

#endif
