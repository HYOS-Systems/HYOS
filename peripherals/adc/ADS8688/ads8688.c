/*
 * ads8688.c
 *
 *  Created on: 09.12.2020
 *      Author: Bayram
 */

#include "peripherals/adc/ADS8688/ads8688.h"

#ifdef INC_ADS8688_H_

uint16_t datTx[2];
uint16_t datRx[2];

void ADS_appendData(char* message, uint16_t len, uint16_t data){
	uint8_t data_len;
	getNumberLength(&data_len, data);
	uint16_t pay_len = data_len + len + 1;
	char payload[pay_len];
	sprintf(payload, "%s%d\n", message, data);
	xprintf(payload);
}

void ADS_writeRegister(uint16_t command, ADS_8688* ads){
	datTx[0] = command;
	datTx[1] = 0x0000;
	datRx[0] = 0x0000;
	datRx[1] = 0x0000;

	HAL_GPIO_WritePin(ads->cs_port, ads->cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(ads->hspi, (uint8_t*) datTx, (uint8_t*) datRx, 2, 100);
	HAL_GPIO_WritePin(ads->cs_port, ads->cs_pin, GPIO_PIN_SET);

	ADS_appendData("SPI command: ", 13, datTx[0]);
	ADS_appendData("SPI Status: ", 12, status);
	ADS_appendData("Data Written: ", 14, datRx[1]);
}

void ADS_setInputRange(ADS_8688* ads){
	if (0b00000001 & ads->active_pins){
		ADS_writeRegister(0x0B00 | (uint8_t) ads->input_range, ads);  // Input Range Channel 0
	}
	if (0b00000010 & ads->active_pins){
		ADS_writeRegister(0x0D00 | (uint8_t) ads->input_range, ads);  // Input Range Channel 1
	}
	if (0b00000100 & ads->active_pins){
		ADS_writeRegister(0x0F00 | (uint8_t) ads->input_range, ads);  // Input Range Channel 2
	}
	if (0b00001000 & ads->active_pins){
		ADS_writeRegister(0x1100 | (uint8_t) ads->input_range, ads);  // Input Range Channel 3
	}
	if (0b00010000 & ads->active_pins){
		ADS_writeRegister(0x1300 | (uint8_t) ads->input_range, ads);  // Input Range Channel 4
	}
	if (0b00100000 & ads->active_pins){
		ADS_writeRegister(0x1500 | (uint8_t) ads->input_range, ads);  // Input Range Channel 5
	}
	if (0b01000000 & ads->active_pins){
		ADS_writeRegister(0x1700 | (uint8_t) ads->input_range, ads);  // Input Range Channel 6
	}
	if (0b10000000 & ads->active_pins){
		ADS_writeRegister(0x1900 | (uint8_t) ads->input_range, ads);  // Input Range Channel 7
	}
}

void ADS_init(ADS_8688* ads) {
	xprintf("Opened File: ADX.txt\n");
	xprintf("Initialize ADC:\n");

	xprintf("\nDevice Reset:\n");
	ADS_writeRegister(0x8500, ads);  // Device Reset

	xprintf("\nAuto Channel:\n");
	ADS_writeRegister(0xA000, ads);  // Auto Channel with Reset

	xprintf("\nEnable Channel 4:\n");
	ADS_writeRegister(0x0300 | ads->active_pins, ads);  // Enable channel 4

	xprintf("\nOther Channels Power Down:\n");
	ADS_writeRegister(0x0500 | ((uint8_t) (~ads->active_pins)), ads);  // Other channels Power Down

	xprintf("\nInput Range:\n");
	ADS_setInputRange(ads);

	xprintf("\nStart Sampling:\n");
	ADS_writeRegister(0xA000, ads);  // Start Sampling

	xprintf("\nInitialization finished.\n");
	xprintf("Closed File.");
}

uint16_t ADS_measure(ADS_8688* ads){
	datTx[0] = 0x0000;
	datTx[1] = 0x0000;
	datRx[0] = 0x0000;
	datRx[1] = 0x0000;

	HAL_GPIO_WritePin(ads->cs_port, ads->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(ads->hspi, (uint8_t*) datTx, (uint8_t*) datRx, 2, 100);
	HAL_GPIO_WritePin(ads->cs_port, ads->cs_pin, GPIO_PIN_SET);

	return datRx[1];
}

#endif /* STM32F4xx_HAL_SPI_H */

