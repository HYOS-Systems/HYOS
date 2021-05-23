/*
 * dataHandling.c
 *
 *  Created on: 06.10.2020
 *      Author: Bayram
 */

#include "connectivity/sd/App/SDfileHandling.h"

// Check for valid Initialization
//#ifdef INC_DATAHANDLING_H_
#ifdef INC_DATAHANDLING_H_

// FileData
FATFS fat = { 0 };
FIL file;
UINT state;
FRESULT fstate = FR_INT_ERR;

// TimeData
static uint16_t time = 0;
static uint16_t sec = 0;
static uint8_t len;
static uint8_t lenF;
static uint8_t lenD;

FRESULT initSDfileHandling() {
//	xprintf("Start initSDfileHandling\n");
//
	FRESULT fstate;
	//HAL_Delay(500);
	fstate = f_mount(0, SDPath, 0);
	xprintf("First unmount with fstate: %d\n", fstate);
	fstate = f_mount(&fat, SDPath, 1);
	//xprintf("First mount with fstate: %d\n", fstate);

//	if (fstate == FR_OK) {
//		//xprintf("Mount Started.\n");
//		char fileName[] = "init.txt";
//		//f_close(&file);
//		fstate = f_open(&file, fileName, FA_WRITE | FA_CREATE_ALWAYS);
//		if (fstate == FR_OK) {
//			//xprintf("File opened successfully.\n");
//			char data[] = "initialized.\n";
//			fstate = f_write(&file, data, sizeof(data), &state);
////			if (fstate == FR_OK) {
////				xprintf("File written succsesfully.\n");
////			}
//			f_close(&file);
//		}
//	} else {
//		//xprintf("Mount failed.\n");
//	}
	return fstate;
}

void getFileNameLength(uint8_t *len, uint16_t sec) {
	getNumberLength(len, sec);
	*len += 9;
}

void getFileName(char *sendMsg, uint16_t sec) {
	sprintf(sendMsg, "%d.txt", sec);
}

void getDataLength(uint8_t *len, uint16_t data1, uint16_t data2) {
	uint8_t len1;
	uint8_t len2;
	getNumberLength(&len1, data1);
	getNumberLength(&len2, data2);
	*len = len1 + len2 + 2;
}

void packData(char *sendMsg, uint16_t data1, uint16_t data2) {
	sprintf(sendMsg, "%d\t%d\n", data1, data2);
}

FRESULT SDFH_openFile(char* fileName) {
	//f_close(&file);
	return f_open(&file, fileName, FA_WRITE | FA_CREATE_ALWAYS);
}

FRESULT SDFH_closeFile() {
	return f_close(&file);
}

FRESULT SDFH_writeToFile(char* input, uint16_t inputLength) {
	return f_write(&file, input, inputLength, &state);
}

FRESULT SDFH_writeIntToFile(int32_t input) {
	if (input < 0){
		uint32_t uns = -1 * input;
		getNumberLength(&len, uns);
		len += 1;
	} else {
		getNumberLength(&len, input);
	}

	char temp[len];
	sprintf(temp, "%d", (int) input);
	return SDFH_writeToFile(temp, len);
}

FRESULT SDFH_writeSingle(char* fileName, char* input, uint16_t inputLength) {
	FRESULT fstate = SDFH_openFile(fileName);
	if (fstate == FR_OK) {
		fstate = SDFH_writeToFile(input, inputLength);
	}
	SDFH_closeFile();
	return fstate;
}

void SDFH_writeTest() {
	time = 0;
	sec = 101;
	xprintf("Time: %d\n", sec);
	getFileNameLength(&lenF, sec);
	char fileName[lenF];
	getFileName(fileName, sec);
	getDataLength(&lenD, sec, time);
	char data1[lenD];
	packData(data1, sec, time);
	fstate = SDFH_writeSingle(fileName, data1, lenD);
	xprintf("File was opened wit state: %d", fstate);
}

void SDFH_normalOperation() {
//	fstate = FR_INT_ERR;
	if (time == 0 && sec < 16) {
		xprintf("Time: %d\n", sec);
		getFileNameLength(&len, sec);
		char fileName[len];
		getFileName(fileName, sec);
		fstate = f_open(&file, fileName, FA_WRITE | FA_CREATE_ALWAYS);
	}

	if (fstate == FR_OK) {
		//			xprintf("File opened successfully.\n");
		getDataLength(&len, sec, time);
		char data[len];
		packData(data, sec, time);
		fstate = f_write(&file, data, len, &state);
		if (fstate == FR_OK) {
//			xprintf("File written succsesfully.\n");
		}
	}

	time++;
	if (time >= 10000) {
		time = 0;
		sec++;
		f_close(&file);
	}
}

#endif // INC_DATAHANDLING_H_
