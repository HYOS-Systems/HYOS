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

RSMS_T_variables rsms_t_struct;

void RSMS_T_initMessages() {
	for (rsms_t_struct.i = 0; rsms_t_struct.i < RSMS_T_n_sensors; rsms_t_struct.i++) {
		snprintf(rsms_t_struct.message[rsms_t_struct.i], 4, "T_%u", rsms_t_struct.i);
	}
}

void RSMS_T_initADC() {
	rsms_t_struct.adc.hspi = rsms_struct->hspi_p;
	rsms_t_struct.adc.cs_pin = rsms_struct->chipSelect_p.pin;
	rsms_t_struct.adc.cs_port = rsms_struct->chipSelect_p.port;
	rsms_t_struct.adc.active_pins = 0b11000011;
	rsms_t_struct.adc.input_range = ZT1V25;
}

void RSMS_T_initStruct() {
	rsms_t_struct.i = 0;
	rsms_t_struct.j = 0;
	rsms_t_struct.counter = 0;
}

void RSMS_T_init(RSMS_PeripheralStruct *rsms_peripheralStruct) {
	rsms_struct = rsms_peripheralStruct;
	RSMS_T_initStruct();
	RSMS_T_initADC();
	RSMS_T_initMessages();
}

void RSMS_T_measureData() {
	for (rsms_t_struct.i = 0; rsms_t_struct.i < RSMS_T_n_sensors; rsms_t_struct.i++) {
		ADS_measure(&rsms_t_struct.adc);
		rsms_t_struct.buffer[rsms_t_struct.counter][rsms_t_struct.i] = rsms_t_struct.adc.dataBuffer;
	}
	rsms_t_struct.counter++;
	if (rsms_t_struct.counter >= RSMS_T_max_smoothing) {
		rsms_t_struct.counter = 0;
	}
	for (rsms_t_struct.i = 0; rsms_t_struct.i < RSMS_T_n_sensors; rsms_t_struct.i++) {
		rsms_t_struct.sum = 0;
		for (rsms_t_struct.j = 0; rsms_t_struct.j < RSMS_T_max_smoothing; rsms_t_struct.j++) {
			rsms_t_struct.sum += rsms_t_struct.buffer[rsms_t_struct.j][rsms_t_struct.i] / (double) RSMS_T_n_sensors;
		}
		rsms_t_struct.currentData[rsms_t_struct.i] = (uint16_t) rsms_t_struct.sum;
	}
}

void RSMS_T_logData() {
	for (rsms_t_struct.i = 0; rsms_t_struct.i < RSMS_T_n_sensors; rsms_t_struct.i++) {
		Logger_logData(rsms_t_struct.message[rsms_t_struct.i], 4, rsms_t_struct.currentTime[rsms_t_struct.i],
				rsms_t_struct.currentData[rsms_t_struct.i]);
	}
}

void RSMS_T_printData() {
	xprintf("Temperature:\t");
	for (rsms_t_struct.i = 0; rsms_t_struct.i < RSMS_T_n_sensors; rsms_t_struct.i++) {
		xprintf("Time: %06u ", rsms_t_struct.currentTime[rsms_t_struct.i]);
		xprintf("Value: %06u\t\t", rsms_t_struct.currentTime[rsms_t_struct.i]);
	}
	xprintf("\n");
}


#endif
