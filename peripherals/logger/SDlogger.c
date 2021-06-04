/*
 * logger.c
 *
 *  Created on: Jun 3, 2021
 *      Author: Bayram
 */

#include "peripherals/logger/SDlogger.h"

uint16_t fileCounter;
uint32_t dataCounter;
uint32_t maxDataPerFile;

uint8_t overheadLength;

char prefix[2];
char fileName[9];
char dataString[255];

void LoggerOpenFile() {
	snprintf(fileName, 9, "%s%06u", prefix, fileCounter);
	SDFH_openFile(fileName);
}

void LoggerCloseFile() {
	SDFH_closeFile();
}

void initLogger() {
	uint8_t dataStringLength = 5;
	uint8_t timeStampStringLength = 10;
	uint8_t seperatorLength = 4; // ,,\n\0
	overheadLength = dataStringLength + timeStampStringLength + seperatorLength;

	fileCounter = 0;
	dataCounter = 0;
	maxDataPerFile = 1000;
	prefix[0] = 'D';
	prefix[1] = 'T';
	LoggerOpenFile();
}

void LoggerSetMaxDataPerFile(uint32_t maxData) {
	maxDataPerFile = maxData;
}

void Loggerlog(const char *message, uint8_t messageLength, uint32_t time_stamp,
		uint16_t data) {
	uint8_t len = messageLength + overheadLength;

	snprintf(dataString, len, "%s,%010u,%06u\n", message, time_stamp, data);
	SDFH_writeToFile(dataString, len);

	dataCounter++;
	if (dataCounter >= maxDataPerFile) {
		LoggerCloseFile();
		fileCounter++;
		LoggerOpenFile();
		dataCounter = 0;
	}
}
