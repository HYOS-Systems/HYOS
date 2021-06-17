/*
 * rsms_t.c
 *
 *  Created on: Jun 16, 2021
 *      Author: Bayram
 */

#include "systems/compass/rsms/rsms_t.h"

#ifdef _H_RSMS_T

#define RSMS_T_max_smoothing 5
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

	ADS_8688 adc;
} RSMS_T_variables;

RSMS_T_variables rsmsT;

void RSMS_T_initMessages() {
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		snprintf(rsmsT.message[rsmsT.i], 4, "T_%u", rsmsT.i);
	}
}

void RSMS_T_initADC() {
	rsmsT.adc.hspi = rsms_struct->hspi_p;
	rsmsT.adc.cs_pin = rsms_struct->chipSelect_p.pin;
	rsmsT.adc.cs_port = rsms_struct->chipSelect_p.port;
	rsmsT.adc.active_pins = 0b11000011;
	rsmsT.adc.input_range = ZT1V25;
}

void RSMS_T_initStruct() {
	rsmsT.i = 0;
	rsmsT.j = 0;
	rsmsT.counter = 0;
}

void RSMS_T_init(RSMS_PeripheralStruct *rsms_peripheralStruct) {
	rsms_struct = rsms_peripheralStruct;
	RSMS_T_initStruct();
	RSMS_T_initADC();
	RSMS_T_initMessages();
}

void RSMS_T_measureData() {
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		ADS_measure(&rsmsT.adc);
		rsmsT.buffer[rsmsT.counter][rsmsT.i] = rsmsT.adc.dataBuffer;
	}
	rsmsT.counter++;
	if (rsmsT.counter >= RSMS_T_max_smoothing) {
		rsmsT.counter = 0;
	}
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		rsmsT.sum = 0;
		for (rsmsT.j = 0; rsmsT.j < RSMS_T_max_smoothing; rsmsT.j++) {
			rsmsT.sum += rsmsT.buffer[rsmsT.j][rsmsT.i] / (double) RSMS_T_n_sensors;
		}
		rsmsT.currentData[rsmsT.i] = (uint16_t) rsmsT.sum;
	}
}

void RSMS_T_logData() {
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		Logger_logData(rsmsT.message[rsmsT.i], 4, rsmsT.currentTime[rsmsT.i],
				rsmsT.currentData[rsmsT.i]);
	}
}

void RSMS_T_printData() {
	xprintf("Temperature:\t");
	for (rsmsT.i = 0; rsmsT.i < RSMS_T_n_sensors; rsmsT.i++) {
		xprintf("Time: %06u ", rsmsT.currentTime[rsmsT.i]);
		xprintf("Value: %06u\t\t", rsmsT.currentTime[rsmsT.i]);
	}
	xprintf("\n");
}


#endif
