/*
 * gopro.c
 *
 *  Created on: Jun 18, 2021
 *      Author: Bayram
 */

#include "peripherals/ina233/ina233.h"
#ifdef PERIPHERALS_INA233_H_

typedef struct {
	uint8_t calibrationAdd;
	uint8_t calibrationValue[2];
	uint8_t calibrationMessage[3];
	uint8_t readAdd;
	uint8_t readMessage[4];

	uint8_t someAdd;
} INA_DATA;

INA_DATA inaData;

void INA_initStruct() {
	inaData.calibrationAdd = 0xD4;
	inaData.readAdd = 0x89;
	// 0x0A00
	inaData.calibrationValue[0] = 0xFF; // LSB
	inaData.calibrationValue[1] = 0xFF;


	// Calibrate Message
	inaData.calibrationMessage[0] = inaData.calibrationAdd;
	inaData.calibrationMessage[1] = inaData.calibrationValue[0];
	inaData.calibrationMessage[2] = inaData.calibrationValue[1];

	inaData.someAdd = 0x99;
}
void INA_init(INA233 *ina) {
	INA_initStruct();
	HAL_StatusTypeDef stat = HAL_I2C_Master_Transmit(ina->hi2c, ina->address, inaData.calibrationMessage, 3, 100);
	stat = HAL_I2C_Mem_Read(ina->hi2c, ina->address, inaData.calibrationAdd, 1, inaData.readMessage, 3, 100);
	stat = stat;
}

short INA_measure(INA233 *ina) {
	inaData.readMessage[0] = 0;
	inaData.readMessage[1] = 0;
	inaData.readMessage[2] = 0;
	HAL_StatusTypeDef stat;
	stat = HAL_I2C_Mem_Read(ina->hi2c, ina->address, inaData.readAdd, 1, inaData.readMessage, 2, 100);
	short res = inaData.readMessage[0] | inaData.readMessage[1] << 8;
	return res;
}

#endif
