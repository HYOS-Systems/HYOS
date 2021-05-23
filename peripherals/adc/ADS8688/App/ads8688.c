/*
 * ads8688.c
 *
 *  Created on: 09.12.2020
 *      Author: Bayram
 */

#include "peripherals/adc/ADS8688/App/ads8688.h"

#ifdef INC_ADS8688_H_

uint16_t cnt;
GPIO_TypeDef* cs_port;
uint16_t cs_pin;
SPI_HandleTypeDef spi_handle;

void writeStatus(char* message, uint16_t len, HAL_StatusTypeDef status){
	uint8_t numbLen;
	uint16_t data = (uint16_t) status;
	getNumberLength(&numbLen, data);
	uint16_t pay_len = 27 + numbLen + len;
	char payload[pay_len];
	sprintf(payload, "Initialize: %s with status: %d\n", message, data);
	SDFH_writeToFile(payload, pay_len);
}

void appendData(char* message, uint16_t len, uint16_t data){
	uint8_t data_len;
	getNumberLength(&data_len, data);
	uint16_t pay_len = data_len + len + 1;
	char payload[pay_len];
	sprintf(payload, "%s%d\n", message, data);
	SDFH_writeToFile(payload, pay_len);
}

void writeData(char* message, uint16_t len, uint16_t data){
	uint8_t data_len;
	getNumberLength(&data_len, data);
	uint16_t pay_len = data_len + len + 1;
	char payload[pay_len];
	sprintf(payload, "%s%d\n", message, data);

	getNumberLength(&data_len, cnt);
	data_len += 4;
	char fileName[data_len];
	sprintf(fileName, "%d.txt", cnt);

	SDFH_writeSingle(fileName, payload, pay_len);
	cnt++;
}

void writeRegister(uint16_t command, ADS_8688* ads){
	uint16_t datTx[2];
	uint16_t datRx[2];

	datTx[0] = command;
	datTx[1] = 0x0000;
	datRx[0] = 0x0000;
	datRx[1] = 0x0000;

	HAL_GPIO_WritePin(ads->cs_port, ads->cs_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(ads->hspi, (uint8_t*) datTx, (uint8_t*) datRx, 2, 100);
	HAL_GPIO_WritePin(ads->cs_port, ads->cs_pin, GPIO_PIN_SET);

	appendData("SPI Status: ", 12, status);
	appendData("Data Written: ", 14, datRx[0]);
	appendData("Data Written: ", 14, datRx[1]);
}

void initADC(ADS_8688* ads) {
	SDFH_openFile("ADC.txt");
	SDFH_writeToFile("Initialize ADC:\n", 16);
	cnt = 0; // init counter for measurement

	SDFH_writeToFile("\nDevice Reset:\n", 15);
	writeRegister(0x8500, ads);  // Device Reset

	SDFH_writeToFile("\nAuto Channel:\n", 15);
	writeRegister(0xA000, ads);  // Auto Channel with Reset

	SDFH_writeToFile("\nEnable Channel 4:\n", 19);
	writeRegister(0x0310, ads);  // Enable channel 4

	SDFH_writeToFile("\nOther Channels Power Down:\n", 28);
	writeRegister(0x05EF, ads);  // Other channels Power Down

	SDFH_writeToFile("\nInput Range:\n", 14);
	writeRegister(0x1306, ads);  // Input Range Channel 4 to: 0 - 1.25 * V_ref

	SDFH_writeToFile("\nStart Sampling:\n", 17);
	writeRegister(0xA000, ads);  // Start Sampling

	SDFH_writeToFile("\nInitialization finished.\n", 26);
	SDFH_closeFile();
}

void measure(ADS_8688* ads){
	uint16_t txData[2];
	uint16_t rxData[2];
	uint16_t result;

	txData[0] = 0x0000;
	txData[1] = 0x0000;
	rxData[0] = 0x0000;
	rxData[1] = 0x0000;
	HAL_GPIO_WritePin(ads->cs_port, ads->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(ads->hspi, (uint8_t*) txData, (uint8_t*) rxData, 2, 100);
	HAL_GPIO_WritePin(ads->cs_port, ads->cs_pin, GPIO_PIN_SET);

	result = rxData[1];
	writeData("Measure: ", 9, result);
}

#endif /* STM32F4xx_HAL_SPI_H */

