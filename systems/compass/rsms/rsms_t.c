/*
 * rsms_t.c
 *
 *  Created on: Jun 16, 2021
 *      Author: Bayram
 */

#include "systems/compass/rsms/rsms_t.h"
#ifdef _H_RSMS_T

RSMS_PeripheralStruct *rsms_struct;
ADS_8688 adc;

uint8_t i;
uint16_t j;

uint16_t counter;
#define max_smoothing 5
#define n_sensors 4
uint16_t buffer[max_smoothing][n_sensors];
char message[n_sensors][4];
uint16_t currentData[n_sensors];
uint32_t currentTime[n_sensors];
long double sum;

void RSMS_T_initADC() {
	adc.hspi = rsms_struct->hspi_t;
	adc.cs_pin = rsms_struct->chipSelect_t.pin;
	adc.cs_port = rsms_struct->chipSelect_t.port;
	adc.active_pins = 0b11000011;
	adc.input_range = ZT1V25;
}

void RSMS_T_initMessages() {
	for (i = 0; i < n_sensors; i++) {
		snprintf(message[i], 4, "T_%u", i);
	}
}

void RSMS_T_init(RSMS_PeripheralStruct *rsms_peripheralStruct) {
	rsms_struct = rsms_peripheralStruct;
	RSMS_T_initADC();
}

void RSMS_T_measureTemperature() {
	for (i = 0; i < n_sensors; i++) {
		ADS_measure(&adc);
		buffer[counter][i] = adc.dataBuffer;
	}
	counter++;
	if (counter >= max_smoothing) {
		counter = 0;
	}
	for (i = 0; i < n_sensors; i++) {
		sum = 0;
		for (j = 0; j < max_smoothing; j++) {
			sum += buffer[j][i] / (double) n_sensors;
		}
		currentData[i] = (uint16_t) sum;
	}
}

void RSMS_T_logData() {
	for (i = 0; i < n_sensors; i++) {
		Logger_logData(message[i], 4, currentTime[i], currentData[i]);
	}
}

void RSMS_T_printData() {

}

#endif
