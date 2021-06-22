/*
 * rsms_t.c
 *
 *  Created on: Jun 16, 2021
 *      Author: Bayram
 */

#include "systems/compass/rsms/rsms_t.h"

#ifdef _H_RSMS_T

#define RSMS_T_max_smoothing 1
#define RSMS_T_n_sensors 4

RSMS_PeripheralStruct *rsms_struct;

typedef struct {
	uint8_t i;
	uint16_t j;
	uint16_t counter;
	uint16_t buffer[RSMS_T_max_smoothing][RSMS_T_n_sensors];
	char message[RSMS_T_n_sensors][4];
	uint16_t currentData[RSMS_T_n_sensors];
	uint32_t currentTime[RSMS_T_n_sensors];
	long double sum;
	double polyCoef[RSMS_T_n_sensors][3];

	uint16_t currentTemp[RSMS_T_n_sensors];
	CANBus *bus;
	CANP_DataMessage dataMessage;
	ADS_8688 adc;
} RSMS_T_variables;

RSMS_T_variables rsmsT;

void RSMS_T_initMessages() {
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		snprintf(rsmsT.message[rsmsT.i], 4, "T_%u", rsmsT.i);
	}
}

void RSMS_T_initADC() {
	rsmsT.adc.hspi = rsms_struct->hspi_t;
	rsmsT.adc.cs_pin = rsms_struct->chipSelect_t.pin;
	rsmsT.adc.cs_port = rsms_struct->chipSelect_t.port;
	rsmsT.adc.active_pins = 0b11000011;
//	rsmsT.adc.input_range = PM1V25;
//	rsmsT.adc.input_range = PM2V5;
//	rsmsT.adc.input_range = PM2V5;
//	rsmsT.adc.input_range = ZT2V5;
	rsmsT.adc.input_range = ZT1V25;

	ADS_init(&rsmsT.adc);
}

void RSMS_T_initStruct() {
	rsmsT.i = 0;
	rsmsT.j = 0;
	rsmsT.counter = 0;

	// Arch_Dept
	rsmsT.polyCoef[0][0] = -8.38247E1;
	rsmsT.polyCoef[0][1] = 2.77975E-03;
	rsmsT.polyCoef[0][2] = 1.26347E-07;

	rsmsT.polyCoef[1][0] = -9.82874E+01;
	rsmsT.polyCoef[1][1] = 3.96460E-03;
	rsmsT.polyCoef[1][2] = 1.02905E-07;

	rsmsT.polyCoef[2][0] = -1.02257E+02;
	rsmsT.polyCoef[2][1] = 4.39660E-03;
	rsmsT.polyCoef[2][2] = 9.22682E-08;

	rsmsT.polyCoef[3][0] = -1.06720E+02;
	rsmsT.polyCoef[3][1] = 4.68381E-03;
	rsmsT.polyCoef[3][2] = 8.89494E-08;
}

void RSMS_T_initCAN(CANBus *bus) {
	rsmsT.bus = bus;
	rsmsT.dataMessage.header.targetMCU = IFC;
	rsmsT.dataMessage.header.messageType = DATA;

	rsmsT.dataMessage.data1.header.status = DATA_OK;
	rsmsT.dataMessage.data2.header.status = DATA_OK;
}

void RSMS_T_init(RSMS_PeripheralStruct *rsms_peripheralStruct, CANBus *bus) {
	rsms_struct = rsms_peripheralStruct;
	RSMS_T_initStruct();
	RSMS_T_initADC();
	RSMS_T_initMessages();
	RSMS_T_initCAN(bus);
}

void RSMS_T_measureData() {
	// Measure
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		ADS_measure(&rsmsT.adc);
		rsmsT.buffer[rsmsT.counter][rsmsT.i] = rsmsT.adc.dataBuffer;
		rsmsT.currentTime[rsmsT.i] = HYOS_GetTick();
	}

	// Count data for Smoothing
	rsmsT.counter++;
	if (rsmsT.counter >= RSMS_T_max_smoothing) {
		rsmsT.counter = 0;
	}

	// Smooth Data
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		rsmsT.sum = 0;
		for (rsmsT.j = 0; rsmsT.j < RSMS_T_max_smoothing; rsmsT.j++) {
			rsmsT.sum += rsmsT.buffer[rsmsT.j][rsmsT.i] / (double) RSMS_T_max_smoothing;
		}
		rsmsT.currentData[rsmsT.i] = (uint16_t) rsmsT.sum;
	}
}

void RSMS_T_logData() {
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		Logger_logData(rsmsT.message[rsmsT.i], 3, rsmsT.currentTime[rsmsT.i], rsmsT.currentTemp[rsmsT.i]);
	}
}

void RSMS_T_printData() {
	xprintf("Temperature:\t");
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		xprintf("Time: %06u ", rsmsT.currentTime[rsmsT.i]);
		xprintf("Value: %06u\t\t", rsmsT.currentData[rsmsT.i]);
	}
	xprintf("\n");
}

void RSMS_T_calcPoly() {
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		rsmsT.currentTemp[rsmsT.i] = (uint16_t) (100
//				* (273.15 + rsmsT.polyCoef[rsmsT.i][0] + rsmsT.polyCoef[rsmsT.i][1] * rsmsT.currentData[rsmsT.i]
				* (273.15
						+ (rsmsT.polyCoef[rsmsT.i][0] + rsmsT.polyCoef[rsmsT.i][1] * rsmsT.currentData[rsmsT.i]
								+ rsmsT.polyCoef[rsmsT.i][2] * rsmsT.currentData[rsmsT.i] * rsmsT.currentData[rsmsT.i])
								/ 2));
	}
}

void RSMS_T_printTemperature() {
	xprintf("Temperature:\t");
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		xprintf("Time: %06u ", rsmsT.currentTime[rsmsT.i]);
		xprintf("Value: %06u deg\t\t", rsmsT.currentTemp[rsmsT.i]);
	}
	xprintf("\n");
}

void RSMS_T_sendData() {
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i += 2) {
		rsmsT.dataMessage.header.timeStamp = rsmsT.currentTime[rsmsT.i];

		rsmsT.dataMessage.data1.header.ID = RSMS_PT_1 + rsmsT.i;
		rsmsT.dataMessage.data1.payload = rsmsT.currentTemp[rsmsT.i];
		rsmsT.dataMessage.data2.header.ID = RSMS_PT_1 + rsmsT.i + 1;
		rsmsT.dataMessage.data2.payload = rsmsT.currentTemp[rsmsT.i + 1];

		CANI_sendData(rsmsT.bus, &rsmsT.dataMessage);
	}
}

#endif
