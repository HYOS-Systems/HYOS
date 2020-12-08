/*
 * dataHandling.c
 *
 *  Created on: 06.10.2020
 *      Author: Bayram
 */

#include "sd/App/SDfileHandling.h"
#ifdef STM32F4xx_HAL_SD_H

// FileData
FATFS fat = { 0 };
FIL file;
UINT state;
FRESULT fstate = FR_INT_ERR;

// TimeData
static uint16_t time = 0;
static uint16_t sec = 0;
static uint16_t len;
static uint16_t lenF;
static uint16_t lenD;

FRESULT initSDfileHandling() {
	FATFS_LinkDriver(&SD_Driver, SDPath);

	FRESULT fstate;
	for (int i = 0; i < 1; i++) {
		HAL_Delay(500);
		fstate = f_mount(&fat, SDPath, 1);
		xprintf("Mount with fstate: %d\n", fstate);
		if (fstate == FR_OK)
			break;
	}
	if (fstate == FR_OK) {
		xprintf("Mount Started.\n");
		char fileName[] = "init.txt";
		fstate = f_open(&file, fileName, FA_WRITE | FA_CREATE_ALWAYS);
		if (fstate == FR_OK) {
			xprintf("File opened successfully.\n");
			char data[] = "initialized.\n";
			fstate = f_write(&file, data, sizeof(data), &state);
			if (fstate == FR_OK) {
				xprintf("File written succsesfully.\n");
			}
			f_close(&file);
		}
	} else {
		xprintf("Mount failed.\n");
	}
	return fstate;
}

void getNumberLength(uint16_t *len, uint16_t sz) {
	if (sz > 99999) {
		char err[] =
				"Error, message must not exceed the length of 99999 characters (including CR+LF).\r\n";
		xprintf(err);
		*len = 6;
		return;
	} else if (sz > 9999) {
		*len = 5;
	} else if (sz > 999) {
		*len = 4;
	} else if (sz > 99) {
		*len = 3;
	} else if (sz > 9) {
		*len = 2;
	} else {
		*len = 1;
	}
}

void getFileNameLength(uint16_t *len, uint16_t sec) {
	getNumberLength(len, sec);
	*len += 9;
}

void getFileName(char *sendMsg, uint16_t sec) {
	sprintf(sendMsg, "Data_%d.bin", sec);
}

void getDataLength(uint16_t *len, uint16_t data1, uint16_t data2) {
	uint16_t len1;
	uint16_t len2;
	getNumberLength(&len1, data1);
	getNumberLength(&len2, data2);
	*len = len1 + len2 + 2;
}

void packData(char *sendMsg, uint16_t data1, uint16_t data2) {
	sprintf(sendMsg, "%d\t%d\n", data1, data2);
}

FRESULT SDFH_openFile(char* fileName) {
	return f_open(&file, fileName, FA_WRITE | FA_CREATE_ALWAYS);
}

FRESULT SDFH_closeFile() {
	return f_close(&file);
}

FRESULT SDFH_writeToFile(char* input, uint16_t inputLength) {
	return f_write(&file, input, inputLength, &state);
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

#endif // STM32F4xx_HAL_SD_H
