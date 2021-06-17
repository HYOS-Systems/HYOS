/*
 * rsms_t.c
 *
 *  Created on: Jun 16, 2021
 *      Author: Bayram
 */

#include "systems/compass/rsms/rsms_p.h"

#ifdef _H_RSMS_P

#define RSMS_P_max_smoothing 5
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

	ADS_8688 adc;
} RSMS_P_variables;

RSMS_P_variables rsms_p_struct;

void RSMS_P_initMessages() {
	for (rsms_p_struct.i = 0; rsms_p_struct.i < RSMS_P_n_sensors; rsms_p_struct.i++) {
		snprintf(rsms_p_struct.message[rsms_p_struct.i], 4, "T_%u", rsms_p_struct.i);
	}
}

void RSMS_P_initADC() {
	rsms_p_struct.adc.hspi = rsms_struct->hspi_p;
	rsms_p_struct.adc.cs_pin = rsms_struct->chipSelect_p.pin;
	rsms_p_struct.adc.cs_port = rsms_struct->chipSelect_p.port;
	rsms_p_struct.adc.active_pins = 0b11000011;
	rsms_p_struct.adc.input_range = ZT1V25;
}

void RSMS_P_initStruct() {
	rsms_p_struct.i = 0;
	rsms_p_struct.j = 0;
	rsms_p_struct.counter = 0;
}

void RSMS_P_init(RSMS_PeripheralStruct *rsms_peripheralStruct) {
	rsms_struct = rsms_peripheralStruct;
	RSMS_P_initStruct();
	RSMS_P_initADC();
	RSMS_P_initMessages();
}

void RSMS_P_measureData() {
	for (rsms_p_struct.i = 0; rsms_p_struct.i < RSMS_P_n_sensors; rsms_p_struct.i++) {
		ADS_measure(&rsms_p_struct.adc);
		rsms_p_struct.buffer[rsms_p_struct.counter][rsms_p_struct.i] = rsms_p_struct.adc.dataBuffer;
	}
	rsms_p_struct.counter++;
	if (rsms_p_struct.counter >= RSMS_P_max_smoothing) {
		rsms_p_struct.counter = 0;
	}
	for (rsms_p_struct.i = 0; rsms_p_struct.i < RSMS_P_n_sensors; rsms_p_struct.i++) {
		rsms_p_struct.sum = 0;
		for (rsms_p_struct.j = 0; rsms_p_struct.j < RSMS_P_max_smoothing; rsms_p_struct.j++) {
			rsms_p_struct.sum += rsms_p_struct.buffer[rsms_p_struct.j][rsms_p_struct.i] / (double) RSMS_P_n_sensors;
		}
		rsms_p_struct.currentData[rsms_p_struct.i] = (uint16_t) rsms_p_struct.sum;
	}
}

void RSMS_P_logData() {
	for (rsms_p_struct.i = 0; rsms_p_struct.i < RSMS_P_n_sensors; rsms_p_struct.i++) {
		Logger_logData(rsms_p_struct.message[rsms_p_struct.i], 4, rsms_p_struct.currentTime[rsms_p_struct.i],
				rsms_p_struct.currentData[rsms_p_struct.i]);
	}
}

void RSMS_P_printData() {
	xprintf("Pressure:\t");
	for (rsms_p_struct.i = 0; rsms_p_struct.i < RSMS_P_n_sensors; rsms_p_struct.i++) {
		xprintf("Time: %06u ", rsms_p_struct.currentTime[rsms_p_struct.i]);
		xprintf("Value: %06u\t\t", rsms_p_struct.currentTime[rsms_p_struct.i]);
	}
	xprintf("\n");
}

#endif
